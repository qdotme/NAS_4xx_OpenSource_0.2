#ifndef _corby_object_i_h
#define _corby_object_i_h

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

#include <corby/object.h>
#include <corby/channel.h>
#include <corby/orb.h>
#include "ior.h"


struct _sw_corby_object
{
	sw_corby_orb					m_orb;
	sw_corby_ior					m_iors;
	sw_const_corby_profile		m_profile;
	sw_corby_channel				m_channel;
	sw_socket_options				m_options;
	sw_uint32							m_bufsize;
};


sw_result
sw_corby_object_init(
					sw_corby_object	*	object);
					

#endif