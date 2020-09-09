/*
Copyright (C) 2003-2004 Narcis Ilisei
Modifications by Steve Horbachuk
Copyright (C) 2006 Steve Horbachuk
Modifications by Bryan Hoover (bhoover@wecs.com)
Copyright (C) 2007 Bryan Hoover (bhoover@wecs.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/*
	Dyn Dns update main implementation file 
	Author: narcis Ilisei
	Date: May 2003

	History:
        - first implemetnation
        - 18 May 2003 : cmd line option reading added - 
        - Nov 2003 - new version
        - April 2004 - freedns.afraid.org system added.
        - October 2004 - Unix syslog capability added.
        - October 2007 - win32 RAS events trap thread, RAS, 
          and network online status checking added.  Debug 
          level command line parameter added.  Refactored,
          augmented main loop, including moving one time
          initialization outside of loop.  Two files
          changed -- dyndns.c, inadyn_cmd.c.  Two files
          added -- event_trap.c, event_trap.h.
        - November 2007 - multithread safe debug log.  One file
		  changed -- os.c.
        - December 2007 - Windows service routines, parser default 
          command, and registry, config file, command line command
          options hierachy.  Parser error handling callback.  
          Added files service.c, service.h, service_main.c,
          service_main.h, debug_service.h.  Changed  inadyn_cmd.c, 
        - get_cmd.c, os_windows.c, main.c.
*/
#define MODULE_TAG      "INADYN: "  

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "dyndns.h"
#include "debug_if.h"
#include "base64.h"
#include "get_cmd.h"
#include "unicode_util.h"
#include "os.h"
#include "safe_mem.h"
#include "lang.h"

#ifdef _WIN32

#include "debug_service.h"
#include "unicode_util.h"

static HANDLE   hUpdateMutex=NULL;
static BOOL     returnSignaled=false;

DWORD get_mutex(DYN_DNS_CLIENT *p_dyndns,HANDLE *hMutex,int *is_waited);
DWORD get_mutex_wait(DYN_DNS_CLIENT *p_dyndns,HANDLE *hMutex);
int release_mutex(DYN_DNS_CLIENT *p_dyndns,HANDLE *hMutex);
static void atomic_inc(DYN_DNS_CLIENT *p_self,int *src,int inc,HANDLE hMutex);

#endif

/* DNS systems specific configurations*/

DYNDNS_ORG_SPECIFIC_DATA dyndns_org_dynamic = {"dyndns"};
DYNDNS_ORG_SPECIFIC_DATA dyndns_org_custom = {"custom"};
DYNDNS_ORG_SPECIFIC_DATA dyndns_org_static = {"statdns"};

static int get_req_for_dyndns_server(DYN_DNS_CLIENT *this, int nr, DYNDNS_SYSTEM *p_sys_info);
static int get_req_for_freedns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info);
static int get_req_for_generic_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info);
static int get_req_for_noip_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info);
static int get_req_for_easydns_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info);
static int get_req_for_sitelutions_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info);

static BOOL is_dyndns_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string);
static BOOL is_freedns_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string);
static BOOL is_generic_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string);
static BOOL is_zoneedit_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string);
static BOOL is_easydns_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string);
static BOOL is_sitelutions_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string);

DYNDNS_SYSTEM_INFO dns_system_table[] = 
{ 
    {DYNDNS_DEFAULT, 
        {"default@dyndns.org", &dyndns_org_dynamic, 
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_dyndns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC)get_req_for_dyndns_server,
             DYNDNS_MY_IP_SERVER, DYNDNS_MY_IP_SERVER_URL,
			DYNDNS_MY_DNS_SERVER, DYNDNS_MY_DNS_SERVER_URL, NULL}},
    {DYNDNS_DYNAMIC, 
        {"dyndns@dyndns.org", &dyndns_org_dynamic,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_dyndns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_dyndns_server,
             DYNDNS_MY_IP_SERVER, DYNDNS_MY_IP_SERVER_URL,
			DYNDNS_MY_DNS_SERVER, DYNDNS_MY_DNS_SERVER_URL, NULL}},    
    {DYNDNS_CUSTOM, 
        {"custom@dyndns.org", &dyndns_org_custom,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_dyndns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_dyndns_server,
             DYNDNS_MY_IP_SERVER, DYNDNS_MY_IP_SERVER_URL,
			DYNDNS_MY_DNS_SERVER, DYNDNS_MY_DNS_SERVER_URL, NULL}},
    {DYNDNS_STATIC, 
        {"statdns@dyndns.org", &dyndns_org_static,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_dyndns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_dyndns_server,
             DYNDNS_MY_IP_SERVER, DYNDNS_MY_IP_SERVER_URL,
				DYNDNS_MY_DNS_SERVER, DYNDNS_MY_DNS_SERVER_URL, NULL}},

    {FREEDNS_AFRAID_ORG_DEFAULT, 
        {"default@freedns.afraid.org", NULL,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_freedns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_freedns_server,
            DYNDNS_MY_IP_SERVER, DYNDNS_MY_IP_SERVER_URL,
			"freedns.afraid.org", "/dynamic/update.php?", NULL}},

    {ZONE_EDIT_DEFAULT, 
        {"default@zoneedit.com", NULL,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_zoneedit_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_generic_http_dns_server,
            "dynamic.zoneedit.com", "/checkip.html", 
			"dynamic.zoneedit.com", "/auth/dynamic.html?host=", ""}},

    {NOIP_DEFAULT, 
        {"default@no-ip.com", NULL,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_dyndns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_noip_http_dns_server,
            "ip1.dynupdate.no-ip.com", "/", 
			"dynupdate.no-ip.com", "/nic/update?hostname=", ""}},

    {EASYDNS_DEFAULT, 
        {"default@easydns.com", NULL,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_easydns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_easydns_http_dns_server,
            DYNDNS_MY_IP_SERVER, DYNDNS_MY_IP_SERVER_URL, 
			"members.easydns.com", "/dyn/dyndns.php?hostname=", ""}},

    {DYNDNS_3322_DYNAMIC, 
        {"dyndns@3322.org", &dyndns_org_dynamic,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_dyndns_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_dyndns_server,
             DYNDNS_3322_MY_IP_SERVER, DYNDNS_3322_MY_IP_SERVER_URL,
			DYNDNS_3322_MY_DNS_SERVER, DYNDNS_3322_MY_DNS_SERVER_URL, NULL}},

    {SITELUTIONS_DOMAIN, 
        {"default@sitelutions.com", NULL,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_sitelutions_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_sitelutions_http_dns_server,
             NULL, NULL,"www.sitelutions.com", "/dnsup?", NULL}},


    {CUSTOM_HTTP_BASIC_AUTH, 
        {"custom@http_svr_basic_auth", NULL,  
            (DNS_SYSTEM_SRV_RESPONSE_OK_FUNC)is_generic_server_rsp_ok, 
            (DNS_SYSTEM_REQUEST_FUNC) get_req_for_generic_http_dns_server,
            GENERIC_DNS_IP_SERVER_NAME, DYNDNS_MY_IP_SERVER_URL,
			"", "", "OK"}},

    {LAST_DNS_SYSTEM, {NULL, NULL, NULL, NULL, NULL, NULL}}
};

static DYNDNS_SYSTEM* get_dns_system_by_id(DYNDNS_SYSTEM_ID id)
{
    {
        DYNDNS_SYSTEM_INFO *it;
        for (it = dns_system_table; it->id != LAST_DNS_SYSTEM; ++it)
        {
            if (it->id == id)
            {
                return &it->system;
            }
        }    
    } 
    return NULL;
}

DYNDNS_SYSTEM_INFO* get_dyndns_system_table(void)
{
    return dns_system_table;
}

/*************PRIVATE FUNCTIONS ******************/
static RC_TYPE dyn_dns_wait_for_cmd(DYN_DNS_CLIENT *p_self,void **hUpdateMutex)
{
	int counter = p_self->sleep_sec / p_self->cmd_check_period;
	int counter_ref = counter;
	int	cmd_check_period_ms = p_self->cmd_check_period * 1000;
	DYN_DNS_CMD old_cmd = p_self->cmd;
	
#ifdef _WIN32

	HANDLE	*hMutex=NULL;

	if (*hUpdateMutex)

		hMutex=(HANDLE *) hUpdateMutex;
#endif

	
	if (old_cmd != NO_CMD)
	{
		return RC_OK;
	}

	while(counter --)
	{

#ifndef _WIN32

        if (p_self->cmd != old_cmd) {
#else
		if (p_self->cmd != old_cmd || returnSignaled) {

#endif
			counter_ref=counter;

			/**nix signal to update, or exiting*/
			if (!(p_self->cmd == CMD_UPDTED))

				break;

			p_self->cmd = NO_CMD;
		}

		os_sleep_ms(cmd_check_period_ms);
	}

#ifndef _WIN32

	p_self->time_sleep_lost_ms=(counter_ref-counter)*cmd_check_period_ms;
#else

	get_mutex_wait(p_self,hMutex);


	if (!(p_self->cmd == CMD_UPDTED)) /*ras update happened after exited timer loop*/

		p_self->time_sleep_lost_ms=(counter_ref-counter)*cmd_check_period_ms;


	if (returnSignaled)

		p_self->cmd=CMD_STOP;


	release_mutex(p_self,hMutex);

#endif


	return RC_OK;
}

static int get_req_for_dyndns_server(DYN_DNS_CLIENT *p_self, int cnt,DYNDNS_SYSTEM *p_sys_info)
{	

	int bytes_stored=0;


    DYNDNS_ORG_SPECIFIC_DATA *p_dyndns_specific = 
		(DYNDNS_ORG_SPECIFIC_DATA*) p_sys_info->p_specific_data;	

	memset(p_self->p_req_buffer,0,DYNDNS_HTTP_RESPONSE_BUFFER_SIZE);


	bytes_stored=sprintf(p_self->p_req_buffer, DYNDNS_GET_MY_IP_HTTP_REQUEST_FORMAT,


		p_self->info.dyndns_server_url,
		p_dyndns_specific->p_system,
		p_self->alias_info.names[cnt].name,
		p_self->info.my_ip_address.name,
                p_self->wildcard ? "ON" : "OFF",
		p_self->alias_info.names[cnt].name,
        p_self->info.dyndns_server_name.name,
		p_self->info.credentials.p_enc_usr_passwd_buffer
		);


	return bytes_stored;
}

static int get_req_for_freedns_server(DYN_DNS_CLIENT *p_self, int cnt, DYNDNS_SYSTEM *p_sys_info)
{
	(void)p_sys_info;
	return sprintf(p_self->p_req_buffer, FREEDNS_UPDATE_MY_IP_REQUEST_FORMAT,
		p_self->info.dyndns_server_url,
		p_self->alias_info.hashes[cnt].str,
        p_self->info.dyndns_server_name.name);
}


static int get_req_for_generic_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt, DYNDNS_SYSTEM *p_sys_info)
{
	(void)p_sys_info;
	return sprintf(p_self->p_req_buffer, GENERIC_DNS_BASIC_AUTH_MY_IP_REQUEST_FORMAT,
		p_self->info.dyndns_server_url,		
		p_self->alias_info.names[cnt].name,
        p_self->info.credentials.p_enc_usr_passwd_buffer,
		p_self->info.dyndns_server_name.name);
}
static int get_req_for_noip_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info)
{
	(void)p_sys_info;
	return sprintf(p_self->p_req_buffer, GENERIC_NOIP_AUTH_MY_IP_REQUEST_FORMAT,
		p_self->info.dyndns_server_url,		
		p_self->alias_info.names[cnt].name,
		p_self->info.my_ip_address.name,
        p_self->info.credentials.p_enc_usr_passwd_buffer,
		p_self->info.dyndns_server_name.name		
		);
}
static int get_req_for_easydns_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info)
{
	(void)p_sys_info;
	return sprintf(p_self->p_req_buffer, GENERIC_EASYDNS_AUTH_MY_IP_REQUEST_FORMAT,
		p_self->info.dyndns_server_url,		
		p_self->alias_info.names[cnt].name,
		p_self->info.my_ip_address.name,
                p_self->wildcard ? "ON" : "OFF",
        p_self->info.credentials.p_enc_usr_passwd_buffer,
		p_self->info.dyndns_server_name.name		
		);
}
static int get_req_for_sitelutions_http_dns_server(DYN_DNS_CLIENT *p_self, int cnt,  DYNDNS_SYSTEM *p_sys_info)
{

	(void)p_sys_info;
	return sprintf(p_self->p_req_buffer, SITELUTIONS_GET_MY_IP_HTTP_REQUEST_FORMAT,
		p_self->info.dyndns_server_url,	
		p_self->info.credentials.my_username,
		p_self->info.credentials.my_password,
		p_self->alias_info.names[cnt].name,
		p_self->info.dyndns_server_name.name		
		);
}

static int get_req_for_ip_server(DYN_DNS_CLIENT *p_self, void *p_specific_data)
{
    return sprintf(p_self->p_req_buffer, DYNDNS_GET_MY_IP_HTTP_REQUEST,
        p_self->info.ip_server_name.name, p_self->info.ip_server_url);
}

/* 
	Send req to IP server and get the response
*/
static RC_TYPE do_ip_server_transaction(DYN_DNS_CLIENT *p_self)
{
	RC_TYPE rc = RC_OK;
	HTTP_CLIENT *p_http;

	p_http = &p_self->http_to_ip_server;

	rc = http_client_init(&p_self->http_to_ip_server);
	if (rc != RC_OK)
	{
		return rc;
	}

	do
	{
		/*prepare request for IP server*/
		{
			HTTP_TRANSACTION *p_tr = &p_self->http_tr;

            p_tr->req_len = get_req_for_ip_server((DYN_DNS_CLIENT*) p_self,
                                                     p_self->info.p_dns_system->p_specific_data);
			if (p_self->dbg.level > 2) 
			{
				DBG_PRINTF((LOG_DEBUG,"The request for IP server:\n%s\n",p_self->p_req_buffer));
			}
            p_tr->p_req = (char*) p_self->p_req_buffer;		
			p_tr->p_rsp = (char*) p_self->p_work_buffer;
			p_tr->max_rsp_len = p_self->work_buffer_size - 1;/*save place for a \0 at the end*/
			p_tr->rsp_len = 0;

			rc = http_client_transaction(&p_self->http_to_ip_server, &p_self->http_tr);		
			p_self->p_work_buffer[p_tr->rsp_len] = 0;
		}
	}
	while(0);

	/*close*/
	http_client_shutdown(&p_self->http_to_ip_server);
	
	return rc;
}


/* 
	Read in 4 integers the ip addr numbers.
	construct then the IP address from those numbers.
    Note:
        it updates the flag: info->'my_ip_has_changed' if the old address was different 
*/
static RC_TYPE do_parse_my_ip_address(DYN_DNS_CLIENT *p_self)
{
	int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
	int count;
	char *p_ip;
	char *p_current_str = p_self->http_tr.p_rsp;
	BOOL found;
    char new_ip_str[IP_V4_MAX_LENGTH];

	if (p_self->http_tr.rsp_len <= 0 || 
		p_self->http_tr.p_rsp == NULL)
	{
		return RC_INVALID_POINTER;
	}

	found = FALSE;
	do
	{
		/*try to find first decimal number (begin of IP)*/
		p_ip = strpbrk(p_current_str, DYNDNS_ALL_DIGITS);
		if (p_ip != NULL)
		{
			/*maybe I found it*/
			count = sscanf(p_ip, DYNDNS_IP_ADDR_FORMAT,
							&ip1, &ip2, &ip3, &ip4);
			if (count != 4 ||
				ip1 <= 0 || ip1 > 255 ||
				ip2 < 0 || ip2 > 255 ||
				ip3 < 0 || ip3 > 255 ||
				ip4 < 0 || ip4 > 255 )
			{
				p_current_str = p_ip + 1;
			}
			else
			{
				/* FIRST occurence of a valid IP found*/
				found = TRUE;
				break;
			}
		}
	}
	while(p_ip != NULL);
		   

	if (found)
	{        
        sprintf(new_ip_str, DYNDNS_IP_ADDR_FORMAT, ip1, ip2, ip3, ip4);
        p_self->info.my_ip_has_changed = (strcmp(new_ip_str, p_self->info.my_ip_address.name) != 0);
		strcpy(p_self->info.my_ip_address.name, new_ip_str);
		return RC_OK;
	}
	else
	{
		return RC_DYNDNS_INVALID_RSP_FROM_IP_SERVER;
	}	
}

/*
    Updates for every maintained name the property: 'update_required'.
    The property will be checked in another function and updates performed.
        
      Action:
        Check if my IP address has changed. -> ALL names have to be updated.
        Nothing else.
        Note: In the update function the property will set to false if update was successful.
*/
static RC_TYPE do_check_alias_update_table(DYN_DNS_CLIENT *p_self)
{
	int		i;
	int		time_slept=p_self->times_since_last_update * p_self->sleep_sec - ((int)
						((p_self->time_sleep_lost_ms / 1000)));


    if (p_self->info.my_ip_has_changed ||
        p_self->force_addr_update || (time_slept >= p_self->forced_update_period_sec)
       )
    {
        for (i = 0; i < p_self->alias_info.count; ++i)
	    {
            p_self->alias_info.update_required[i] = TRUE;
			{
				DBG_PRINTF((LOG_WARNING,"I:" MODULE_TAG "IP address for alias '%s' needs update to '%s'\n",
					p_self->alias_info.names[i].name,
					p_self->info.my_ip_address.name ));
			}
        }
    }
    return RC_OK;
}

/* DynDNS org.specific response validator.
    'good' or 'nochange' are the good answers,
*/
static BOOL is_dyndns_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string)
{
	(void) p_ok_string;
    return ( (strstr(p_rsp, DYNDNS_OK_RESPONSE) != NULL) ||
             (strstr(p_rsp, DYNDNS_OK_NOCHANGE) != NULL) );
}

/* Freedns afraid.org.specific response validator.
    ok blabla and n.n.n.n
    fail blabla and n.n.n.n
    are the good answers. We search our own IP address in response and that's enough.
*/
static BOOL is_freedns_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string)
{
	(void) p_ok_string;
    return (strstr(p_rsp, p_self->info.my_ip_address.name) != NULL);
}

/** generic http dns server ok parser 
	parses a given string. If found is ok,
	Example : 'SUCCESS CODE='
*/
static BOOL is_generic_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string)
{
	if (p_ok_string == NULL)
	{
		return FALSE;
	}
    return (strstr(p_rsp, p_ok_string) != NULL);
}

/**
	the OK codes are:
		CODE=200
		CODE=707, for duplicated updates
*/
BOOL is_zoneedit_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string)
{
	return 
	(		
		(strstr(p_rsp, "CODE=\"200\"") != NULL) ||
		(strstr(p_rsp, "CODE=\"707\"") != NULL)
	);	
}

/**
	NOERROR is the OK code here
*/
BOOL is_easydns_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string)
{
	return (strstr(p_rsp, "NOERROR") != NULL);
}

static BOOL is_sitelutions_server_rsp_ok( DYN_DNS_CLIENT *p_self, char*p_rsp, char* p_ok_string)
{

	return (strstr(p_rsp,"success") != NULL);
}

static RC_TYPE do_update_alias_table(DYN_DNS_CLIENT *p_self)
{
	int i;
	RC_TYPE rc = RC_OK;
	FILE *fp;
	
	do 
	{			
		for (i = 0; i < p_self->alias_info.count; ++i)
		{
			if (p_self->alias_info.update_required[i] != TRUE)
			{
				continue;
			}	
			
			rc = http_client_init(&p_self->http_to_dyndns);
			if (rc != RC_OK)
			{
				break;
			}
			
			/*build dyndns transaction*/
			{
				HTTP_TRANSACTION http_tr;
				http_tr.req_len = p_self->info.p_dns_system->p_dns_update_req_func(
                        (struct _DYN_DNS_CLIENT*) p_self,i,
						(struct DYNDNS_SYSTEM*) p_self->info.p_dns_system);
				http_tr.p_req = (char*) p_self->p_req_buffer;
				http_tr.p_rsp = (char*) p_self->p_work_buffer;
				http_tr.max_rsp_len = p_self->work_buffer_size - 1;/*save place for a \0 at the end*/
				http_tr.rsp_len = 0;
				p_self->p_work_buffer[http_tr.rsp_len+1] = 0;
				
				/*send it*/
				rc = http_client_transaction(&p_self->http_to_dyndns, &http_tr);					

				if (p_self->dbg.level > 2)
				{
					p_self->p_req_buffer[http_tr.req_len] = 0;
					DBG_PRINTF((LOG_DEBUG,"DYNDNS my Request:\n%s\n", p_self->p_req_buffer));
				}

				if (rc == RC_OK)
				{
					BOOL update_ok = 
                        p_self->info.p_dns_system->p_rsp_ok_func((struct _DYN_DNS_CLIENT*)p_self, 
                            http_tr.p_rsp, 
							p_self->info.p_dns_system->p_success_string);
					if (update_ok)
					{
			                        p_self->alias_info.update_required[i] = FALSE;

						DBG_PRINTF((LOG_WARNING,"I:" MODULE_TAG "Alias '%s' to IP '%s' updated successfully.\n", 
							p_self->alias_info.names[i].name,
							p_self->info.my_ip_address.name));                        
						p_self->times_since_last_update = 0;
						p_self->time_sleep_lost_ms=0;
						/*recalc forced update period*/
						p_self->forced_update_period_sec = p_self->forced_update_period_sec_orig;
						p_self->forced_update_times = p_self->forced_update_period_sec / p_self->sleep_sec;

#ifndef _WIN32
						if ((fp=fopen(p_self->ip_cache, "w")))
						{
							fprintf(fp,"%s", p_self->info.my_ip_address.name);
							fclose(fp);
						}

						if ((fp=fopen(p_self->time_cache, "w")))
						{
							fprintf(fp,"%ld", time (NULL));
							fclose(fp);
						}
#else
						if (!(isWinNT())) {
 #ifndef UNICOWS						
							if ((fp=fopen(p_self->ip_cache, "w")))
							{
								fprintf(fp,"%s", p_self->info.my_ip_address.name);
								fclose(fp);
							}

							if ((fp=fopen(p_self->time_cache, "w")))
							{
								fprintf(fp,"%ld", time (NULL));
								fclose(fp);
							}
						}
  #else
							;
						}
  #endif

  #ifndef UNICOWS

						else						
  #endif
						{
							wchar_t	*utf_16=NULL;


							if ((fp=_wfopen(utf_8_to_16(utf_malloc_8_to_16(&utf_16,p_self->ip_cache)
								,p_self->ip_cache), L"w")))
							{
								fprintf(fp,"%s", p_self->info.my_ip_address.name);
								fclose(fp);
							}

							free(utf_16);
						}

						{
							wchar_t	*utf_16=NULL;


							if ((fp=_wfopen(utf_8_to_16(utf_malloc_8_to_16(&utf_16,p_self->time_cache)
								,p_self->time_cache), L"w")))

							{
								fprintf(fp,"%ld", time (NULL));
								fclose(fp);
							}

							free(utf_16);
						}
#endif
						if (strlen(p_self->external_command) > 0)
							os_shell_execute(p_self->external_command);
					}
					else
					{
						DBG_PRINTF((LOG_WARNING,"W:" MODULE_TAG "Error validating DYNDNS svr answer. Check usr,pass,hostname!\n", http_tr.p_rsp));

						rc = RC_IP_SEND_ERROR;
					}
					if (p_self->dbg.level > 2)
					{							
						http_tr.p_rsp[http_tr.rsp_len] = 0;
						DBG_PRINTF((LOG_WARNING,"W:" MODULE_TAG "DYNDNS Server response:\n%s\n", http_tr.p_rsp));
					}
				}
			}
			
			{
				RC_TYPE rc2 = http_client_shutdown(&p_self->http_to_dyndns);
				if (rc == RC_OK)
				{
					rc = rc2;
				}			
			}
			if (rc != RC_OK)
			{
				break;
			}
			os_sleep_ms(1000);
		}
		if (rc != RC_OK)
		{
			break;
		}
	}
	while(0);
	return rc;
}


RC_TYPE get_default_config_data(DYN_DNS_CLIENT *p_self)
{
	RC_TYPE rc = RC_OK;

	do
	{
        p_self->info.p_dns_system = p_self->info.p_dns_system ? p_self->info.p_dns_system: get_dns_system_by_id(DYNDNS_MY_DNS_SYSTEM);
        if (p_self->info.p_dns_system == NULL)
        {
            rc = RC_DYNDNS_INVALID_DNS_SYSTEM_DEFAULT;
            break;
        }
						
		/*forced update period*/
		p_self->forced_update_period_sec = p_self->forced_update_period_sec ? p_self->forced_update_period_sec: DYNDNS_MY_FORCED_UPDATE_PERIOD_S;
		p_self->forced_update_period_sec_orig = p_self->forced_update_period_sec_orig ? p_self->forced_update_period_sec_orig: DYNDNS_MY_FORCED_UPDATE_PERIOD_S;

		/*network comm retries*/
		p_self->net_retries = p_self->net_retries ? p_self->net_retries: DYNDNS_NET_RETRIES;
		p_self->retry_interval = p_self->retry_interval ? p_self->retry_interval: DYNDNS_RETRY_INTERVAL;
#ifdef UNIX_OS
        if (!(p_self->ip_cache))
		  sprintf(p_self->ip_cache, "%s%s", DYNDNS_DEFAULT_CACHE_PREFIX, DYNDNS_DEFAULT_IP_FILE);

        if (!(p_self->time_cache))
		  sprintf(p_self->time_cache, "%s%s", DYNDNS_DEFAULT_CACHE_PREFIX, DYNDNS_DEFAULT_TIME_FILE);
#endif
#ifdef _WIN32
		p_self->ip_cache ? sprintf(p_self->ip_cache, "%s",p_self->ip_cache): sprintf(p_self->ip_cache, "%s", DYNDNS_DEFAULT_IP_FILE);
		p_self->time_cache ? sprintf(p_self->time_cache, "%s", p_self->time_cache): sprintf(p_self->time_cache, "%s", DYNDNS_DEFAULT_TIME_FILE);
#endif
		/*update period*/
		p_self->sleep_sec = p_self->sleep_sec ? p_self->sleep_sec: DYNDNS_DEFAULT_SLEEP;
	}
	while(0);
	
	return rc;
}


static RC_TYPE get_encoded_user_passwd(DYN_DNS_CLIENT *p_self,char *str_uri_encoded,int str_size)
{
	RC_TYPE rc = RC_OK;
	const char* format = "%s:%s";
	char *p_tmp_buff = NULL;
	int size = strlen(p_self->info.credentials.my_password) + 
			   strlen(p_self->info.credentials.my_username) + 
			   strlen(format) + 1;
	int actual_len;


	do
	{
		p_tmp_buff = (char *) safe_malloc(size);
		if (p_tmp_buff == NULL)
		{
			rc = RC_OUT_OF_MEMORY;
			break;
		}

		actual_len = sprintf(p_tmp_buff, format, 
				p_self->info.credentials.my_username, 
				p_self->info.credentials.my_password);
		if (actual_len >= size)
		{
			rc = RC_OUT_BUFFER_OVERFLOW;
			break;
		}

		/*encode*/

		memset(str_uri_encoded,0,str_size);

		p_self->info.credentials.p_enc_usr_passwd_buffer = 
			b64encode(utf_8_uri_encoded(str_uri_encoded,p_tmp_buff,"&#",";"));

		p_self->info.credentials.encoded = 
			(p_self->info.credentials.p_enc_usr_passwd_buffer != NULL);
		p_self->info.credentials.size = strlen(p_self->info.credentials.p_enc_usr_passwd_buffer);
	}
	while(0);

	if (p_tmp_buff != NULL)
	{
		free(p_tmp_buff);
	}

	return rc;	
}

/*************PUBLIC FUNCTIONS ******************/

/*
	printout
*/
void dyn_dns_print_hello(void*p)
{
	(void) p;

    DBG_PRINTF((LOG_SYSTEM, "S:" MODULE_TAG "Started 'inadyn-mt version %s' - dynamic DNS updater.\n", DYNDNS_VERSION_STRING));
}

/*
	 basic resource allocations for the dyn_dns object
*/
RC_TYPE dyn_dns_construct(DYN_DNS_CLIENT **pp_self)
{
	RC_TYPE rc;
	DYN_DNS_CLIENT *p_self;
    BOOL http_to_dyndns_constructed = FALSE;
    BOOL http_to_ip_constructed = FALSE;

	if (pp_self == NULL)
	{
		return RC_INVALID_POINTER;
	}
	/*alloc space for me*/
	*pp_self = (DYN_DNS_CLIENT *) safe_malloc(sizeof(DYN_DNS_CLIENT));
	if (*pp_self == NULL)
	{
		return RC_OUT_OF_MEMORY;
	}

	do
	{
		p_self = *pp_self;
		memset(p_self, 0, sizeof(DYN_DNS_CLIENT));
	
		/*alloc space for http_to_ip_server data*/
		p_self->work_buffer_size = DYNDNS_HTTP_RESPONSE_BUFFER_SIZE;
		p_self->p_work_buffer = (char*) safe_malloc(p_self->work_buffer_size);
		if (p_self->p_work_buffer == NULL)
		{
			rc = RC_OUT_OF_MEMORY;
			break;		
   		}
	
		/*alloc space for request data*/
		p_self->req_buffer_size = DYNDNS_HTTP_REQUEST_BUFFER_SIZE;
		p_self->p_req_buffer = (char*) safe_malloc(p_self->req_buffer_size);
		if (p_self->p_req_buffer == NULL)
		{
			rc = RC_OUT_OF_MEMORY;
			break;
		}
		
        
		rc = http_client_construct(&p_self->http_to_ip_server);
		if (rc != RC_OK)
		{	
			rc = RC_OUT_OF_MEMORY;
			break;
		}
        http_to_ip_constructed = TRUE;
	
		rc = http_client_construct(&p_self->http_to_dyndns);
		if (rc != RC_OK)
		{		
			rc = RC_OUT_OF_MEMORY;
			break;
		}
        http_to_dyndns_constructed = TRUE;
	
		(p_self)->cmd = NO_CMD;
		(p_self)->sleep_sec = DYNDNS_DEFAULT_SLEEP;
		(p_self)->total_iterations = DYNDNS_DEFAULT_ITERATIONS;	
		(p_self)->initialized = FALSE;
	
		p_self->info.credentials.p_enc_usr_passwd_buffer = NULL;

		p_self->lang_file = NULL;

	}
	while(0);

    if (rc != RC_OK)
    {
        if (*pp_self)
        {
            free(*pp_self);
        }
        if (p_self->p_work_buffer != NULL)
        {
            free(p_self->p_work_buffer);
        }
        if (p_self->p_req_buffer != NULL)
        {
            free (p_self->p_work_buffer);
        }
        if (http_to_dyndns_constructed) 
        {
            http_client_destruct(&p_self->http_to_dyndns);
        }
        if (http_to_ip_constructed) 
        {
            http_client_destruct(&p_self->http_to_ip_server);
        }            
    }

	return RC_OK;
}


/*
	Resource free.
*/	
RC_TYPE dyn_dns_destruct(DYN_DNS_CLIENT *p_self)
{
	RC_TYPE rc;
	if (p_self == NULL)
	{
		return RC_OK;
	}

	if (p_self->initialized == TRUE)
	{
		dyn_dns_shutdown(p_self);
	}

	rc = http_client_destruct(&p_self->http_to_ip_server);
	if (rc != RC_OK)
	{		
		
	}

	rc = http_client_destruct(&p_self->http_to_dyndns);
	if (rc != RC_OK)
	{	
		
	}

	if (p_self->p_work_buffer != NULL)
	{
		free(p_self->p_work_buffer);
		p_self->p_work_buffer = NULL;
	}

	if (p_self->p_req_buffer != NULL)
	{
		free(p_self->p_req_buffer);
		p_self->p_req_buffer = NULL;
	}

	if (p_self->info.credentials.p_enc_usr_passwd_buffer != NULL)
	{
		free(p_self->info.credentials.p_enc_usr_passwd_buffer);
		p_self->info.credentials.p_enc_usr_passwd_buffer = NULL;
	}

	if (p_self->lang_file != NULL)
	{

		free(p_self->lang_file);
		p_self->lang_file = NULL;
	}


	free(p_self);
	p_self = NULL;

	return RC_OK;
}

/* 
	Sets up the object.
	- sets the IPs of the DYN DNS server
    - if proxy server is set use it! 
	- ...
*/
RC_TYPE dyn_dns_init(DYN_DNS_CLIENT *p_self)
{
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (p_self->initialized == TRUE)
	{
		return RC_OK;
	}

	p_self->abort_on_network_errors = FALSE;
	p_self->force_addr_update = FALSE;

    if (strlen(p_self->info.proxy_server_name.name) > 0)
    {
        http_client_set_port(&p_self->http_to_ip_server, p_self->info.proxy_server_name.port);
        http_client_set_remote_name(&p_self->http_to_ip_server, p_self->info.proxy_server_name.name);

        http_client_set_port(&p_self->http_to_dyndns, p_self->info.proxy_server_name.port);
        http_client_set_remote_name(&p_self->http_to_dyndns, p_self->info.proxy_server_name.name);
    }
    else
    {
        http_client_set_port(&p_self->http_to_ip_server, p_self->info.ip_server_name.port);
        http_client_set_remote_name(&p_self->http_to_ip_server, p_self->info.ip_server_name.name);

        http_client_set_port(&p_self->http_to_dyndns, p_self->info.dyndns_server_name.port);
        http_client_set_remote_name(&p_self->http_to_dyndns, p_self->info.dyndns_server_name.name);    
    }

	p_self->cmd = NO_CMD;
    if (p_self->cmd_check_period == 0)
    {
	    p_self->cmd_check_period = DYNDNS_DEFAULT_CMD_CHECK_PERIOD;
    }


	p_self->initialized = TRUE;
	return RC_OK;
}

/* 
	Disconnect and some other clean up.
*/
RC_TYPE dyn_dns_shutdown(DYN_DNS_CLIENT *p_self)
{
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (p_self->initialized == FALSE)
	{
		return RC_OK;
	}

	return RC_OK;
}

static void sleep_lightly_ms(DYN_DNS_CLIENT *p_self)
{

	int ms=p_self->retry_interval;


#ifndef _WIN32

	while (1) {

#else

	while (!(returnSignaled)) {

#endif

		os_sleep_ms((ms<500) ? ms:500);

		ms-=500;

		if (ms<=0 || p_self->cmd==CMD_STOP)

			break;
	}
}

/*
	- increment the forced update times counter
	- detect current IP
		- connect to an HTTP server 
		- parse the response for IP addr

	- for all the names that have to be maintained
		- get the current DYN DNS address from DYN DNS server
		- compare and update if neccessary
*/
#ifndef _WIN32

RC_TYPE dyn_dns_update_ip(DYN_DNS_CLIENT *p_self)

#else

RC_TYPE dyn_dns_update_ip(DYN_DNS_CLIENT *p_self,RAS_THREAD_DATA *p_ras_thread_data)

#endif
{
	RC_TYPE rc;
	int		net_attempts=1;


	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	do
	{

#ifndef _WIN32

		while (!(p_self->cmd==CMD_STOP)) {
#else
		while (!(returnSignaled) && !(p_self->cmd==CMD_STOP) && is_online(p_ras_thread_data,
				p_self->http_to_ip_server.super.super.p_remote_host_name,p_self->dbg.level)) {

#endif
			/*ask IP server something so he will respond and give me my IP */
			rc = do_ip_server_transaction(p_self);

			if (rc==RC_OK)

				break;

			if (p_self->net_retries<net_attempts++)

				break;

			if (p_self->dbg.level>=LOG_WARNING)

				DBG_PRINTF((LOG_INFO,"W:DYNDNS: Failed checking current ip... iterating after retry interval...\n"));


			sleep_lightly_ms(p_self);
		} 

		if (rc != RC_OK)
		{
			DBG_PRINTF((LOG_WARNING,"W:DYNDNS: Error '%s' (0x%x) when talking to IP server\n",
				errorcode_get_name(rc), rc));
			break;
		}
		if (p_self->dbg.level >= LOG_INFO)
		{
			DBG_PRINTF((LOG_INFO,"D:DYNDNS: IP server response: %s\n", p_self->p_work_buffer));		
		}

		/*extract my IP, check if different than previous one*/
		rc = do_parse_my_ip_address(p_self);
		if (rc != RC_OK)
		{	
			break;
		}
		
		if (p_self->dbg.level >= LOG_INFO)
		{
			DBG_PRINTF((LOG_INFO,"W:DYNDNS: My IP address: %s\n", p_self->info.my_ip_address.name));		
		}

		/*step through aliases list, resolve them and check if they point to my IP*/
		rc = do_check_alias_update_table(p_self);
		if (rc != RC_OK)
		{	
			break;
		}

#ifndef _WIN32

		while (!(p_self->cmd==CMD_STOP)) {

#else
		while(!(returnSignaled) && !(p_self->cmd==CMD_STOP) && is_online(p_ras_thread_data,
				p_self->http_to_ip_server.super.super.p_remote_host_name,p_self->dbg.level)) {

#endif

			/*update IPs marked as not identical with my IP*/
			rc = do_update_alias_table(p_self);

			if (rc==RC_OK)

				break;


			if (p_self->net_retries<net_attempts++)

				break;

			if (p_self->dbg.level>=LOG_WARNING)

				DBG_PRINTF((LOG_INFO,"W:DYNDNS: Failed updating alias table... iterating after retry interval...\n"));


			sleep_lightly_ms(p_self);
		}

	}
	while(0);

	return rc;
}

#ifdef _WIN32

/*try to get mutex without waiting, and unless is_waited flagged as NULL,
  wait for mutex if have to, and set is_waited to indicate had to wait.
*/
DWORD get_mutex(DYN_DNS_CLIENT *p_dyndns,HANDLE *hMutex,int *is_waited)
{

	int	wait_ret=0;


	if (is_waited)

		*is_waited=0;


	if (*hMutex) {

		wait_ret=WaitForSingleObject(*hMutex,0);


		if (!(wait_ret==WAIT_OBJECT_0)) {

			if (!(wait_ret==WAIT_TIMEOUT)) {

				dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "WaitForSingleObject failed in get_mutex.",
										GetLastError(),LOG_CRIT,p_dyndns->dbg.level);
			}
			else {

				if (is_waited) {

					wait_ret=WaitForSingleObject(*hMutex,INFINITE);

					if (!(wait_ret==WAIT_OBJECT_0))
	
						dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "WaitForSingleObject failed in get_mutex.",
												GetLastError(),LOG_CRIT,p_dyndns->dbg.level);


					*is_waited=1;
				}
			}
		}
	}

	return wait_ret;
}

DWORD get_mutex_wait(DYN_DNS_CLIENT *p_dyndns,HANDLE *hMutex)
{

	int	is_waited;


	return get_mutex(p_dyndns,hMutex,&is_waited);
}

int release_mutex(DYN_DNS_CLIENT *p_dyndns,HANDLE *hMutex)
{
 
	int	release_ret=0;


	if (*hMutex)

		if (!(release_ret=ReleaseMutex(*hMutex)))

			dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "Failed ReleaseMutex in get_mutex  Low resources?  System is unstable...",GetLastError(),LOG_CRIT,p_dyndns->dbg.level);


	return release_ret;
}

static RC_TYPE do_attempt_update(DYN_DNS_CLIENT *p_dyndns,RAS_THREAD_DATA *p_ras_thread_data)
{

  RC_TYPE            update_ret=RC_OK;

  DWORD              wait_ret=0;


  if (p_dyndns->dbg.level>=LOG_INFO)

    DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "Attempting ip update...\n"));

  wait_ret=WaitForSingleObject(hUpdateMutex,0);

  if (!(wait_ret==WAIT_OBJECT_0)) {

    if (!(wait_ret==WAIT_TIMEOUT))

      dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "WaitForSingleObject failed in update handler.",GetLastError(),LOG_CRIT,p_dyndns->dbg.level);

  }
  else {


    if (p_dyndns->dbg.level>=LOG_INFO)

      DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "Got mutex...\n"));


    if (!(is_online(p_ras_thread_data,p_dyndns->http_to_ip_server.super.super.p_remote_host_name,
                    p_dyndns->dbg.level))) {

      if (p_dyndns->dbg.level>=LOG_INFO)

        DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "Not online, skipping update...\n"));
    }
    else {

      update_ret=dyn_dns_update_ip(p_dyndns,p_ras_thread_data);

      if (update_ret==RC_OK) {

        p_dyndns->iterations++;

        if (p_dyndns->dbg.level>=LOG_INFO)

          DBG_PRINTF((LOG_INFO, "I:" MODULE_TAG "Update ip returned OK...\n"));
      }
    }

    if (!(ReleaseMutex(hUpdateMutex)))

      dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "Failed ReleaseMutex.  Low resources?  System is unstable...",GetLastError(),LOG_CRIT,p_dyndns->dbg.level);

    else 

      if (p_dyndns->dbg.level>=LOG_INFO)

        DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "Update handler released mutex...\n"));
  }

  return update_ret;
}

static RC_TYPE attempt_update(DYN_DNS_CLIENT *p_dyndns,RAS_THREAD_DATA *p_ras_thread_data)
{

  RC_TYPE            update_ret=RC_OK;

  DWORD              wait_ret=0;


  wait_ret=WaitForSingleObject(hUpdateMutex,0);

  if (!(wait_ret==WAIT_OBJECT_0)) {

    if (!(wait_ret==WAIT_TIMEOUT))

      dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "WaitForSingleObject failed in RAS update handler.",GetLastError(),LOG_CRIT,p_dyndns->dbg.level);

  }
  else {


    update_ret=do_attempt_update(p_dyndns,p_ras_thread_data);


    if (update_ret==RC_OK) 

      if (!(p_dyndns->cmd==CMD_STOP))

        p_dyndns->cmd=CMD_UPDTED;


    if (!(ReleaseMutex(hUpdateMutex)))

      dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "Failed ReleaseMutex.  Low resources?  System is unstable...",GetLastError(),LOG_CRIT,p_dyndns->dbg.level);

  }

  return update_ret;
}

/**********************Event Handlers**********************/

static void dyn_dns_update_ip_handler(RAS_THREAD_DATA *p_ras_thread_data)
{  
  
  DYN_DNS_CLIENT     *p_dyndns;


  p_dyndns=p_ras_thread_data->p_context;
  
/* need reinit dyndns structure (with synchronization) on fail?
*/

  if (p_ras_thread_data->dwEvent==RAS_CONNECT) {

    attempt_update(p_dyndns,p_ras_thread_data);
  }
  else {

    if (p_dyndns->dbg.level>=LOG_INFO)

      DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "Not an event of interest...\n"));
  }
}

int service_event_handler(SERVICE_EVENT service_event)
{

/*trigger dyn_dns_main return*/

  DBG_PRINTF((LOG_INFO, "I:" MODULE_TAG "Entered service_event_handler.  Setting return signal...\n"));


  returnSignaled=true;

  return 1;
}

void start_ras_thread(DYN_DNS_CLIENT *p_dyndns,RAS_THREAD_DATA **p_ras_thread_data,HANDLE *hUpdateMutex)
{

  *hUpdateMutex=CreateMutex(NULL,false,NULL);

  if (!(*hUpdateMutex)) {

    dbg_printf_and_err_str(NULL,"C:" MODULE_TAG "Failed create mutex.  Low resources?  System is unstable.  Continuing wihtout RAS events trapping...",GetLastError(),LOG_CRIT,p_dyndns->dbg.level);
  }
  else {

    if (p_dyndns->dbg.level>=LOG_INFO)

      DBG_PRINTF((LOG_INFO, "I:" MODULE_TAG "Creating, launching ras thread...\n"));

    *p_ras_thread_data=construct_and_launch_trap_ras_events(dyn_dns_update_ip_handler,p_dyndns,p_dyndns->\
                                                            http_to_ip_server.super.super.p_remote_host_name,\
                                                            p_dyndns->dbg.level);

    if (*p_ras_thread_data) {

      if (p_dyndns->dbg.level>=LOG_INFO)

        DBG_PRINTF((LOG_INFO, "I:" MODULE_TAG "Launched RAS events trapping...\n"));

    }
    else {

      CloseHandle(*hUpdateMutex);

      *hUpdateMutex=NULL;

      if (p_dyndns->dbg.level>=LOG_ERR)

        DBG_PRINTF((LOG_ERR, "E:" MODULE_TAG "RAS events trapping constructor returned NULL.  Continuing without RAS events trapping...\n"));
    }
  }
}

void atomic_inc(DYN_DNS_CLIENT *p_self,int *src,int inc,HANDLE hMutex)
{

	get_mutex_wait(p_self,&hMutex);


	*src+=inc;


    release_mutex(p_self,&hMutex);
}

#endif

RC_TYPE dyn_dns_reinit(DYN_DNS_CLIENT *p_dyndns,void **hUpdateMutex)
{

	RC_TYPE		rc=RC_OK;

#ifdef _WIN32

	HANDLE *hMutex=(HANDLE *) hUpdateMutex;

  /*if ras has mutex, wait for it so can 
    reset update structures.
  */
	get_mutex_wait(p_dyndns,hMutex);

#endif

	dyn_dns_shutdown(p_dyndns);

	rc=dyn_dns_init(p_dyndns);

#ifdef _WIN32

	release_mutex(p_dyndns,hMutex);
#endif

  return rc;
}

/*  Record the last ip as per that in ip cache file.  Report it via log output.
*/

RC_TYPE check_ip_cache(DYN_DNS_CLIENT *p_dyndns)
{

	FILE	*fp;
	RC_TYPE	rc=RC_OK;

#ifndef _WIN32

	if ((fp=fopen(p_dyndns->ip_cache, "r")))
	{
		fgets (p_dyndns->info.my_ip_address.name, sizeof (p_dyndns->info.my_ip_address.name),fp);

		fclose(fp);

		
		DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "IP read from cache file is '%s'...\n", p_dyndns->info.my_ip_address.name));
	}

#else

	if (!(isWinNT())) {

 #ifndef UNICOWS

		if ((fp=fopen(p_dyndns->ip_cache, "r")))
		{
			fgets (p_dyndns->info.my_ip_address.name, sizeof (p_dyndns->info.my_ip_address.name),fp);

			fclose(fp);

		
			DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "IP read from cache file is '%s'...\n",p_dyndns->info.my_ip_address.name));
		}
	}

  #else
		;
	}

  #endif

  #ifndef UNICOWS

	else

  #endif

	{
		wchar_t	*utf_16=NULL;

		if ((fp=_wfopen(utf_8_to_16(utf_malloc_8_to_16(&utf_16,p_dyndns->ip_cache)
						,p_dyndns->ip_cache), L"r")))
		{
			fgets (p_dyndns->info.my_ip_address.name, sizeof (p_dyndns->info.my_ip_address.name),fp);

			fclose(fp);

		
			DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "IP read from cache file is '%s'...\n", p_dyndns->info.my_ip_address.name));
		}

		free(utf_16);
	}
#endif

	return rc;
}

BOOL is_completed_iterations(DYN_DNS_CLIENT *p_dyndns,int *iteration)
{


#ifdef _WIN32

	get_mutex_wait(p_dyndns,&hUpdateMutex);
#endif

	*iteration=p_dyndns->iterations;

#ifdef _WIN32

	release_mutex(p_dyndns,&hUpdateMutex);
#endif

        /* check if the user wants us to stop */

	return (*iteration >= p_dyndns->total_iterations && p_dyndns->total_iterations != 0);
}

#ifndef _WIN32

RC_TYPE init_update_loop(DYN_DNS_CLIENT *p_dyndns,int argc, char* argv[],char *str_uri_encoded,BOOL *init_flag)

#else

RC_TYPE init_update_loop(DYN_DNS_CLIENT *p_dyndns,int argc, char* argv[],RAS_THREAD_DATA **p_ras_thread_data,
                         char *str_uri_encoded,BOOL *init_flag)

#endif

{
	RC_TYPE			rc = RC_OK;
	RC_TYPE			rc_cmd_line = RC_OK;

#ifdef _WIN32
    int             regParamsC=1;
    wchar_t         *regArgs[50];
    char            *utf_8_argv[50];
    int             i=0;
    
    RC_TYPE         rc_reg=RC_ERROR;
#endif

  if (p_dyndns == NULL)

    return RC_INVALID_POINTER;

#ifdef _WIN32

  SetLastError(0);

/*set up to any registry parameters first -- input file, 
  and command args override registry*/

  utf_8_argv[0]=safe_malloc(strlen(argv[0])+1);
  strcpy(utf_8_argv[0],argv[0]);

/*if we're a service, service_main will have these*/
  regParamsC+=get_registry_parameters(regArgs);

  if (regParamsC>1) {

    utf_16_to_8_ar((utf_8_argv+1),regArgs,regParamsC-1);

	rc_reg=get_config_data(p_dyndns,regParamsC,utf_8_argv);

    for (i=0;i<regParamsC;i++) free(regArgs[i]);

    utf_free_ar(utf_8_argv,regParamsC);

	if (!(rc_reg == RC_OK))

		return rc_reg;
  }

#endif

  /* read cmd line options and set object properties*/
 	rc_cmd_line = get_config_data(p_dyndns, argc, argv);


#ifndef _WIN32

  if (rc_cmd_line != RC_OK || p_dyndns->abort)

#else

  if (!(rc_cmd_line == RC_OK || rc_reg == RC_OK) || p_dyndns->abort)

#endif

    return rc_cmd_line;


  /*if opt around default language strings, use that*/
  if (p_dyndns->lang_file) {

	if (re_init_lang_strings(p_dyndns->lang_file)==RC_OK)

		DBG_PRINTF((LOG_INFO, "I:" MODULE_TAG "Using default override language strings file, %s...\n",p_dyndns->lang_file));

	else

		DBG_PRINTF((LOG_WARNING, "W:" MODULE_TAG "Failed using default override language strings file, %s...\n",p_dyndns->lang_file));
  }

  /*if logfile provided, redirect output to log file*/
  if (strlen(p_dyndns->dbg.p_logfilename) != 0)
  {

    DBG_PRINTF((LOG_INFO, "I:" MODULE_TAG "Attempting to open log file: %s...\n",p_dyndns->dbg.p_logfilename));

    rc = os_open_dbg_output(DBG_FILE_LOG, "", p_dyndns->dbg.p_logfilename);

    if (rc != RC_OK)

      return rc;
  }

  if (p_dyndns->debug_to_syslog == TRUE || (p_dyndns->run_in_background == TRUE))
  {
    if (get_dbg_dest() == DBG_STD_LOG) /*avoid file and syslog output */
    {
      rc = os_open_dbg_output(DBG_SYS_LOG, "INADYN", NULL);
      if (rc != RC_OK)

        return rc;
    }
  }

  if (p_dyndns->change_persona)
  {
    OS_USER_INFO os_usr_info;
    memset(&os_usr_info, 0, sizeof(os_usr_info));
    os_usr_info.gid = p_dyndns->sys_usr_info.gid;
    os_usr_info.uid = p_dyndns->sys_usr_info.uid;
    rc = os_change_persona(&os_usr_info);
    if (rc != RC_OK)
    {
      return rc;
    }
  }

  /*if silent required, close console window*/
  if (p_dyndns->run_in_background == TRUE)
  {
    rc = close_console_window();
    if (rc != RC_OK)
    {
      return rc;
    }       
    if (get_dbg_dest() == DBG_SYS_LOG)
    {
      fclose(stdout);
    }
  }

  dyn_dns_print_hello(NULL);

/*  now that log is open, report any command line errors eventhough registry params made up for them --
    if argc is 1, parser returns error if can't open default config file -- ignore that condition
*/
  if (rc_cmd_line != RC_OK && argc > 1) {

    DBG_PRINTF((LOG_WARNING, "W:" MODULE_TAG "%s error returned getting command line "\
               "parameters.  One or more command line parameters ignored...\n",errorcode_get_name(rc_cmd_line)));

  }

  check_ip_cache(p_dyndns);

  rc = dyn_dns_init(p_dyndns);

  if (rc==RC_OK) {

    *init_flag=true;

    rc = os_install_signal_handler(p_dyndns);

    if (rc != RC_OK)

      DBG_PRINTF((LOG_WARNING,"DYNDNS: Error '%s' (0x%x) installing OS signal handler\n",errorcode_get_name(rc), rc));
  }

  if (rc==RC_OK) {

    rc = get_encoded_user_passwd(p_dyndns,str_uri_encoded,1024);

#ifdef _WIN32

    start_ras_thread(p_dyndns,p_ras_thread_data,&hUpdateMutex);

#endif

  }

  return rc;
}

/* MAIN - Dyn DNS update entry point.*/

/* 
	Actions:
		- read the configuration options
		- perform various init actions as specified in the options
		- create and init dyn_dns object.
		- launch the IP update action loop
*/		
int dyn_dns_main(DYN_DNS_CLIENT *p_dyndns, int argc, char* argv[])
{

  RC_TYPE			rc=RC_OK;
  BOOL				quit_flag=FALSE;
  BOOL				init_flag=FALSE;
  char				str_uri_encoded[1024];
  int				current_iteration=0;

#ifdef _WIN32

  RAS_THREAD_DATA	*p_ras_thread_data=NULL;


  rc=init_update_loop(p_dyndns,argc,argv,&p_ras_thread_data,str_uri_encoded,&init_flag);

#else


  rc=init_update_loop(p_dyndns,argc,argv,str_uri_encoded,&init_flag);
    
#endif

  if (p_dyndns->abort)

    return rc;

/*

Flow:  Two threads -- main, and ras

init

spawn thread wait for ras:

do
  wait for event of interest
    if kill thread event
      shut down thread
    distpatch callback handler
      if kill thread event
        break
      if disconnect event
        break
      if connect event
        do
          update
        till comm success or number of retries or disconnect
loop;

do (main thread)
  do 
    update;

    if not update okay
      break

    wait timer os signal handler;

    if kill signal
      kill events thread
      quit program
  loop

  re-init
loop (main thread)

The ras thread gives dns updates as soon as go online,
so don't have to wait for timer interval, and don't
have to busy with shorter intervals for more responsive
dns updates -- the catch may be, it may be possible
(I don't know), depending on connection hardware, type
of connection, etcetera, to change ip without triggering
a connect, or disconnect ras event, in which case, we'll
miss it, so we synchronize interval updates in main thread, 
with ras updates thread.  This set up also gives a bit of
a fail safe.

*/

  do
  {

    if (rc != RC_OK)
    {
      break;
    }

    /*update IP address in a loop*/
		
    while(1)
    {

      if (is_completed_iterations(p_dyndns,&current_iteration))

        break;

#ifndef _WIN32

      rc = dyn_dns_update_ip(p_dyndns);

      if (rc==RC_OK)

        p_dyndns->iterations++;
#else

      if (hUpdateMutex) { /*ras thread successfully start, and running?*/

        rc=do_attempt_update(p_dyndns,p_ras_thread_data);

      }
      else {

		rc=dyn_dns_update_ip(p_dyndns,NULL);

        if (rc==RC_OK)

          p_dyndns->iterations++;
      }
			
#endif   

      if (is_completed_iterations(p_dyndns,&current_iteration)) {

        rc=RC_OK;

        break;
      }

	  if (!(rc == RC_OK)) {

	    DBG_PRINTF((LOG_WARNING,"W:'%s' (0x%x) updating the IPs. (it %d)\n",
		errorcode_get_name(rc), rc, current_iteration));

#ifndef _WIN32

        rc=dyn_dns_reinit(p_dyndns,NULL);

#else
        rc=dyn_dns_reinit(p_dyndns,&hUpdateMutex);

#endif

	  } 

      if (!(rc==RC_OK)) {

        init_flag=false;

        break;
      }

#ifndef _WIN32

      /* sleep the time set in the ->sleep_sec data memeber*/
      dyn_dns_wait_for_cmd(p_dyndns,NULL);

      p_dyndns->times_since_last_update++;

#else
      /* sleep the time set in the ->sleep_sec data memeber*/
      dyn_dns_wait_for_cmd(p_dyndns,&hUpdateMutex);

      atomic_inc(p_dyndns,&(p_dyndns->times_since_last_update),1,hUpdateMutex);
#endif

/*TODO:  Main loop update failures should be added to forced update period time;
         mutex waits for RAS thread update, in event of RAS thread update failure, 
         should be added to forced update period time, or otherwise adjusted for.
*/


      /*reset the command*/
      if (!(p_dyndns->cmd==CMD_STOP))

        p_dyndns->cmd=NO_CMD;

      else {
    
          DBG_PRINTF((LOG_DEBUG,"I:" MODULE_TAG "STOP command received. Exiting.\n"));

          break;
      }

#ifndef _WIN32

      if (p_dyndns->dbg.level > 0)
      {

        DBG_PRINTF((LOG_DEBUG, "."));
      }    

#endif

    } /*while*/
		
    /*if everything ok here we should exit. End of program*/
    if (rc == RC_OK)
    {
      break;
    }

  }
  while(quit_flag == FALSE);

#ifdef _WIN32

  destroy_trap_ras_events(&p_ras_thread_data); 

#endif

  if (init_flag)
  {
    /* dyn_dns_shutdown object */			
    rc = dyn_dns_shutdown(p_dyndns);
  }

  return rc;
}
