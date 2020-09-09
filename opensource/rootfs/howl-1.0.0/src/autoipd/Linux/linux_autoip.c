/*
 * Copyright 2003, 2004 Porchdog Software. All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without modification,
 *	are permitted provided that the following conditions are met:
 *
 *		1. Redistributions of source code must retain the above copyright notice,
 *		   this list of conditions and the following disclaimer.   
 *		2. Redistributions in binary form must reproduce the above copyright notice,
 *		   this list of conditions and the following disclaimer in the documentation
 *		   and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY PORCHDOG SOFTWARE ``AS IS'' AND ANY
 *	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *	IN NO EVENT SHALL THE HOWL PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *	OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies,
 *	either expressed or implied, of Porchdog Software.
 */

#include "linux_autoip.h"
#include <netinet/in.h>
#include <linux/sockios.h>
#include <linux/if_ether.h>
#include <salt/socket.h>
#include <salt/address.h>
#include <salt/debug.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>


static sw_string g_default_interface_name = "eth0";


static sw_result
sw_linux_autoip_network_interface_socket_event_handler(
         								sw_socket_handler handler,
         								sw_salt           salt,
         								sw_socket         socket,
         								sw_socket_event   events,
         								sw_opaque         extra);



sw_string
sw_platform_autoip_network_interface_default_interface_name()
{
	return g_default_interface_name;
}


sw_result
sw_platform_autoip_network_interface_new(
					sw_autoip_network_interface	*	anif,
					sw_salt									salt,
					sw_network_interface					nif)
{
	sw_linux_autoip_network_interface	self;
	int											res;
	int 											macsock;
	struct ifreq								ifr;
	char 											tname[18];
	sw_result									err = SW_OKAY;

	self = (sw_linux_autoip_network_interface) sw_malloc(sizeof(struct _sw_linux_autoip_network_interface));
	sw_check(self, exit, err = SW_E_MEM);

	err = sw_autoip_network_interface_init(&self->m_super, salt, nif);
	sw_check_okay(err, exit);
	
	/*
	 * create out socket
	 */
	self->m_osock = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP));
	err = sw_translate_error(self->m_osock != -1, errno);
	sw_check_okay_log(err, exit);

	/*
	 * create in socket
	 */
	self->m_isock = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP));
	err = sw_translate_error(self->m_isock != -1, errno);
	sw_check_okay_log(err, exit);

	/*
	 * set non-blocking socket
	 */
	res = fcntl(self->m_isock, F_GETFL);
	err = sw_translate_error(res != -1, errno);
	sw_check_okay_log(err, exit);

	res = fcntl(self->m_isock, F_SETFL, res | O_NONBLOCK);
	err = sw_translate_error(res != -1, errno);
	sw_check_okay_log(err, exit);

	err = sw_tcp_socket_init_with_desc(&self->m_socket, self->m_isock);
	sw_check_okay(err, exit);

	err = sw_autoip_network_interface_handle_wakeup(&self->m_super);
	sw_check_okay(err, exit);

	*anif = &self->m_super;

exit:

	if (err && self)
	{
		sw_platform_autoip_network_interface_delete(&self->m_super);
		*anif = NULL;
	}

	return err;
}


sw_result
sw_platform_autoip_network_interface_delete(
					sw_autoip_network_interface anif)
{
	sw_linux_autoip_network_interface self = (sw_linux_autoip_network_interface) anif;

	sw_autoip_network_interface_fina(&self->m_super);

	sw_platform_autoip_network_interface_stop_monitoring(anif);

	/*
	 * free
	 */
	close(self->m_isock);
	close(self->m_osock);

	/* 
	 * and we're good
	 */
	sw_free(self);

	return SW_OKAY;
}


sw_result
sw_platform_autoip_network_interface_set_mac_address(
		const char * s)
{
	return SW_E_INIT;
}


sw_result
sw_platform_autoip_network_interface_start_monitoring(
											sw_autoip_network_interface	anif)
{
	sw_linux_autoip_network_interface self = (sw_linux_autoip_network_interface) anif;
	sw_result									err;

	err = sw_salt_register_socket(self->m_super.m_salt, self->m_socket, SW_SOCKET_READ, (sw_socket_handler) self, sw_linux_autoip_network_interface_socket_event_handler, (sw_opaque) NULL);
	sw_check_okay(err, exit);

exit:

	return err;
}


sw_result
sw_platform_autoip_network_interface_stop_monitoring(
											sw_autoip_network_interface	anif)
{
	sw_linux_autoip_network_interface self = (sw_linux_autoip_network_interface) anif;
	sw_result									err;

	err = sw_salt_unregister_socket(self->m_super.m_salt, self->m_socket);
	sw_check_okay(err, exit);

exit:

	return err;
}

											

sw_result
sw_platform_autoip_network_interface_send_arp_packet(
											sw_autoip_network_interface	anif, 
											sw_arp_packet					*	packet)
{
	sw_linux_autoip_network_interface	self = (sw_linux_autoip_network_interface) anif;
	struct sockaddr 							sndarp;
	char											name[IFNAMSIZ];
	sw_mac_address								mymac;
	int											res;
	sw_result									err = SW_OKAY;

	/*
	 * get interface name
	 */
	sw_network_interface_name(self->m_super.m_nif, name, IFNAMSIZ);
	strcpy(sndarp.sa_data, name);

	/*
	 * send packet
	 */
	res = sendto(self->m_osock, packet, sizeof(struct _sw_arp_packet), 0, &sndarp, sizeof(sndarp));
	err = sw_translate_error(res >= 0, errno);
	sw_check_okay_log(err, exit);

exit:

	return err;
}


sw_result
sw_platform_autoip_network_interface_set_ip_address(
											sw_autoip_network_interface	anif)
{
	sw_linux_autoip_network_interface	self = (sw_linux_autoip_network_interface) anif;
   int            r;
   int            sock_fd;
   struct ifreq   ifr;
   struct in_addr saddr;
   char           name[IFNAMSIZ];
	int				res;
   sw_result      err = SW_OKAY;

   /*
	 * socket for ioctl
	 */
	sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	err = sw_translate_error(sock_fd != -1, errno);
	sw_check_okay_log(err, exit);

	/*
	 * initialize the device name
	 */
   sw_memset(&ifr, 0, sizeof(ifr));
   sw_network_interface_name(self->m_super.m_nif, name, IFNAMSIZ);
   strncpy(ifr.ifr_name, name, IFNAMSIZ - 1);
                                                          
	/*
	 * initialize the ip address
	 */
   saddr.s_addr = sw_ipv4_address_saddr(self->m_super.m_ip_addr);
   ifr.ifr_addr.sa_family = AF_INET;
   ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr = saddr;
   r = ioctl(sock_fd, SIOCSIFADDR, &ifr);
	err = sw_translate_error(r == 0, errno);
	sw_check_okay_log(err, exit);

	err = sw_network_interface_set_ipv4_address(self->m_super.m_nif, self->m_super.m_ip_addr);
	sw_check_okay(err, exit);

exit:

	if (sock_fd != -1)
	{
	   close(sock_fd);
	}
}


static sw_result
sw_linux_autoip_network_interface_socket_event_handler(
         								sw_socket_handler handler,
         								sw_salt           salt,
         								sw_socket         socket,
         								sw_socket_event   events,
         								sw_opaque         extra)
{
	static sw_mac_address	ProbeSenderMAC = { 0, 0, 0, 0, 0, 0 };

	sw_linux_autoip_network_interface self = (sw_linux_autoip_network_interface) handler;
   sw_arp_packet    			packet; 
	char 							smyip[18], spacketip[18];
	char 							smymac[18], spacketmac[18];
	int							bytes;
	sw_result					err = SW_OKAY;
	
   sw_assert(self != NULL);

	/*
	 * ignore invocations if not our socket
	 */
	sw_check(self->m_isock == sw_socket_desc(socket), exit, err = SW_OKAY);

	sw_memset(&packet, 0, sizeof(struct _sw_arp_packet));
	bytes = read(self->m_isock, &packet, sizeof(struct _sw_arp_packet));
	err = sw_translate_error(bytes >= 0, errno);
	sw_check_okay_log(err, exit);

	if (bytes >= (sizeof(struct _sw_ethernet_header) + sizeof(struct _sw_arp_header)))
	{
		if (((ntohs(packet.m_arp_header.m_op)) == SW_AUTOIP_ARP_REQUEST) || ((ntohs(packet.m_arp_header.m_op)) == SW_AUTOIP_ARP_REPLY))
		{
			err = sw_autoip_network_interface_read_arp_packet(&self->m_super, &packet);
			sw_check_okay(err, exit);
		}
	}

exit:

	return err;
}
