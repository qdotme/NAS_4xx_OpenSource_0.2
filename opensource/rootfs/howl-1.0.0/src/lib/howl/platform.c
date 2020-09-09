
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

#include <salt/platform.h>
#include <salt/salt.h>
#include <salt/debug.h>


sw_string
sw_strdup(sw_const_string str)
{
	sw_string	ret = NULL;
	sw_result	err;
	
	if (str == NULL)
	{
		return NULL;
	}
	
	ret = malloc(strlen(str) + 1);
	sw_check(ret, exit, err = SW_E_MEM);

	sw_strcpy(ret, str);

exit:

	return ret;
}


#if defined(__VXWORKS__)

const sw_uint8
sw_int8_tmap[] =
{
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
	'\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
	'\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
	'\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
	'\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
	'\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
	'\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
	'\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
	'\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
	'\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
	'\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};


sw_int32
sw_strcasecmp(
			sw_const_string	s1,
			sw_const_string	s2)
{
   register const sw_uint8 * cm	= sw_int8_tmap;
   register const sw_uint8 * us1	= (const sw_uint8*) s1;
   register const sw_uint8 * us2	= (const sw_uint8*) s2;

	while (cm[*us1] == cm[*us2++])
	{
		if (*us1++ == '\0')
		{
			return 0;
		}
	}

	return (cm[*us1] - cm[*--us2]);
}


sw_int32
sw_strncasecmp(
				const char	*	s1,
				const char	*	s2,
				sw_len			n)
{
	if (n != 0)
	{
	 register const sw_uint8 *cm		= sw_int8_tmap;
	 register const sw_uint8 *us1	= (sw_uint8*)s1;
	 register const sw_uint8	*us2	= (sw_uint8*)s2;

		do
		{
			if (cm[*us1] != cm[*us2++])
			{
				return (cm[*us1] - cm[*--us2]);
			}

			if (*us1++ == '\0')
			{
				break;
			}
		}
		while (--n != 0);
	}

	return 0;
}

#endif


#if defined(WIN32) || defined(__VXWORKS__) || defined(__PALMOS__)


sw_string
sw_strtok_r(
			sw_string			s1,
			sw_const_string	s2,
			sw_string		*	lasts)
{
 char * ret = NULL;
 char * p   = s1 ? s1 : *lasts; 

	if (s2) 
	{ 
		while (*p && strchr(s2, *p)) { p++; }
		ret = p;
		while (*p && !strchr(s2, *p)) { p++; }
		if (*p) { *p++ = '\0'; }
		*lasts = p;
	}

	if (ret != NULL)
	{
		if (*ret == '\0') ret = NULL;
	}

	return ret;
}


#endif
