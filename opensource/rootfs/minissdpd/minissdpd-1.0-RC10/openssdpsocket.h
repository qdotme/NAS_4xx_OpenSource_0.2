#ifndef __OPENSSDPSOCKET_H__
#define __OPENSSDPSOCKET_H__

int
OpenAndConfSSDPReceiveSocket(const char * ifaddr,
                             int n_add_listen_addr,
							 const char * * add_listen_addr);

#endif

