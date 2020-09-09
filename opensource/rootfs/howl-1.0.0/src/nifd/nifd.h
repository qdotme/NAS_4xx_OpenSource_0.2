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

#include <salt/salt.h>
#include <salt/address.h>
#include <salt/interface.h>
#include <salt/signal.h>

/* nifd 
 * 	Polls all interfaces to determine if any interface has changed its
 *		IP address.	Notifies autoipd and mDNSResponderd (by sending SIGUSR1) 
 *		any time an IP address changes.
 *		Polls all interfaces to determine if the network link is lost or found.  
 *		Notifies autoipd and mDNSResponderd (by sending SIGUSR2) any time
 *		a link is lost or found.
 *		Any time a link is lost on an interface, nifd calls /sbin/ifdown on 
 *		that interface.  Any time a new link is established on an interface, 
 *		nifd calls /sbin/ifup on that interface.
 */
typedef struct _nifd
{
	sw_salt						m_salt;		
	int							m_pidfd;
	sw_timer						m_timer;
	sw_time						m_polltime;
	sw_signal					m_sigusr1;
	sw_signal					m_sigusr2;
	sw_signal					m_sighup;
	sw_signal					m_sigint;
	sw_uint32						m_nifcount;
	sw_network_interface *	m_nifs;
} nifd;

sw_result
nifd_init(nifd * self, sw_uint32 interval);

sw_result
nifd_fina(nifd * self);
