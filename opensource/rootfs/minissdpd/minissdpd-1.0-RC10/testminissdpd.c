/* $Id: testminissdpd.c,v 1.1 2007/11/14 08:25:54 wiley Exp $ */
/* Project : miniupnp
 * website : http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * Author : Thomas BERNARD
 * copyright (c) 2005-2007 Thomas Bernard
 * This software is subjet to the conditions detailed in the
 * provided LICENCE file. */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


/* test program for minissdpd */
int
main(int argc, char * * argv)
{
	char command[] = "\x01\x00urn:schemas-upnp-org:device:InternetGatewayDevice";
	struct sockaddr_un addr;
	int s;
	int l;
	int i;
	unsigned char buf[512];
	const unsigned char * p;
	ssize_t n;
	const char * sockpath = "/var/run/minissdpd.sock";

	command[1] = sizeof(command) - 3;
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sockpath, sizeof(addr.sun_path));
	if(connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("Connected.\n");
	write(s, command, sizeof(command));
	printf("Command written\n");
	n = read(s, buf, sizeof(buf));
	printf("Response received %d bytes\n", (int)n);
	for(i=0; i<n; i++)
		printf("%02x ", buf[i]);
	printf("\n");
	p = buf + 1;
	for(i = 0; i < buf[0]; i++)
	{
		l = *(p++);
		printf("%d - %.*s\n", i, l, p);
		p += l;
		l = *(p++);
		printf("    %.*s\n", l, p);
		p += l;
	}
	return 0;
}

