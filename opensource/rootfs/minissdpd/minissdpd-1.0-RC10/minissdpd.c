/* $Id: minissdpd.c,v 1.1 2007/11/14 08:25:54 wiley Exp $ */
/* MiniUPnP project
 * (c) 2007 Thomas Bernard
 * website : http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>
#include <ctype.h>
#include <time.h>
#include <sys/queue.h>
/* for chmod : */
#include <sys/stat.h>
/* unix sockets */
#include <sys/un.h>

/*#include "minissdp.h"*/
#include "openssdpsocket.h"
#include "daemonize.h"

/* current request management stucture */
struct reqelem {
	int socket;
	LIST_ENTRY(reqelem) entries;
};

/* divice data structures */
struct header {
	const char * p;
	int l;
};

#define HEADER_NT	0
#define HEADER_USN	1
#define HEADER_LOCATION	2

struct device {
	struct device * next;
	time_t t;
	struct header headers[3];
	char data[];
};

#define NTS_SSDP_ALIVE	1
#define NTS_SSDP_BYEBYE	2

/* discovered device list kept in memory */
struct device * devlist = 0;

/* updateDevice() :
 * adds or updates the device to the list.
 * return value :
 *   0 : the device was updated
 *   1 : the device was new    */
int updateDevice(const struct header * headers, time_t t)
{
	struct device ** pp = &devlist;
	struct device * p = *pp;	/* = devlist; */
	while(p)
	{
		if(  p->headers[HEADER_NT].l == headers[HEADER_NT].l
		  && (0==memcmp(p->headers[HEADER_NT].p, headers[HEADER_NT].p, headers[HEADER_NT].l))
		  && p->headers[HEADER_USN].l == headers[HEADER_USN].l
		  && (0==memcmp(p->headers[HEADER_USN].p, headers[HEADER_USN].p, headers[HEADER_USN].l)) )
		{
			//printf("found! %d\n", (int)(t - p->t));
			syslog(LOG_DEBUG, "device updated : %.*s", headers[HEADER_USN].l, headers[HEADER_USN].p);
			p->t = t;
			/* update Location ! */
			if(headers[HEADER_LOCATION].l > p->headers[HEADER_LOCATION].l)
			{
				p = realloc(p, sizeof(struct device)
		           + headers[0].l+headers[1].l+headers[2].l );
				if(!p)	/* allocation error */
					return 0;
				*pp = p;
			}
			memcpy(p->data + p->headers[0].l + p->headers[1].l,
			       headers[2].p, headers[2].l);
			return 0;
		}
		pp = &p->next;
		p = *pp;	/* p = p->next; */
	}
	syslog(LOG_INFO, "new device discovered : %.*s", headers[HEADER_USN].l, headers[HEADER_USN].p);
	/* add */
	{
		char * pc;
		int i;
		p = malloc(  sizeof(struct device)
		           + headers[0].l+headers[1].l+headers[2].l );
		p->next = devlist;
		p->t = t;
		pc = p->data;
		for(i = 0; i < 3; i++)
		{
			p->headers[i].p = pc;
			p->headers[i].l = headers[i].l;
			memcpy(pc, headers[i].p, headers[i].l);
			pc += headers[i].l;
		}
		devlist = p;
	}
	return 1;
}

/* removeDevice() :
 * remove a device from the list
 * return value :
 *    0 : no device removed
 *   -1 : device removed */
int removeDevice(const struct header * headers)
{
	struct device ** pp = &devlist;
	struct device * p = *pp;	/* = devlist */
	while(p)
	{
		if(  p->headers[HEADER_NT].l == headers[HEADER_NT].l
		  && (0==memcmp(p->headers[HEADER_NT].p, headers[HEADER_NT].p, headers[HEADER_NT].l))
		  && p->headers[HEADER_USN].l == headers[HEADER_USN].l
		  && (0==memcmp(p->headers[HEADER_USN].p, headers[HEADER_USN].p, headers[HEADER_USN].l)) )
		{
			syslog(LOG_INFO, "remove device : %.*s", headers[HEADER_USN].l, headers[HEADER_USN].p);
			*pp = p->next;
			free(p);
			return -1;
		}
		pp = &p->next;
		p = *pp;	/* p = p->next; */
	}
	syslog(LOG_WARNING, "device not fount for removing : %.*s", headers[HEADER_USN].l, headers[HEADER_USN].p);
	return 0;
}

/* ParseSSDPPacket() :
 * parse a received SSDP Packet and call 
 * updateDevice() or removeDevice() as needed
 * return value :
 *    -1 : a device was removed
 *     0 : no device removed nor added
 *     1 : a device was added.  */
int ParseSSDPPacket(const char * p, ssize_t n)
{
	const char * linestart;
	const char * lineend;
	const char * nameend;
	const char * valuestart;
	struct header headers[3];
	int i, r = 0;
	int methodlen;
	int nts = -1;
	unsigned int lifetime = 180;	/* 3 minutes by default */
	memset(headers, 0, sizeof(headers));
	for(methodlen = 0; methodlen < n && isalpha(p[methodlen]); methodlen++);
	/*printf("method: '%.*s'\n", methodlen, p);*/
	linestart = p;
	while(linestart < p + n - 2) {
		/* start parsing the line : detect line end */
		lineend = linestart;
		while(*lineend != '\n' && *lineend != '\r' && lineend < p + n)
			lineend++;
		//printf("line: '%.*s'\n", lineend - linestart, linestart);
		/* detect name end : ':' character */
		nameend = linestart;
		while(*nameend != ':' && nameend < lineend)
			nameend++;
		/* detect value */
		valuestart = nameend + 1;
		while(isspace(*valuestart) && valuestart < lineend)
			valuestart++;
		/* suppress leading " if needed */
		if(*valuestart=='\"')
			valuestart++;
		if(nameend > linestart && valuestart < lineend) {
			int l = nameend - linestart;
			int m = lineend - valuestart;
			/* suppress tailing spaces */
			while(m>0 && isspace(valuestart[m-1]))
				m--;
			/* suppress tailing ' if needed */
			if(valuestart[m-1] == '\"')
				m--;
			i = -1;
			/*printf("--%.*s: (%d)%.*s--\n", l, linestart,
			                           m, m, valuestart);*/
			if(l==2 && 0==strncasecmp(linestart, "nt", 2))
				i = HEADER_NT;
			else if(l==3 && 0==strncasecmp(linestart, "usn", 3))
				i = HEADER_USN;
			else if(l==3 && 0==strncasecmp(linestart, "nts", 3)) {
				if(m==10 && 0==strncasecmp(valuestart, "ssdp:alive", 10))
					nts = NTS_SSDP_ALIVE;
				else if(m==11 && 0==strncasecmp(valuestart, "ssdp:byebye", 11))
					nts = NTS_SSDP_BYEBYE;
			}
			else if(l==8 && 0==strncasecmp(linestart, "location", 8))
				i = HEADER_LOCATION;
			else if(l==13 && 0==strncasecmp(linestart, "cache-control", 13)) {
				const char * name = valuestart;
				const char * val;
				int rem = m;
				while(rem > 0) {
					val = name;
					while(*val != '=' && *val != ',')
						val++;
					if(*val == '=') {
						while(*val == '=' || isspace(*val))
							val++;
						if(0==strncasecmp(name, "max-age", 7))
							lifetime = (unsigned int)strtoul(val, 0, 0);
						while(rem > 0 && *name != ',') {
							rem--;
							name++;
						}
						while(rem > 0 && (*name == ',' || isspace(*name))) {
							rem--;
							name++;
						}
					} else {
						rem -= (val - name);
						name = val;
						while(rem > 0 && (*name == ',' || isspace(*name))) {
							rem--;
							name++;
						}
					}
				}
				/*syslog(LOG_DEBUG, "**%.*s**%u", m, valuestart, lifetime);*/
			}
			if(i>=0) {
				headers[i].p = valuestart;
				headers[i].l = m;
			}
		}
		linestart = lineend;
		while((*linestart == '\n' || *linestart == '\r') && linestart < p + n)
			linestart++;
	}
#if 0
	printf("NTS=%d\n", nts);
	for(i=0; i<3; i++) {
		if(headers[i].p)
			printf("%d-'%.*s'\n", i, headers[i].l, headers[i].p);
	}
#endif
	if(headers[0].p && headers[1].p && headers[2].p) {
		if(nts==NTS_SSDP_ALIVE) {
			r = updateDevice(headers, time(NULL) + lifetime);
		}
		else if(nts==NTS_SSDP_BYEBYE) {
			r = removeDevice(headers);
		}
	}
	return r;
}

/* OpenUnixSocket()
 * open the unix socket and call bind() and listen()
 * return -1 in case of error */
int
OpenUnixSocket(const char * path)
{
	struct sockaddr_un addr;
	int s;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	if(s < 0)
	{
		syslog(LOG_ERR, "socket(AF_UNIX): %m");
		return -1;
	}
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path, sizeof(addr.sun_path));
	if(bind(s, (struct sockaddr *)&addr,
	           sizeof(struct sockaddr_un)) < 0)
	{
		syslog(LOG_ERR, "bind(unixsocket, \"%s\"): %m", path);
		close(s);
		return -1;
	}
	else if(listen(s, 5) < 0)
	{
		syslog(LOG_ERR, "listen(unixsocket): %m");
		close(s);
		return -1;
	}
	if(chmod(path, 0666) < 0)
	{
		syslog(LOG_WARNING, "chmod(\"%s\"): %m", path);
	}
	return s;
}

#define DECODELENGTH(n, p) n = 0; \
                           do { n = (n << 7) | (*p & 0x7f); p++; } \
                           while(*p&0x80);
#define CODELENGTH(n, p) do { *p = (n & 0x7f) | ((n > 0x7f) ? 0x80 : 0); \
                              p++; n >>= 7; } while(n);
/* processRequest() :
 * process the request coming from a unix socket */
void processRequest(struct reqelem * req)
{
	ssize_t n;
	unsigned int l, m;
	unsigned char buf[512];
	const unsigned char * p;
	int type;
	struct device * d = devlist;
	unsigned char rbuf[512];
	unsigned char * rp = rbuf+1;
	unsigned char nrep = 0;
	time_t t;

	n = read(req->socket, buf, sizeof(buf));
	if(n<0)
	{
		syslog(LOG_ERR, "processRequest(): read(): %m");
		close(req->socket);
		req->socket = -1;
		return;
	}
	if(n==0)
	{
		syslog(LOG_INFO, "request connection closed");
		close(req->socket);
		req->socket = -1;
		return;
	}
	t = time(NULL);
	type = buf[0];
	p = buf + 1;
	DECODELENGTH(l, p);
	syslog(LOG_INFO, "request type=%d str='%.*s'", type, l, p);
	while(d && (nrep < 255))
	{
		if(d->t < t) {
			syslog(LOG_INFO, "outdated device");
		} else {
			switch(type)
			{
			case 1:
				//printf(" %.*s\n", d->headers[HEADER_NT].l, d->headers[HEADER_NT].p);
				//printf(" %.*s\n", l, d->headers[HEADER_NT].p);
				if(0==memcmp(d->headers[HEADER_NT].p, p, l))
				{
					/*printf("Ok\n");
					printf(" %.*s\n", d->headers[HEADER_NT].l, d->headers[HEADER_NT].p);
					printf(" %.*s\n", d->headers[HEADER_USN].l, d->headers[HEADER_USN].p);
					printf(" %.*s\n", d->headers[HEADER_LOCATION].l, d->headers[HEADER_LOCATION].p);*/
					m = d->headers[HEADER_LOCATION].l;
					CODELENGTH(m, rp);
					memcpy(rp, d->headers[HEADER_LOCATION].p, d->headers[HEADER_LOCATION].l);
					rp += d->headers[HEADER_LOCATION].l;
					m = d->headers[HEADER_NT].l;
					CODELENGTH(m, rp);
					memcpy(rp, d->headers[HEADER_NT].p, d->headers[HEADER_NT].l);
					rp += d->headers[HEADER_NT].l;
					nrep++;
				}
				break;
			}
		}
		d = d->next;
	}
	rbuf[0] = nrep;
	if(write(req->socket, rbuf, rp - rbuf) < 0)
		syslog(LOG_ERR, "write: %m");
}

static volatile int quitting = 0;
/* SIGTERM signal handler */
static void
sigterm(int sig)
{
	signal(sig, SIG_IGN);
	syslog(LOG_NOTICE, "received signal %d, good-bye", sig);
	quitting = 1;
}

/* main(): program entry point */
int main(int argc, char * * argv)
{
	int pid;
	struct sigaction sa;
	char buf[1500];
	ssize_t n;
	int s_ssdp;	/* udp socket receiving ssdp packets */
	int s_unix;	/* unix socket communicating with clients */
	int s;
	LIST_HEAD(reqstructhead, reqelem) reqlisthead;
	struct reqelem * req;
	struct reqelem * reqnext;
	fd_set readfds;
	const char * if_addr = NULL;
	int i;
	const char * sockpath = "/var/run/minissdpd.sock";
	const char * pidfilename = "/var/run/minissdpd.pid";
	int debug_flag = 0;
	int deltadev = 0;

	LIST_INIT(&reqlisthead);
	/* process command line */
	for(i=1; i<argc; i++)
	{
		if(0==strcmp(argv[i], "-i"))
			if_addr = argv[++i];
		else if(0==strcmp(argv[i], "-d"))
			debug_flag = 1;
	}
	if(!if_addr)
	{
		fprintf(stderr, "Usage: %s [-d] -i <interface_address>\n", argv[0]);
		return 1;
	}

	/* daemonize or in any case get pid ! */
	if(debug_flag)
		pid = getpid();
	else
		pid = daemonize();
	/* open log */
	openlog("minissdpd",
	        LOG_CONS|LOG_PID|(debug_flag?LOG_PERROR:0),
			LOG_DAEMON);
	if(!debug_flag) /* speed things up and ignore LOG_INFO and LOG_DEBUG */
		setlogmask(LOG_UPTO(LOG_NOTICE));
	
	if(checkforrunning(pidfilename) < 0)
	{
		syslog(LOG_ERR, "MiniSSDPd is already running. EXITING");
		return 1;
	}

	writepidfile(pidfilename, pid);

	/* set signal handlers */
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigterm;
	if(sigaction(SIGTERM, &sa, NULL))
	{
		syslog(LOG_ERR, "Failed to set SIGTERM handler. EXITING");
		return 1;
	}
	if(sigaction(SIGINT, &sa, NULL))
	{
		syslog(LOG_ERR, "Failed to set SIGINT handler. EXITING");
		return 1;
	}
	s_ssdp = OpenAndConfSSDPReceiveSocket(if_addr, 0, 0);
	if(s_ssdp < 0)
	{
		syslog(LOG_ERR, "Cannot open socket for receiving SSDP messages, exiting");
		return 1;
	}
	s_unix = OpenUnixSocket(sockpath);
	if(s_unix < 0)
	{
		syslog(LOG_ERR, "Cannot open unix socket for communicating with clients. Exiting");
		return 1;
	}

	/* Main loop */
	while(!quitting)
	{
		/* fill readfds fd_set */
		FD_ZERO(&readfds);
		FD_SET(s_ssdp, &readfds);
		FD_SET(s_unix, &readfds);
		for(req = reqlisthead.lh_first; req; req = req->entries.le_next)
		{
			if(req->socket >= 0)
				FD_SET(req->socket, &readfds);
		}
		/* select call */
		if(select(FD_SETSIZE, &readfds, 0, 0, 0) < 0)
		{
			if(errno != EINTR)
				syslog(LOG_ERR, "select: %m");
			break;
		}
		if(FD_ISSET(s_ssdp, &readfds))
		{
			n = recvfrom(s_ssdp, buf, sizeof(buf), 0, 0, 0);
			if(n<0)
			{
				syslog(LOG_ERR, "recvfrom: %m");
			}
			else
			{
				/*printf("%.*s", n, buf);*/
				i = ParseSSDPPacket(buf, n);
				syslog(LOG_DEBUG, "** i=%d deltadev=%d **", i, deltadev);
				if(i==0 || (i*deltadev < 0))
				{
					if(deltadev > 0)
						syslog(LOG_NOTICE, "%d new devices added", deltadev);
					else if(deltadev < 0)
						syslog(LOG_NOTICE, "%d devices removed (good-bye!)", -deltadev);
					deltadev = i;
				}
				else if((i*deltadev) >= 0)
				{
					deltadev += i;
				}
			}
		}
		for(req = reqlisthead.lh_first; req;)
		{
			reqnext = req->entries.le_next;
			if((req->socket >= 0) && FD_ISSET(req->socket, &readfds))
			{
				processRequest(req);
			}
			if(req->socket < 0)
			{
				LIST_REMOVE(req, entries);
				free(req);
			}
			req = reqnext;
		}
		if(FD_ISSET(s_unix, &readfds))
		{
			struct reqelem * tmp;
			s = accept(s_unix, NULL, NULL);
			if(s<0)
			{
				syslog(LOG_ERR, "accept(s_unix): %m");
			}
			else
			{
				syslog(LOG_INFO, "new request connection");
				tmp = malloc(sizeof(struct reqelem));
				tmp->socket = s;
				LIST_INSERT_HEAD(&reqlisthead, tmp, entries);
			}
		}
	}
	/* closing and cleaning everything */
	close(s_ssdp);
	close(s_unix);
	if(unlink(sockpath) < 0)
		syslog(LOG_ERR, "unlink(%s): %m", sockpath);
	if(unlink(pidfilename) < 0)
		syslog(LOG_ERR, "unlink(%s): %m", pidfilename);
	closelog();
	return 0;
}

