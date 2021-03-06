#include "tracker.h"

#ifndef WINDOWS
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <netdb.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "ctorrent.h"
#include "peerlist.h"
#include "httpencode.h"
#include "bencode.h"
#include "setnonblock.h"
#include "connect_nonb.h"
#include "btcontent.h"
#include "iplist.h"

#include "btconfig.h"
#include "ctcs.h"

btTracker Tracker;

btTracker::btTracker()
{
  memset(m_host,0,MAXHOSTNAMELEN);
  memset(m_path,0,MAXPATHLEN);
  memset(m_trackerid,0,PEER_ID_LEN+1);

  m_sock = INVALID_SOCKET;
  m_port = 80;
  m_status = T_FREE;
  m_f_started = m_f_stoped = m_f_pause = m_f_completed = 0;
  m_f_softquit = m_f_restart = 0;

  m_interval = 15;

  m_connect_refuse_click = 0;
  m_last_timestamp = (time_t) 0;
  m_prevpeers = 0;
}

btTracker::~btTracker()
{
  if( m_sock != INVALID_SOCKET) CLOSE_SOCKET(m_sock);
}

void btTracker::Reset(time_t new_interval)
{
  if(new_interval) m_interval = new_interval;
  
  if( INVALID_SOCKET != m_sock ){
    CLOSE_SOCKET(m_sock);
    m_sock = INVALID_SOCKET;
  }
  
  m_reponse_buffer.Reset();
  time(&m_last_timestamp);
  if (m_f_stoped){
    m_status = T_FINISHED;
    if( m_f_restart ) Resume();
  }
  else m_status = T_FREE;
}

int btTracker:: _IPsin(char *h, int p, struct sockaddr_in *psin)
{
  psin->sin_family = AF_INET;
  psin->sin_port = htons(p);
  psin->sin_addr.s_addr = inet_addr(h);
  return ( psin->sin_addr.s_addr == INADDR_NONE ) ? -1 : 0;
}

int btTracker:: _s2sin(char *h,int p,struct sockaddr_in *psin)
{
  psin->sin_family = AF_INET;
  psin->sin_port = htons(p);
  if( h ){
    psin->sin_addr.s_addr = inet_addr(h);
    if(psin->sin_addr.s_addr == INADDR_NONE){
      struct hostent *ph = gethostbyname(h);
      if( !ph  || ph->h_addrtype != AF_INET){
        memset(psin,0,sizeof(struct sockaddr_in));
        return -1;
      }
      memcpy(&psin->sin_addr,ph->h_addr_list[0],sizeof(struct in_addr));
    }
  }else 
    psin->sin_addr.s_addr = htonl(INADDR_ANY);
  return 0;
}

int btTracker::_UpdatePeerList(char *buf,size_t bufsiz)
{
  char tmphost[MAXHOSTNAMELEN];
  const char *ps;
  size_t i,pos,tmpport;
  size_t cnt = 0;

  struct sockaddr_in addr;

  if( decode_query(buf,bufsiz,"failure reason",&ps,&i,(int64_t*) 0,QUERY_STR) ){
    char failreason[1024];
    if( i < 1024 ){
      memcpy(failreason, ps, i);
      failreason[i] = '\0';
    }else{
      memcpy(failreason, ps, 1000);
      failreason[1000] = '\0';
      strcat(failreason,"...");
    }
    char wmsg[1048];
    snprintf(wmsg,1048,"TRACKER FAILURE REASON: %s",failreason);
    warning(1, wmsg);
    return -1;
  }
  if( decode_query(buf,bufsiz,"warning message",&ps,&i,(int64_t*) 0,QUERY_STR) ){
    char warnmsg[1024];
    if( i < 1024 ){
      memcpy(warnmsg, ps, i);
      warnmsg[i] = '\0';
    }else{
      memcpy(warnmsg, ps, 1000);
      warnmsg[1000] = '\0';
      strcat(warnmsg,"...");
    }
    char wmsg[1048];
    snprintf(wmsg,1048,"TRACKER WARNING: %s",warnmsg);
    warning(2, wmsg);
  }

  m_peers_count = 0;

  if( decode_query(buf,bufsiz,"tracker id",&ps,&i,(int64_t*) 0,QUERY_STR) ){
    if( i <= PEER_ID_LEN ){
      memcpy(m_trackerid, ps, i);
      m_trackerid[i] = '\0';
    }else{
      memcpy(m_trackerid, ps, PEER_ID_LEN);
      m_trackerid[PEER_ID_LEN] = '\0';
    }
  }

  if(!decode_query(buf,bufsiz,"interval",(const char**) 0,&i,(int64_t*) 0,QUERY_INT)){return -1;}

  if(m_interval != (time_t)i) m_interval = (time_t)i;

  if(decode_query(buf,bufsiz,"complete",(const char**) 0,&i,(int64_t*) 0,QUERY_INT)) {
    m_peers_count += i;
  }
  if(decode_query(buf,bufsiz,"incomplete",(const char**) 0,&i,(int64_t*) 0,QUERY_INT)) {
    m_peers_count += i;
  }

  pos = decode_query(buf,bufsiz,"peers",(const char**) 0,(size_t *) 0,(int64_t*) 0,QUERY_POS);

  if( !pos ){
    return -1;
  }

  if(4 > bufsiz - pos){return -1; } // peers list ̫С

  buf += (pos + 1); bufsiz -= (pos + 1);

  ps = buf-1;
  if (*ps != 'l') {		// binary peers section if not 'l'
    addr.sin_family = AF_INET;
    i = 0;
    while (*ps != ':' ) i = i * 10 + (*ps++ - '0');
    i /= 6;
    ps++;
    while (i-- > 0) {
      // if peer is not us
      if(memcmp(&Self.m_sin.sin_addr,ps,sizeof(struct in_addr))) {
        memcpy(&addr.sin_addr,ps,sizeof(struct in_addr));
        memcpy(&addr.sin_port,ps+sizeof(struct in_addr),sizeof(unsigned short));
        cnt++;
        IPQUEUE.Add(&addr);
      }
    ps += 6;
    }
  }
  else
  for( ;bufsiz && *buf!='e'; buf += pos, bufsiz -= pos ){
    pos = decode_dict(buf,bufsiz,(char*) 0);
    if(!pos) break;
    if(!decode_query(buf,pos,"ip",&ps,&i,(int64_t*) 0,QUERY_STR) || MAXHOSTNAMELEN < i) continue;
    memcpy(tmphost,ps,i); tmphost[i] = '\0';

    if(!decode_query(buf,pos,"port",(const char**) 0,&tmpport,(int64_t*) 0,QUERY_INT)) continue;

    if(!decode_query(buf,pos,"peer id",&ps,&i,(int64_t*) 0,QUERY_STR) && i != 20 ) continue;

    if(_IPsin(tmphost,tmpport,&addr) < 0){
      fprintf(stderr,"warn, detected invalid ip address %s.\n",tmphost);
      continue;
    }

    if( !Self.IpEquiv(addr) ){
      cnt++;
      IPQUEUE.Add(&addr);
    }
  }
  
  if(arg_verbose)
    fprintf(stderr, "\nnew peers=%u; next check in %u sec\n", cnt, m_interval);
// moved to CheckResponse--this function isn't called if no peer data.
//  if( !cnt ) fprintf(stderr,"warn, peers list received from tracker is empty.\n");
  return 0;
}

int btTracker::CheckReponse()
{
#define MAX_LINE_SIZ 32
  char *pdata, *format;
  ssize_t r;
  size_t q, hlen, dlen;

  r = m_reponse_buffer.FeedIn(m_sock);

  if( r > 0 ) return 0;
  
  q = m_reponse_buffer.Count();

  Reset( (-1 == r) ? 15 : 0 );

  if( !q ){
    int error = 0;
    socklen_t n = sizeof(error);
    if(getsockopt(m_sock, SOL_SOCKET,SO_ERROR,&error,&n) < 0 ||
       error != 0 ){
      char wmsg[256];
      snprintf(wmsg,256,
        "warn, received nothing from tracker! %s",strerror(error));
      warning(2, wmsg);
    }
    return -1;
  }

  hlen = Http_split(m_reponse_buffer.BasePointer(), q, &pdata,&dlen);

  if( !hlen ){
    warning(2, "warn, tracker reponse invalid. No html header found.");
    return -1;
  }

  r = Http_reponse_code(m_reponse_buffer.BasePointer(),hlen);
  if ( r != 200 ){
    if( r == 301 || r == 302 ){
      char redirect[MAXPATHLEN],ih_buf[20 * 3 + 1],pi_buf[20 * 3 + 1],tmppath[MAXPATHLEN];
      if( Http_get_header(m_reponse_buffer.BasePointer(), hlen, "Location", redirect) < 0 )
        return -1;

      if( Http_url_analyse(redirect,m_host,&m_port,m_path) < 0){
        char wmsg[256];
        snprintf(wmsg,256,
          "warn, tracker redirected to an invalid url %s", redirect);
        warning(1, wmsg);
        return -1;
      }

      if(!strstr(m_path, "info_hash=")) {
        strcpy(tmppath,m_path);

        if(strchr(m_path, '?'))
          format=REQ_URL_P1A_FMT;
        else format=REQ_URL_P1_FMT;
      
        if(MAXPATHLEN < snprintf(m_path,MAXPATHLEN,format,
                                 tmppath,
                                 Http_url_encode(ih_buf, (char*)BTCONTENT.GetInfoHash(), 20),
                                 Http_url_encode(pi_buf, (char*)BTCONTENT.GetPeerId(), 20),
                                 cfg_listen_port,
                                 m_key)){
          return -1;
        }
      }

      return Connect();
    }else if( r >= 400 ){
      fprintf(stderr,"\nTracker reponse code >= 400 !!! The file is not registered on this tracker, or it may have been removed. IF YOU SEE THIS MESSAGE FOR A LONG TIME AND DOWNLOAD DOESN'T BEGIN, RECOMMEND YOU STOP NOW!!!\n");
      fprintf(stderr,"\nTracker reponse data DUMP:\n");
      if( pdata && dlen ) write(STDERR_FILENO, pdata, dlen);
      fprintf(stderr,"\n== DUMP OVER==\n");
      return -1;
    }else
      return 0;
  }

  if ( !pdata ){
    fprintf(stderr,"warn, peers list received from tracker is empty.\n");
    return 0;
  }

  if( !m_f_started ) m_f_started = 1;
  m_connect_refuse_click = 0;
  m_ok_click++;

  return _UpdatePeerList(pdata,dlen);
}

int btTracker::Initial()
{
  char ih_buf[20 * 3 + 1],pi_buf[20 * 3 + 1],tmppath[MAXPATHLEN];
  char *format;

  if(Http_url_analyse(BTCONTENT.GetAnnounce(),m_host,&m_port,m_path) < 0){
    fprintf(stderr,"error, invalid tracker url format!\n");
    return -1;
  }

  strcpy(tmppath,m_path);

  if(strchr(m_path, '?'))
    format=REQ_URL_P1A_FMT;
  else format=REQ_URL_P1_FMT;

  char chars[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for(int i=0; i<8; i++)
    m_key[i] = chars[random()%36];
  m_key[8] = 0;

  if(MAXPATHLEN < snprintf((char*)m_path,MAXPATHLEN,format,
                           tmppath,
                           Http_url_encode(ih_buf,(char*)BTCONTENT.GetInfoHash(),20),
                           Http_url_encode(pi_buf,(char*)BTCONTENT.GetPeerId(),20),
                           cfg_listen_port,
                           m_key)){
    return -1;
  }
        
  /* get local ip address */
  // 1st: if behind firewall, this only gets local side
  {
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  if(getsockname(m_sock,(struct sockaddr*)&addr,&addrlen) == 0)
        Self.SetIp(addr);
  }
  // 2nd: better to use addr of our domain
  {
          struct hostent *h;
          char hostname[128];
          char *hostdots[2]={0,0}, *hdptr=hostname;

          if (gethostname(hostname, 128) == -1) return -1;
//        printf("%s\n", hostname);
          while(*hdptr) if(*hdptr++ == '.') {
            hostdots[0] = hostdots[1];
            hostdots[1] = hdptr;
          }
          if (hostdots[0] == 0) return -1;
//        printf("%s\n", hostdots[0]);
          if ((h = gethostbyname(hostdots[0])) == NULL) return -1;
          //printf("Host domain  : %s\n", h->h_name);
          //printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *)h->h_addr)));
          memcpy(&Self.m_sin.sin_addr,h->h_addr,sizeof(struct in_addr));
  }
  return 0;
}

int btTracker::Connect()
{
  ssize_t r;
  time(&m_last_timestamp);

  if(_s2sin(m_host,m_port,&m_sin) < 0) {
    warning(2, "warn, get tracker's ip address failed.");
    return -1;
  }

  m_sock = socket(AF_INET,SOCK_STREAM,0);
  if(INVALID_SOCKET == m_sock) return -1;

  // we only need to bind if we have specified an ip
  // we need it to bind here before the connect!!!!
  if ( cfg_listen_ip != 0 ) {
    struct sockaddr_in addr;
    // clear the struct as requested in the manpages
    memset(&addr,0, sizeof(sockaddr_in));
    // set the type
    addr.sin_family = AF_INET;
    // we want the system to choose port
    addr.sin_port = 0;
    // set the defined ip from the commandline
    addr.sin_addr.s_addr = cfg_listen_ip;
    // bind it or return...
    if(bind(m_sock,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) != 0){
      fprintf(stderr, "warn, can't set up tracker connection: %s\n",
        strerror(errno));
      return -1;
    }
  }

  if(setfd_nonblock(m_sock) < 0) {CLOSE_SOCKET(m_sock); return -1; }

  r = connect_nonb(m_sock,(struct sockaddr*)&m_sin);

  if( r == -1 ){ CLOSE_SOCKET(m_sock); return -1;}
  else if( r == -2 ) m_status = T_CONNECTING;
  else{
    if( 0 == SendRequest()) m_status = T_READY;
    else{ CLOSE_SOCKET(m_sock); return -1;}
  }
  return 0;
}

int btTracker::SendRequest()
{
  char *event,*str_event[] = {"started","stopped","completed" };
  char REQ_BUFFER[MAXPATHLEN];
  socklen_t addrlen;

  struct sockaddr_in addr;
  addrlen = sizeof(struct sockaddr_in);

  /* get local ip address */
  if(getsockname(m_sock,(struct sockaddr*)&addr,&addrlen) < 0){ return -1;}

//jc  Self.SetIp(addr);
//  fprintf(stdout,"Old Set Self:");
//  fprintf(stdout,"%s\n", inet_ntoa(Self.m_sin.sin_addr));

  if( m_f_stoped )
    event = str_event[1];	/* stopped */
  else if( m_f_started == 0 ) {
    if( BTCONTENT.pBF->IsFull() ) m_f_completed = 1;
    event = str_event[0];	/* started */
  } else if( BTCONTENT.pBF->IsFull() && !m_f_completed){
    event = str_event[2];	/* download complete */
    m_f_completed = 1;		/* only send download complete once */
  } else
    event = (char*) 0;  /* interval */

  if(event){
    if(MAXPATHLEN < snprintf(REQ_BUFFER,MAXPATHLEN,REQ_URL_P2_FMT,
                             m_path,
                             Self.TotalUL(),
                             Self.TotalDL(),
                             BTCONTENT.GetLeftBytes(),
                             event,
                             cfg_max_peers,
                             m_key)){
      return -1;
    }
  }else{
    if(MAXPATHLEN < snprintf(REQ_BUFFER,MAXPATHLEN,REQ_URL_P3_FMT,
                             m_path,
                             Self.TotalUL(),
                             Self.TotalDL(),
                             BTCONTENT.GetLeftBytes(),
                             cfg_max_peers,
                             m_key)){
      return -1;
    }
  }
  if( *m_trackerid &&
      MAXPATHLEN - strlen(m_path) > 11 + strlen(m_trackerid) )
    strcat(strcat(m_path, "&trackerid="), m_trackerid);

  if(_IPsin(m_host, m_port, &addr) < 0){
    char REQ_HOST[MAXHOSTNAMELEN];
    if(MAXHOSTNAMELEN < snprintf(REQ_HOST,MAXHOSTNAMELEN,"\r\nHost: %s",m_host)) return -1;
    strcat(REQ_BUFFER, REQ_HOST);
  }
  
  strcat(REQ_BUFFER,"\r\n\r\n");
  // hc
  //fprintf(stderr,"SendRequest: %s\n", REQ_BUFFER);

  if( 0 != m_reponse_buffer.PutFlush(m_sock,REQ_BUFFER,strlen((char*)REQ_BUFFER))){
    char wmsg[256];
    snprintf(wmsg,256,
      "warn, send request to tracker failed. %s",strerror(errno));
    warning(2, wmsg);
    return -1;
  }

  return 0;
}

int btTracker::IntervalCheck(const time_t *pnow, fd_set *rfdp, fd_set *wfdp)
{
  /* tracker communication */
  if( T_FREE == m_status ){
    if( *pnow - m_last_timestamp >= m_interval ||
        // Connect to tracker early if we run low on peers.
        (WORLD.TotalPeers() < cfg_min_peers && m_prevpeers >= cfg_min_peers &&
          *pnow - m_last_timestamp >= 15) ||
        (m_f_pause && !WORLD.TotalPeers()) ){
      m_prevpeers = WORLD.TotalPeers();
   
      if(Connect() < 0){ Reset(15); return -1; }
    
      if( m_status == T_CONNECTING ){
        FD_SET(m_sock, rfdp);
        FD_SET(m_sock, wfdp);
      }else{
        FD_SET(m_sock, rfdp);
      }

      if( m_f_pause && !WORLD.TotalPeers() ) m_f_stoped = 1;
    }
  }else{
    if( m_status == T_CONNECTING ){
      FD_SET(m_sock, rfdp);
      FD_SET(m_sock, wfdp);
    }else if (INVALID_SOCKET != m_sock){
      FD_SET(m_sock, rfdp);
    }
  }
  return m_sock;
}

int btTracker::SocketReady(fd_set *rfdp, fd_set *wfdp, int *nfds)
{
  if( T_FREE == m_status ) return 0;

  if( T_CONNECTING == m_status && FD_ISSET(m_sock,wfdp) ){
    int error = 0;
    socklen_t n = sizeof(error);
    (*nfds)--;
    FD_CLR(m_sock, wfdp); 
    if(getsockopt(m_sock, SOL_SOCKET,SO_ERROR,&error,&n) < 0 ||
       error != 0 ){
      if( ECONNREFUSED != error ){
        char wmsg[256];
        snprintf(wmsg,256,
          "warn, connect to tracker failed. %s\n",strerror(error));
        warning(2, wmsg);
      }else
        m_connect_refuse_click++;
      Reset(15);
      return -1;
    }else{
      if( SendRequest() == 0 ) m_status = T_READY; 
      else { Reset(15); return -1; }
    }
  }else if( T_CONNECTING == m_status && FD_ISSET(m_sock,rfdp) ){
    int error = 0;
    socklen_t n = sizeof(error);
    (*nfds)--;
    FD_CLR(m_sock, rfdp); 
    getsockopt(m_sock, SOL_SOCKET,SO_ERROR,&error,&n);
    char wmsg[256];
    snprintf(wmsg,256,
      "warn, connect to tracker failed. %s\n",strerror(error));
    warning(2, wmsg);
    Reset(15);
    return -1;
  }else if(INVALID_SOCKET != m_sock && FD_ISSET(m_sock, rfdp) ){
    (*nfds)--;
    FD_CLR(m_sock,rfdp);
    CheckReponse();
  }
  return 0;
}

void btTracker::Resume()
{
  m_f_pause = m_f_stoped = 0;

  if( T_FINISHED == m_status ){
    m_status = T_FREE;
    m_f_started = 0;
    m_interval = 15;
  }
}

