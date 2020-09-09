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

#ifndef _autoip_h
#define _autoip_h


#include <salt/salt.h>
#include <salt/interface.h>
#include <salt/address.h>


#if defined(__cplusplus)
extern "C"
{
#endif


struct														_sw_autoip_network_interface;
typedef struct _sw_autoip_network_interface	*	sw_autoip_network_interface;
struct														_sw_ethernet_header;
typedef struct _sw_ethernet_header					sw_ethernet_header;	
struct														_sw_arp_header;
typedef struct _sw_arp_header							sw_arp_header;	
struct														_sw_arp_packet;
typedef struct _sw_arp_packet							sw_arp_packet;
typedef sw_uint32											sw_autoip_state;


struct _sw_autoip_network_interface
{
	sw_network_interface	m_nif;
	sw_salt					m_salt;		
	sw_autoip_state		m_state;		
	sw_mac_address			m_mac_addr;	
	sw_ipv4_address		m_ip_addr;	
	sw_timer					m_timer;		
	sw_uint32					m_conflicts;	
};


#define SW_AUTOIP_MAC_ADDRESS_SIZE	6
#define SW_AUTOIP_IP_ADDRESS_SIZE	4
#define SW_AUTOIP_ARP_REQUEST			1
#define SW_AUTOIP_ARP_REPLY			2
#define SW_AUTOIP_ETHERNET_IP			0x800
#define SW_AUTOIP_ETHERNET_ARP		0x806


/*
 * Ethernet packet header
 */
struct _sw_ethernet_header
{
	sw_uint8	m_target[SW_AUTOIP_MAC_ADDRESS_SIZE];
	sw_uint8	m_sender[SW_AUTOIP_MAC_ADDRESS_SIZE];
	sw_int16	m_ftype;
} __attribute__((__packed__));


/*
 * ARP packet
 */
struct _sw_arp_header
{
	sw_uint16 	m_hwtype;
	sw_uint16 	m_prottype;
	sw_uint8 	m_mac_size;
	sw_uint8 	m_ip_size;
	sw_uint16 	m_op;
} __attribute__((__packed__));


struct _sw_arp_packet
{
	struct _sw_ethernet_header	m_ethernet_header;
	struct _sw_arp_header		m_arp_header;
	sw_uint8							m_sender_mac[SW_AUTOIP_MAC_ADDRESS_SIZE];
	sw_uint32 						m_sender_ip;
	sw_uint8 						m_target_mac[SW_AUTOIP_MAC_ADDRESS_SIZE];
	sw_uint32							m_target_ip;
	sw_uint8							m_pad[18];
} __attribute__((__packed__));


struct _sw_autoip_compile_time_assertion_checks
{
	char assert0[(sizeof(sw_ethernet_header)	==	14	)	? 1 : -1];
	char assert1[(sizeof(sw_arp_header)			==	8	)	? 1 : -1];
	char assert2[(sizeof(sw_arp_packet)			==	60	)	? 1 : -1];
};


#define AUTOIP_STATE_IDLE				0
#define AUTOIP_STATE_SELECTING1		1
#define AUTOIP_STATE_SELECTING2		2
#define AUTOIP_STATE_SELECTING3		3
#define AUTOIP_STATE_SELECTING4		4
#define AUTOIP_STATE_ANNOUNCING1		5
#define AUTOIP_STATE_ANNOUNCING2		6
#define AUTOIP_STATE_MONITORING		8
#define AUTOIP_STATE_DEFENDING		9


sw_result
sw_autoip_network_interface_init(
					sw_autoip_network_interface		self,
					sw_salt									salt,
					sw_network_interface					nif);


sw_result
sw_autoip_network_interface_fina(
					sw_autoip_network_interface		self);


sw_result
sw_autoip_network_interface_handle_wakeup(
					sw_autoip_network_interface		self);


sw_result
sw_autoip_network_interface_read_arp_packet(
					sw_autoip_network_interface		self,
					sw_arp_packet						*	packet);


sw_string
sw_platform_autoip_network_interface_default_interface_name();


sw_result
sw_platform_autoip_network_interface_new(
					sw_autoip_network_interface	*	self,
					sw_salt									salt,
					sw_network_interface					nif);


sw_result
sw_platform_autoip_network_interface_delete(
					sw_autoip_network_interface		self);


sw_result
sw_platform_autoip_network_interface_start_monitoring(
					sw_autoip_network_interface		self);


sw_result
sw_platform_autoip_network_interface_stop_monitoring(
					sw_autoip_network_interface		self);


sw_result
sw_platform_autoip_network_interface_send_arp_packet(
					sw_autoip_network_interface		self,
					sw_arp_packet						*	packet);


sw_saddr
sw_platform_autoip_network_interface_initial_address();


sw_result
sw_platfrom_autoip_network_interface_set_ip_address(
					sw_autoip_network_interface		anif);


sw_result
sw_autoip_network_interface_restart_probing(
					sw_autoip_network_interface 		self);


#if defined(__cplusplus)
}
#endif


#endif
