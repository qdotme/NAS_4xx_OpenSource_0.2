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

#include "autoip.h"
#include <salt/interface.h>
#include <salt/signal.h>
#include <salt/debug.h>
#include <stdio.h>

#define SW_ERROR -1
#define CHANGE_ADDRESS

#if defined(BIG_ENDIAN)						/* this is not network order */
#	define IP_BYTE1(ul) (ul & 0xFF)
#	define IP_BYTE2(ul) (ul >> 8)
#	define IP_BYTE3(ul) (ul >> 16)
#	define IP_BYTE4(ul) (ul >> 24)
#else
#	define IP_BYTE1(ul) (ul >> 24)
#	define IP_BYTE2(ul) (ul >> 16)
#	define IP_BYTE3(ul) (ul >> 8)
#	define IP_BYTE4(ul) (ul & 0xFF)
#endif


static sw_uint8
g_ethernet_broadcast_tag[SW_AUTOIP_MAC_ADDRESS_SIZE] =
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};


/*
 * static function
 */
static sw_result
sw_autoip_network_interface_send_arp_probe(
								sw_autoip_network_interface self);


static sw_result
sw_autoip_network_interface_send_arp_announcement(
								sw_autoip_network_interface self);


/* 
 * Callback functions 
 */
static sw_result
sw_autoip_network_interface_timer_event_handler(
								sw_timer_handler     handler,
								sw_salt           	salt,
								sw_timer          	timer,
								sw_time              timeout,
								sw_opaque            extra);


/* other declarations */
static sw_bool
is_routable_address(sw_ipv4_address	ipaddr);


static sw_bool
is_no_address(sw_ipv4_address	ipaddr);


static sw_result
sw_autoip_network_interface_register_timeout(
								sw_autoip_network_interface	self,
								sw_time								timeout);


/* 
 * autoip_generate_random_llip()
 * 	generate a legal random Link-Local IPv4 address (value must be 
 * 	between 169.254.1.0 and 169.254.254.255)
 */
static sw_result
sw_autoip_network_interface_generate_random_llip(
								sw_ipv4_address * addr)
{
	static char name[16];
	sw_saddr oaddr, saddr;
	sw_uint32 rnum;
	sw_uint8 b3, b4;
	sw_uint8 ob3, ob4;
	sw_result err;

	sw_assert(addr != NULL);

	saddr = sw_ipv4_address_saddr(*addr);
	ob3 = IP_BYTE3(saddr);
	ob4 = IP_BYTE4(saddr); 

	do 
	{
		rnum = rand();
		b3 = IP_BYTE1(rnum);
		b4 = IP_BYTE2(rnum);
	} while ((b3 == 0) || (b3 == 255) || ((b3 == ob3) && (b4 == ob4)));

	sprintf(name, "169.254.%hhu.%hhu", b3, b4);
	sw_debug(SW_LOG_VERBOSE, "autoip_generate_random_llip() generated the address: %s\n", name);

	err = sw_ipv4_address_init_from_name(addr, name);
	sw_check_okay(err, exit);

exit:
	return err;
}


/*
 * seed the random number generator from the mac
 */
static sw_result
sw_autoip_network_interface_make_initial_ip_address(
								sw_autoip_network_interface	self)
{
	sw_saddr		ipaddr;
	sw_uint32	seed;
	sw_uint8 * 	pseed;
	int			i;
	sw_result	err;

	ipaddr = sw_platform_autoip_network_interface_initial_address();

	pseed = (sw_uint8*)&seed;
	for (i = 0; i < 4; i++)
	{
		(*pseed++) = self->m_mac_addr.m_id[i+2];
	}
	srand(seed);

	/* initialize the ip address */
	if (ipaddr != 0)
	{
		char 			name[16];
		sw_uint8 	b3 = ((sw_uint8*)(&ipaddr))[2];
		sw_uint8 	b4 = ((sw_uint8*)(&ipaddr))[3];

		sprintf(name, "169.254.%hhu.%hhu", b3, b4);

		sw_debug(SW_LOG_VERBOSE, "assigning ip address %s\n", name);

		err = sw_ipv4_address_init_from_name(&(self->m_ip_addr), name); 
		sw_check_okay(err, exit);
	}
	else				/* seed the ip address from the mac */
	{
		sw_debug(SW_LOG_VERBOSE, "generating random ip address from the mac\n");

		sw_ipv4_address_init_from_saddr(&(self->m_ip_addr), seed);

		/*
		 * generate a random initial ip
		 */
		err = sw_autoip_network_interface_generate_random_llip(&(self->m_ip_addr));
		sw_check_okay(err, exit);
	}

exit:

	return err;
}


sw_result
sw_autoip_network_interface_init(
						sw_autoip_network_interface	self, 
						sw_salt								salt,
						sw_network_interface				nif)
{
	sw_time		timeout;
	sw_result	err;

	sw_assert(self);
	sw_assert(salt);
	sw_assert(nif);

	self->m_salt		=	salt;
	self->m_nif			=	nif;
	self->m_state		=	AUTOIP_STATE_IDLE;
	self->m_timer		=	NULL;
	self->m_conflicts =	0;

	sw_debug(SW_LOG_VERBOSE, "IDLE\n");

	/*
	 * initialize the MAC address
	 */
	err = sw_network_interface_mac_address(self->m_nif, &self->m_mac_addr);
	sw_check_okay(err, exit);

	/*
	 * initialize the ip address
	 */
	err = sw_autoip_network_interface_make_initial_ip_address(self);
	sw_check_okay(err, exit);

	/*
	 * initialize the timer
	 */
	err = sw_timer_init(&self->m_timer);
	sw_check_okay(err, exit);

	/*
	 * wake up
	 */
	timeout.m_secs		= 10;
	timeout.m_usecs	= 0;
	sw_autoip_network_interface_register_timeout(self, timeout);

exit:

	return err;
}


/*
 *	make_random_onesec_timeout()
 * 	makes a random timeout spec 0 <= timeout <= 1sec before probing
 */
static void
sw_autoip_network_interface_make_random_onesec_timeout(
							sw_autoip_network_interface	self,
							sw_time						*	timeout)
{
	sw_uint32 rnum;

	sw_assert(timeout != NULL);

#ifdef APPLE_CONFORMANCE_TEST_ALIGNS_WITH_STANDARD
	rnum					= rand();
	timeout->m_secs	= 0;
	timeout->m_usecs	= (rnum & 0xFFFFF);	/* lower 20 bits is about 1M usecs */
#else
	timeout->m_secs	= 0;
	timeout->m_usecs	= 500000;
#endif
}


static sw_result
sw_autoip_network_interface_register_timeout(
							sw_autoip_network_interface	self,
							sw_time							timeout)
{
	return sw_salt_register_timer(self->m_salt, self->m_timer, timeout, (sw_timer_handler) self, sw_autoip_network_interface_timer_event_handler, NULL);
}



static sw_result
sw_autoip_network_interface_unregister_timeout(
							sw_autoip_network_interface	self)
{
	return sw_salt_unregister_timer(self->m_salt, self->m_timer);
}



/* 
 * autoip_free_auto_ip()
 *
 *	do any unbind or what have you to relinquish resources 
 */
sw_result
sw_autoip_network_interface_fina(
							sw_autoip_network_interface	self)
{
	sw_result err = SW_OKAY;
	
	/*
	 * free timer
	 */
	if (self->m_timer != NULL)
	{
		err = sw_timer_fina(self->m_timer);
		sw_check_okay(err, exit);
	}

exit:

	return err;
}


/*
 * timer_event_handler()
 * 	called where the main loop detects that a timeout has occurred.
 */
static sw_result
sw_autoip_network_interface_timer_event_handler(
         				sw_timer_handler  handler,
         				sw_salt           salt,
         				sw_timer       	timer,
         				sw_time           timeout,
         				sw_opaque         extra)
{
	sw_autoip_network_interface	self;
	sw_time								new_timeout;
	sw_result							err = SW_OKAY;

	self = (sw_autoip_network_interface) handler;

   sw_assert(self->m_timer == timer);

	switch (self->m_state)
	{
		case AUTOIP_STATE_SELECTING1:
		{
			sw_autoip_network_interface_make_random_onesec_timeout(self, &new_timeout);
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			/*
			 * send our first probe
			 */
			err = sw_autoip_network_interface_send_arp_probe(self);
			sw_check_okay(err, exit);

			sw_debug(SW_LOG_VERBOSE, "SELECTING2\n");
			self->m_state = AUTOIP_STATE_SELECTING2;
		}
		break;

		case AUTOIP_STATE_SELECTING2:
		{
			sw_autoip_network_interface_make_random_onesec_timeout(self, &new_timeout);
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			/*
			 * send our second probex
			 */
			err = sw_autoip_network_interface_send_arp_probe(self);
			sw_check_okay(err, exit);

			sw_debug(SW_LOG_VERBOSE, "SELECTING3\n");
			self->m_state = AUTOIP_STATE_SELECTING3;
		}
		break;

		case AUTOIP_STATE_SELECTING3:
		{
			sw_autoip_network_interface_make_random_onesec_timeout(self, &new_timeout); 
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			/*
			 * send our third probex
			 */
			err = sw_autoip_network_interface_send_arp_probe(self);
			sw_check_okay(err, exit);

			sw_debug(SW_LOG_VERBOSE, "SELECTING4\n");
			self->m_state = AUTOIP_STATE_SELECTING4;
		}
		break;

		case AUTOIP_STATE_SELECTING4:
		{
			/*
			 * wait two seconds to claim
			 */
			new_timeout.m_secs 	= 1;
			new_timeout.m_usecs 	= 0;
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			/*
			 * send our fourth probe -- the conformance test seems to need this
			 */
			err = sw_autoip_network_interface_send_arp_probe(self);
			sw_check_okay(err, exit);

			sw_debug(SW_LOG_VERBOSE, "ANNOUNCING1\n");
			self->m_state = AUTOIP_STATE_ANNOUNCING1;
		}
		break;

		case AUTOIP_STATE_ANNOUNCING1:
		{
			/*
			 * wait two seconds between announcements
			 */
			new_timeout.m_secs 	= 1;
			new_timeout.m_usecs 	= 0;
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			/*
			 * send our first announcement
			 */
			err = sw_autoip_network_interface_send_arp_announcement(self);
			sw_check_okay(err, exit);
			//err = sw_autoip_network_interface_send_arp_announcement(self);
			//sw_check_okay(err, exit);

#ifdef CHANGE_ADDRESS
			/*
			 * change the interface's ip addressx
			 */
			err = sw_platform_autoip_network_interface_set_ip_address(self);
			sw_check_okay(err, exit);
#endif

			sw_debug(SW_LOG_VERBOSE, "ANNOUNCING2\n");
			self->m_state = AUTOIP_STATE_ANNOUNCING2;
		}
		break;

		case AUTOIP_STATE_ANNOUNCING2:
		{
			/*
			 * let's move quickly on to claiming
			 */
			new_timeout.m_secs 	= 2;
			new_timeout.m_usecs 	= 0;
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			/*
			 * send our second announcementx
			 */
			// SW_TRY(sw_autoip_network_interface_send_arp_announcement(self));
			err = sw_autoip_network_interface_send_arp_announcement(self);
			sw_check_okay(err, exit);

			//sw_debug(SW_LOG_VERBOSE, "MONITORING\n");
			self->m_state = AUTOIP_STATE_MONITORING;
		}
		break;

		case AUTOIP_STATE_MONITORING:
		{
			/*
			 * we monitor at an arbitrary 1 minute interval
			 */
			new_timeout.m_secs 	= 60;
			new_timeout.m_usecs 	= 0;
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);
			sw_debug(SW_LOG_VERBOSE, "MONITORING\n");
		}
		break;

		case AUTOIP_STATE_DEFENDING:
		{
			/*
			 * we've successfully defended the addressx
			 */
			new_timeout.m_secs 	= 60;
			new_timeout.m_usecs 	= 0;
			err = sw_autoip_network_interface_register_timeout(self, new_timeout);
			sw_check_okay(err, exit);

			sw_debug(SW_LOG_VERBOSE, "MONITORING\n");
			self->m_state = AUTOIP_STATE_MONITORING;
		}
		break;

		case AUTOIP_STATE_IDLE:
		{
			sw_assert(SW_FALSE);
		}
		break;

		default:
		{
			sw_assert(SW_FALSE);
		}
		break;
	}

exit:

	return err;
}


static sw_result
sw_autoip_network_interface_flee(
							sw_autoip_network_interface	self)
{
	sw_time		timeout;
	sw_result	err;

	/*
	 * unregister the extant timeout
	 */
	sw_autoip_network_interface_unregister_timeout(self);

	/*
	 * back off to 10 seconds if we're getting too many conflicts
	 */
	if (self->m_conflicts < 10)
	{
		sw_autoip_network_interface_make_random_onesec_timeout(self, &timeout);
	}
	else
	{
		timeout.m_secs 	= 10;
		timeout.m_usecs 	= 0;
	}

	/*
	 * register the new timeoutx
	 */
	err = sw_autoip_network_interface_register_timeout(self, timeout);
	sw_check_okay(err, exit);

	/*
	 * generate the new ip
	 */
	err = sw_autoip_network_interface_generate_random_llip(&(self->m_ip_addr));
	sw_check_okay(err, exit);

	/*
	 * reset the state
	 */
	self->m_state = AUTOIP_STATE_SELECTING1;

exit:

	return err;
}


static sw_result
sw_autoip_network_interface_defend(
							sw_autoip_network_interface self)
{
	sw_time		timeout;
	sw_result	err;

	/*
	 * unregister the extant timeout
	 */
	sw_autoip_network_interface_unregister_timeout(self);

	/*
	 * monitor for 10 seconds on defense
	 */
	timeout.m_secs 	= 10;
	timeout.m_usecs 	= 0;
	err = sw_autoip_network_interface_register_timeout(self, timeout);
	sw_check_okay(err, exit);

	/*
	 * send our defense announcement
	 */
	err = sw_autoip_network_interface_send_arp_announcement(self);
	sw_check_okay(err, exit);

	/*
	 * reset our state
	 */
	self->m_state = AUTOIP_STATE_DEFENDING;

exit:

	return err;
}


/*
 * socket_event_handler()
 * 	called where the main loop detects that data is available on a socket.
 */
sw_result
sw_autoip_network_interface_read_arp_packet(
					sw_autoip_network_interface	self,
					sw_arp_packet					*	packet)
{
	sw_ipv4_address	source_addr;
	sw_ipv4_address	target_addr;
	char					sbuf[16];
	char					tbuf[16];

	sw_assert(sw_memcmp(packet->m_ethernet_header.m_sender, packet->m_sender_mac, 6) == 0);

	sw_ipv4_address_init_from_saddr(&source_addr, packet->m_sender_ip);
	sw_ipv4_address_init_from_saddr(&target_addr, packet->m_target_ip);

	sw_debug(SW_LOG_VERBOSE, "sw_autoip_network_interface_read_arp_packet", "read arp packet - source mac = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x, source ip = %s, target mac (ethernet header) = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x, target mac (arp packet) = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x, target = %s\n", packet->m_sender_mac[0], packet->m_sender_mac[1], packet->m_sender_mac[2], packet->m_sender_mac[3], packet->m_sender_mac[4], packet->m_sender_mac[5], sw_ipv4_address_name(source_addr, sbuf, 16), packet->m_ethernet_header.m_target[0], packet->m_ethernet_header.m_target[1], packet->m_ethernet_header.m_target[2], packet->m_ethernet_header.m_target[3],packet->m_ethernet_header.m_target[4], packet->m_ethernet_header.m_target[5], packet->m_target_mac[0], packet->m_target_mac[1], packet->m_target_mac[2], packet->m_target_mac[3], packet->m_target_mac[4], packet->m_target_mac[5], sw_ipv4_address_name(target_addr, tbuf, 16));

	/* 
	 * Check for collisions 
	 */
	if (self->m_state < AUTOIP_STATE_ANNOUNCING2)
	{
		/*
		 * is someone already using this address?
		 * or
		 * is someone else probing for this address?
		 */
		if ((packet->m_sender_ip == sw_ipv4_address_saddr(self->m_ip_addr)) ||
			((packet->m_target_ip == sw_ipv4_address_saddr(self->m_ip_addr)) && (sw_memcmp(packet->m_sender_mac, self->m_mac_addr.m_id, 6) != 0)))
		{
			/*
			 * flee
			 */
			self->m_conflicts++;

			sw_autoip_network_interface_flee(self);
		}
	}
	else
	{
		/*
		 * we've claimed the address
		 */
		if ((packet->m_sender_ip == sw_ipv4_address_saddr(self->m_ip_addr)) &&
		    (sw_memcmp(packet->m_sender_mac, self->m_mac_addr.m_id, 6) != 0)) 
		{
			self->m_conflicts++;

			/*
			 * Can we defend it?
			 */
			if (self->m_state != AUTOIP_STATE_DEFENDING) 
			{
				/*
				 * fight
				 */
				sw_autoip_network_interface_defend(self);
			}
			else
			{
				sw_autoip_network_interface_flee(self);
			}
		}
	}

   return SW_OKAY;
}


static sw_result
sw_autoip_network_interface_monitor(
							sw_autoip_network_interface self)
{
	sw_time timeout;

	/*
	 * reset the state
	 */
	self->m_state = AUTOIP_STATE_MONITORING;

	/*
	 * reset the timeout
	 */
	sw_autoip_network_interface_unregister_timeout(self);
	timeout.m_secs		= 60;
	timeout.m_usecs	= 0;
	sw_autoip_network_interface_register_timeout(self, timeout);

	return SW_OKAY;
}


static sw_result
sw_autoip_network_interface_idle(
							sw_autoip_network_interface self)
{
	sw_result err = SW_OKAY;

	if (self->m_state != AUTOIP_STATE_IDLE)
	{
		/*
		 * unregister socket
		 */
		err = sw_platform_autoip_network_interface_stop_monitoring(self);
		sw_check_okay(err, exit);
	}

	/*
	 * unregister timeout
	 */
	sw_autoip_network_interface_unregister_timeout(self);

	/*
	 * reset the state
	 */
	sw_debug(SW_LOG_VERBOSE, "IDLE\n");
	self->m_state = AUTOIP_STATE_IDLE;

exit:

	return err;
}


sw_result
sw_autoip_network_interface_restart_probing(
							sw_autoip_network_interface self)
{
	sw_time		timeout;
	sw_result	err = SW_OKAY;

   sw_assert(self != NULL);

	sw_debug(SW_LOG_VERBOSE, "restarting probing\n");

	if (self->m_state == AUTOIP_STATE_IDLE)
	{
		/*
		 * re-register socket
		 */
		err = sw_platform_autoip_network_interface_start_monitoring(self);
		sw_check_okay(err, exit);
	}

	/*
	 * initialize the conflicts counter
	 */
	self->m_conflicts = 0;

	/*
	 * reset the state
	 */
	sw_debug(SW_LOG_VERBOSE, "SELECTING1\n");
	self->m_state = AUTOIP_STATE_SELECTING1;

	/*
	 * reset the timeout
	 */
	sw_autoip_network_interface_unregister_timeout(self);
	sw_autoip_network_interface_make_random_onesec_timeout(self, &timeout);
	sw_autoip_network_interface_register_timeout(self, timeout);

exit:

	return err;
}


static sw_bool
is_no_address(sw_ipv4_address	ipaddr)
{
	return (sw_ipv4_address_saddr(ipaddr) == 0) ? SW_TRUE : SW_FALSE;
}


static sw_bool
is_routable_address(sw_ipv4_address	ipaddr)
{
	sw_int8	name[18];

	sw_ipv4_address_name(ipaddr, name, 18);
	name[7] = (char)0;

	return (sw_strcmp(name, "169.254") == 0) ? SW_FALSE : SW_TRUE;
}


sw_result
sw_autoip_network_interface_handle_wakeup(
							sw_autoip_network_interface self)
{
	sw_ipv4_address		ipnow;
	sw_bool					linkednow;
	char						name[32];
	sw_result				err = SW_OKAY;

	sw_assert(self != NULL);

	/*
	 * make sure we're awake
	 */
	err = sw_network_interface_up(self->m_nif);
	sw_check_okay(err, exit);

	/*
	 * get current interface status
	 */
	err = sw_network_interface_linked(self->m_nif, &linkednow);
	sw_check_okay(err, exit);

	err = sw_network_interface_ipv4_address(self->m_nif, &ipnow);
	sw_check_okay(err, exit);

	sw_ipv4_address_name(ipnow, name, 32);

	/*
	 * we have a link
	 */
	if (linkednow)	
	{
		sw_debug(SW_LOG_VERBOSE, "linked\n");
//		if (is_no_address(ipnow))
		if (1)
		{
			err = sw_autoip_network_interface_restart_probing(self);
			sw_check_okay(err, exit);
		}
		else if (is_routable_address(ipnow))
		{
			err = sw_autoip_network_interface_idle(self);
			sw_check_okay(err, exit);
		}
		/*
		 * link-local address
		 */
		else
		{
			if (self->m_state == AUTOIP_STATE_IDLE)
			{
				err = sw_autoip_network_interface_restart_probing(self);
				sw_check_okay(err, exit);
			}
		}
	}
	else
	{
		/*
	 	 * we lost our linkx
	 	 */
		sw_debug(SW_LOG_VERBOSE, "not linked\n");
		err = sw_autoip_network_interface_idle(self);
		sw_check_okay(err, exit);
	}

exit:

	return err;
}


static sw_result
sw_autoip_network_interface_send_arp_probe(
								sw_autoip_network_interface self)
{
	sw_arp_packet	packet;
	sw_saddr			targetip = sw_ipv4_address_saddr(self->m_ip_addr);
	sw_int8			name[16];
	sw_mac_address	mac;
	
	sw_debug(SW_LOG_VERBOSE, "sw_autoip_network_interface_send_arp_probe", "probing for %s\n", sw_ipv4_address_name(self->m_ip_addr, name, 16)); 

	/*
	 * initialize
	 */
	sw_memset(&packet, 0, sizeof(struct _sw_arp_packet));

	/*	
	 * fill in the Ethernet header
	 */
	packet.m_ethernet_header.m_ftype = htons(SW_AUTOIP_ETHERNET_ARP);
	sw_memcpy(&packet.m_ethernet_header.m_target, g_ethernet_broadcast_tag, SW_AUTOIP_MAC_ADDRESS_SIZE);
	sw_network_interface_mac_address(self->m_nif, &mac);
	sw_memcpy(&packet.m_ethernet_header.m_sender, mac.m_id, SW_AUTOIP_MAC_ADDRESS_SIZE);

	/*
	 * format the arp message
	 */
	packet.m_arp_header.m_hwtype		=	htons(1);
	packet.m_arp_header.m_prottype	=	htons(SW_AUTOIP_ETHERNET_IP);
	packet.m_arp_header.m_mac_size  	=	SW_AUTOIP_MAC_ADDRESS_SIZE;
	packet.m_arp_header.m_ip_size 	=	SW_AUTOIP_IP_ADDRESS_SIZE;
	packet.m_arp_header.m_op			=	htons(SW_AUTOIP_ARP_REQUEST);
	sw_memcpy(packet.m_sender_mac, mac.m_id, SW_AUTOIP_MAC_ADDRESS_SIZE);
	sw_memset(&packet.m_sender_ip, 0, SW_AUTOIP_IP_ADDRESS_SIZE);
	sw_memset(packet.m_target_mac, 0, SW_AUTOIP_MAC_ADDRESS_SIZE);
	sw_memcpy(&packet.m_target_ip, (sw_uint8*)&targetip, SW_AUTOIP_IP_ADDRESS_SIZE);

	return sw_platform_autoip_network_interface_send_arp_packet(self, &packet);
}


static sw_result
sw_autoip_network_interface_send_arp_announcement(
								sw_autoip_network_interface	self)
{
	sw_arp_packet	packet;
	sw_saddr			targetip = sw_ipv4_address_saddr(self->m_ip_addr);
	sw_int8			name[16];
	sw_mac_address	mac;

	sw_debug(SW_LOG_VERBOSE, "sending arp announcement for %s\n", sw_ipv4_address_name(self->m_ip_addr, name, 16));

	/*
	 * initialize packet
	 */
	sw_memset(&packet, 0, sizeof(struct _sw_arp_packet));

	/*
	 * fill in the Ethernet header
	 */
	packet.m_ethernet_header.m_ftype = htons(SW_AUTOIP_ETHERNET_ARP);
	sw_memcpy(&packet.m_ethernet_header.m_target, g_ethernet_broadcast_tag, SW_AUTOIP_MAC_ADDRESS_SIZE);
	sw_network_interface_mac_address(self->m_nif, &mac);
	sw_memcpy(&packet.m_ethernet_header.m_sender, mac.m_id, SW_AUTOIP_MAC_ADDRESS_SIZE);

	/*
	 * format the arp message
	 */
	packet.m_arp_header.m_hwtype		=	htons(1);
	packet.m_arp_header.m_prottype	=	htons(SW_AUTOIP_ETHERNET_IP);
	packet.m_arp_header.m_mac_size  	=	SW_AUTOIP_MAC_ADDRESS_SIZE;
	packet.m_arp_header.m_ip_size 	=	SW_AUTOIP_IP_ADDRESS_SIZE;
	packet.m_arp_header.m_op			=	htons(SW_AUTOIP_ARP_REQUEST);
	sw_memcpy(packet.m_sender_mac, mac.m_id, SW_AUTOIP_MAC_ADDRESS_SIZE);
	sw_memcpy(&packet.m_sender_ip, (sw_uint8*)&targetip, SW_AUTOIP_IP_ADDRESS_SIZE);
	sw_memset(packet.m_target_mac, 0, SW_AUTOIP_MAC_ADDRESS_SIZE);
	sw_memcpy(&packet.m_target_ip, (sw_uint8*)&targetip, SW_AUTOIP_IP_ADDRESS_SIZE);

	return sw_platform_autoip_network_interface_send_arp_packet(self, &packet);
}
