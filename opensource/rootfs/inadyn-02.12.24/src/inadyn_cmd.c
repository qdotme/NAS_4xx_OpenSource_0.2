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
		- many options added
		- january 2005 - new format for the config file =Thanks to Jerome Benoit. 
        - january 30 2005 - new parser for config file -
        - october 2007 - debug level command line parameter added
        - dec 2007 - file options handler now provides for command line options precedence
*/
#define MODULE_TAG "CMD_OPTS: "

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>

#include "dyndns.h"
#include "debug_if.h"
#include "base64.h"
#include "get_cmd.h"
#include "unicode_util.h"
#include "safe_mem.h"
#include "path.h"

/* command line options */
#define DYNDNS_INPUT_FILE_OPT_STRING "--input_file"

static RC_TYPE help_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE wildcard_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_username_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_password_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_alias_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_debug_level_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_dns_server_name_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_dns_server_url_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_ip_server_name_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_dyndns_system_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_update_period_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_update_period_sec_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_forced_update_period_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_logfile_name(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE set_silent_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE set_verbose_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_proxy_server_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_options_from_file_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE set_iterations_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE set_syslog_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE set_change_persona_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE print_version_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_exec_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_cache_dir(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_retries_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_retry_interval_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);
static RC_TYPE get_lang_file_handler(CMD_DATA *p_cmd, int current_nr, void *p_context);

static CMD_DESCRIPTION_TYPE cmd_options_table[] = 
{
	{"--help",		0,	{help_handler, NULL,0},	"help" },
	{"-h",			0,	{help_handler, NULL,0},	"help" },

	{"--username",	1,	{get_username_handler, NULL,0},	"your  membername/ hash"},
	{"-u",			1,	{get_username_handler, NULL,0},	"your  membername / hash"},

	{"--password",	1,	{get_password_handler, NULL,0},	"your password. Optional."},
	{"-p",			1,	{get_password_handler, NULL,0},	"your password"},

	{"--alias",		1,	{get_alias_handler, NULL,0},	"alias host name. this option can appear multiple times." },
	{"-a",			1,	{get_alias_handler, NULL,0},	"alias host name. this option can appear multiple times." },

	{"--debug",     1,  {get_debug_level_handler, NULL,0}, "debug level 0..7; higher number, more log debug messages."},
	{"-d",          1,  {get_debug_level_handler, NULL,0}, "debug level 0..7; higher number, more log debug messages."},

/*
for help display only -- service_main takes care of these handlers

could have a servie_main_display_help() function instead
*/

#ifdef _WIN32  
  {"-i",1,{NULL,NULL,0},"[quoted service description] install service"},
  {"-s",0,{NULL, NULL,0},"start service"},
  {"-e",0,{NULL, NULL,0},"exit service"},
  {"-r",0,{NULL, NULL,0},"restart service"},
  {"-x",0,{NULL,NULL,0},"uninstall service"},
#endif


	{DYNDNS_INPUT_FILE_OPT_STRING, 1, {get_options_from_file_handler, NULL,0}, "the file containing [further] inadyn options.  "
			"The default config file, '" DYNDNS_DEFAULT_CONFIG_FILE "' is used if inadyn is called without any cmd line options.  "\
            "Input file options are inserted at point of this option's appearance." },
	
	{"--ip_server_name",	2,	{get_ip_server_name_handler, NULL,0},
        "<srv_name[:port] local_url> - local IP is detected by parsing the response after returned by this server and URL. \n"
		"\t\tThe first IP in found in http response is considered 'my IP'. \n"
		"\t\tDefault value: 'checkip.dyndns.org /"},

	{"--dyndns_server_name", 1,	{get_dns_server_name_handler, NULL,0},	
            "[<NAME>[:port]] \n"
            "\t\tThe server that receives the update DNS request.  \n"
            "\t\tAllows the use of unknown DNS services that accept HTTP updates.\n"  
            "\t\tIf no proxy is wanted, then it is enough to set the dyndns system. The default servers will be taken."},

	{"--dyndns_server_url", 1, {get_dns_server_url_handler, NULL,0},	
            "<name>\n"
			"\tfull URL relative to DynDNS server root.\n"
			"\tEx: /some_script.php?hostname=\n"},	

	{"--dyndns_system",	1,	{get_dyndns_system_handler, NULL,0},	
            "[NAME] - optional DYNDNS service type. SHOULD be one of the following: \n"
            "\t\t-For dyndns.org: dyndns@dyndns.org OR statdns@dyndns.org OR customdns@dyndns.org.\n"
            "\t\t-For freedns.afraid.org: default@freedns.afraid.org\n"
            "\t\t-For zoneedit.com: default@zoneedit.com\n"
            "\t\t-For no-ip.com: default@no-ip.com\n"
            "\t\t-For easydns.com: default@easydns.com\n"
            "\t\t-For 3322.org: dyndns@3322.org\n"
            "\t\t-For generic: custom@http_svr_basic_auth\n"
            "\t\tDEFAULT value is intended for default service at dyndns.org (most users): dyndns@dyndns.org"},

    {"--proxy_server", 1, {get_proxy_server_handler, NULL,0},
            "[NAME[:port]]  - the http proxy server name and port. Default is none."},
	{"--update_period",	1,	{get_update_period_handler, NULL,0},	
            "how often the IP is checked. The period is in [ms]. Default is about 1 min. Max is 10 days"},
	{"--update_period_sec",	1,	{get_update_period_sec_handler, NULL,0},	"how often the IP is checked. The period is in [sec]. Default is about 1 min. Max is 10 days"},
	{"--forced_update_period", 1,   {get_forced_update_period_handler, NULL,0},"how often the IP is updated even if it is not changed. [in sec]"},

	{"--log_file",	1,	{get_logfile_name, NULL,0},		"log file path abd name"},
	{"--background", 0,	{set_silent_handler, NULL,0},		"run in background. output to log file or to syslog"},

	{"--verbose",	1,	{set_verbose_handler, NULL,0},	"set dbg level. 0 to 5"},

	{"--iterations",	1,	{set_iterations_handler, NULL,0},	"set the number of DNS updates. Default is 0, which means infinity."},
	{"--syslog",	0,	{set_syslog_handler, NULL,0},	"force logging to syslog . (e.g. /var/log/messages). Works on **NIX systems only."},
	{"--change_persona", 1, {set_change_persona_handler, NULL,0}, "after init switch to a new user/group. Parameters: <uid[:gid]> to change to. Works on **NIX systems only."},
	{"--version", 0, {print_version_handler, NULL,0}, "print the version number\n"},
	{"--exec", 1, {get_exec_handler, NULL,0}, "external command to exec after an IP update. Include the full path."},
	{"--cache_dir", 1, {get_cache_dir, NULL,0}, "cache directory name. (e.g. /tmp/ddns). Defaults to /tmp on **NIX systems."},
	{"--wildcard", 0, {wildcard_handler, NULL,0}, "enable domain wildcarding for dyndns.org, 3322.org, or easydns.com."},
	{"--retries", 1, {get_retries_handler, NULL,0}, "network comm retry attempts.  0 to 100, default 0"},
	{"--retry_interval", 1, {get_retry_interval_handler, NULL,0}, "network comm miliseconds retry interval.  0 to 30,000, default 1,000"},
	{"--lang_file", 1, {get_lang_file_handler, NULL,0}, "language file path, and file name. defaults to either ../inadyn-mt/lang/en.lng, or /etc/inadyn-mt/en.lng"},
	{NULL,		0,	{0, NULL,0},	NULL }
};


void print_help_page(void)
{
	printf("\n\n\n" \
	"			INADYN-MT Help\n\n" \
	"	INADYN-MT is a dynamic DNS client. That is, it maintains the IP address\n" \
	"of a host name. It periodically checks whether the IP address of the current machine\n" \
	"(the external visible IP address of the machine that runs INADYN) has changed.\n" \
    "If yes it performs an update in the dynamic dns server.\n\n");
    printf("Typical usage: \n"	\
    "\t-for dyndns.org system: \n" \
    "\t\tinadyn-mt -u username -p password -a my.registrated.name \n" \
    "\t-for freedns.afraid.org:\n" \
    "\t\t inadyn-mt --dyndns_system default@freedns.afraid.org -a my.registrated.name,hash -a anothername,hash2\n" \
    "\t\t\t 'hash' is extracted from the grab url batch file that is downloaded from freedns.afraid.org\n\n" \
	"Parameters:\n");

	{
		CMD_DESCRIPTION_TYPE *it = cmd_options_table;
		
		while( it->p_option != NULL)
		{
			printf(
				"\t'%s': %s\n\r",
				it->p_option, it->p_description);
			++it;
		}
	}
	printf("\n\n\n");
}

static RC_TYPE help_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}
	p_self->abort = TRUE;
	print_help_page();
	return RC_OK;
}

static RC_TYPE wildcard_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}
	p_self->wildcard = TRUE;
	return RC_OK;
}

static RC_TYPE set_verbose_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sscanf(p_cmd->argv[current_nr], "%d", &p_self->dbg.level) != 1)
	{
		return RC_DYNDNS_INVALID_OPTION;
	}
	return RC_OK;
}

static RC_TYPE set_iterations_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sscanf(p_cmd->argv[current_nr], "%d", &p_self->total_iterations) != 1)
	{
		return RC_DYNDNS_INVALID_OPTION;
	}
	
	p_self->total_iterations = (p_self->sleep_sec < 0) ?  DYNDNS_DEFAULT_ITERATIONS : p_self->total_iterations;
	return RC_OK;
}

static RC_TYPE set_silent_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	p_self->run_in_background = TRUE;
	return RC_OK;
}


static RC_TYPE get_logfile_name(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sizeof(p_self->dbg.p_logfilename) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
	strcpy(p_self->dbg.p_logfilename, p_cmd->argv[current_nr]);
	return RC_OK;
}


static RC_TYPE get_username_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	/*user*/
	if (sizeof(p_self->info.credentials.my_username) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
	strcpy(p_self->info.credentials.my_username, p_cmd->argv[current_nr]);

	return RC_OK;
}

static RC_TYPE get_password_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	/*user*/
	if (sizeof(p_self->info.credentials.my_password) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}

	strcpy(p_self->info.credentials.my_password, (p_cmd->argv[current_nr]));
	return RC_OK;
}

/**
    Parses alias,hash.
    Example: blabla.domain.com,hashahashshahah
    Action:
	-search by ',' and replace the ',' with 0
	-read hash and alias
*/
static RC_TYPE get_alias_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
        char *p_hash = NULL;

	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (p_self->alias_info.count >= DYNDNS_MAX_ALIAS_NUMBER)
	{
		return RC_DYNDNS_TOO_MANY_ALIASES;
	}

	/*hash*/
        p_hash = strstr(p_cmd->argv[current_nr],",");
        if (p_hash)
	{
	    if (sizeof(*p_self->alias_info.hashes) < strlen(p_hash))
	    {
		return RC_DYNDNS_BUFFER_TOO_SMALL;
	    }
	    strcpy(p_self->alias_info.hashes[p_self->alias_info.count].str, p_hash);
	    *p_hash = '\0';
	}


	/*user*/
	if (sizeof(p_self->alias_info.names[p_self->alias_info.count]) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
	strcpy(p_self->alias_info.names[p_self->alias_info.count].name, (p_cmd->argv[current_nr]));

	p_self->alias_info.count ++;
	return RC_OK;
}

static RC_TYPE get_name_and_port(char *p_src, char *p_dest_name, int *p_dest_port)
{
    const char *p_port = NULL;
    p_port = strstr(p_src,":");
    if (p_port)
    {
        int port_nr, len;
        int port_ok = sscanf(p_port + 1, "%d",&port_nr);
        if (port_ok != 1)
        {
            return RC_DYNDNS_INVALID_OPTION;
        }
        *p_dest_port = port_nr;
        len = p_port - p_src;
        memcpy(p_dest_name, p_src, len);
        p_dest_name[len] = 0;
    }
    else
    {
        strcpy(p_dest_name, p_src);
    }
    return RC_OK;
}

/** Returns the svr name and port if the format is :
 * name[:port] url.
 */
static RC_TYPE get_ip_server_name_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
    RC_TYPE rc;
    int port = -1;

	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	/*user*/
	if (sizeof(p_self->info.ip_server_name) < strlen(p_cmd->argv[current_nr]) + 1)
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}

    p_self->info.ip_server_name.port = HTTP_DEFAULT_PORT;
    rc = get_name_and_port(p_cmd->argv[current_nr], p_self->info.ip_server_name.name, &port);
    if (rc == RC_OK && port != -1)
    {
        p_self->info.ip_server_name.port = port;
    }        

	if (sizeof(p_self->info.ip_server_url) < strlen(p_cmd->argv[current_nr + 1]) + 1)
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
	strcpy(p_self->info.ip_server_url, p_cmd->argv[current_nr + 1]);

	return rc;
}

static RC_TYPE get_dns_server_name_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
    RC_TYPE rc;
    int port = -1;

	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	/*user*/
	if (sizeof(p_self->info.dyndns_server_name) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
    
    p_self->info.dyndns_server_name.port = HTTP_DEFAULT_PORT;
    rc = get_name_and_port(p_cmd->argv[current_nr], p_self->info.dyndns_server_name.name, &port);
    if (rc == RC_OK && port != -1)
    {
        p_self->info.dyndns_server_name.port = port;
    }                                   
	return rc;
}

RC_TYPE get_dns_server_url_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	/*name*/
	if (sizeof(p_self->info.dyndns_server_url) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
	strcpy(p_self->info.dyndns_server_url, p_cmd->argv[current_nr]);
	return RC_OK;
}

/* returns the proxy server nme and port
*/
static RC_TYPE get_proxy_server_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
    RC_TYPE rc;
    int port = -1;

	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	/*user*/
	if (sizeof(p_self->info.proxy_server_name) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
    
    p_self->info.proxy_server_name.port = HTTP_DEFAULT_PORT;
    rc = get_name_and_port(p_cmd->argv[current_nr], p_self->info.proxy_server_name.name, &port);
    if (rc == RC_OK && port != -1)
    {
        p_self->info.proxy_server_name.port = port;
    }                                   
	return rc;    
}

/* Read the dyndnds name update period.
   and impose the max and min limits
*/
static RC_TYPE get_update_period_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sscanf(p_cmd->argv[current_nr], "%d", &p_self->sleep_sec) != 1)
	{
		return RC_DYNDNS_INVALID_OPTION;
	}
	
	p_self->sleep_sec /= 1000;
	p_self->sleep_sec = (p_self->sleep_sec < DYNDNS_MIN_SLEEP) ?  DYNDNS_MIN_SLEEP: p_self->sleep_sec;
	(p_self->sleep_sec > DYNDNS_MAX_SLEEP) ?  p_self->sleep_sec = DYNDNS_MAX_SLEEP: 1;	

	return RC_OK;
}

static RC_TYPE get_update_period_sec_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sscanf(p_cmd->argv[current_nr], "%d", &p_self->sleep_sec) != 1)
	{
		return RC_DYNDNS_INVALID_OPTION;
	}
	
	p_self->sleep_sec = (p_self->sleep_sec < DYNDNS_MIN_SLEEP) ?  DYNDNS_MIN_SLEEP: p_self->sleep_sec;
	(p_self->sleep_sec > DYNDNS_MAX_SLEEP) ?  p_self->sleep_sec = DYNDNS_MAX_SLEEP: 1;	

	return RC_OK;
}

static RC_TYPE get_forced_update_period_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sscanf(p_cmd->argv[current_nr], "%d", &p_self->forced_update_period_sec) != 1 ||
	    sscanf(p_cmd->argv[current_nr], "%d", &p_self->forced_update_period_sec_orig) != 1)
	{
		return RC_DYNDNS_INVALID_OPTION;
	}
	
	return RC_OK;
}

static RC_TYPE set_syslog_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}
	p_self->debug_to_syslog = TRUE;
	return RC_OK;
}

/**
 * Reads the params for change persona. Format:
 * <uid[:gid]>
 */
static RC_TYPE set_change_persona_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	{
		int gid = -1;
		int uid = -1;

		char *p_gid = strstr(p_cmd->argv[current_nr],":");
		if (p_gid)
		{
			if ((strlen(p_gid + 1) > 0) &&  /* if something is present after :*/
				sscanf(p_gid + 1, "%d",&gid) != 1)
			{
				return RC_DYNDNS_INVALID_OPTION;
			}
		}
		if (sscanf(p_cmd->argv[current_nr], "%d",&uid) != 1)
		{
			return RC_DYNDNS_INVALID_OPTION;
		}

		p_self->change_persona = TRUE;
		p_self->sys_usr_info.gid = gid;
		p_self->sys_usr_info.uid = uid;
	}
	return RC_OK;
}

RC_TYPE print_version_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	DBG_PRINTF((LOG_SYSTEM,"Version: %s\n", DYNDNS_VERSION_STRING));
	p_self->abort = TRUE;
	return RC_OK;
}

static RC_TYPE get_exec_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sizeof(p_self->external_command) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}
	strcpy(p_self->external_command, p_cmd->argv[current_nr]);
	return RC_OK;
}

static RC_TYPE get_cache_dir(CMD_DATA *p_cmd, int current_nr, void *p_context)
{

#ifdef _WIN32
	#define dir_separator	"\\"
#else
	#define dir_separator	"/"
#endif


	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}

	if (sizeof(p_self->time_cache) < strlen(p_cmd->argv[current_nr]))
	{
		return  RC_DYNDNS_BUFFER_TOO_SMALL;
	}

	sprintf(p_self->ip_cache, "%s" dir_separator "%s", p_cmd->argv[current_nr], DYNDNS_DEFAULT_IP_FILE);
	sprintf(p_self->time_cache, "%s" dir_separator "%s", p_cmd->argv[current_nr], DYNDNS_DEFAULT_TIME_FILE);


	return RC_OK;
}

/** 
    Searches the DYNDNS system by the argument.
    Input is like: system@server.name
    system=statdns|custom|dyndns|default
    server name = dyndns.org | freedns.afraid.org
    The result is a pointer in the table of DNS systems.
*/
static RC_TYPE get_dyndns_system_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
    DYNDNS_SYSTEM *p_dns_system = NULL;
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;
	if (p_self == NULL)
	{
		return RC_INVALID_POINTER;
	}
    
    {
        DYNDNS_SYSTEM_INFO *it = get_dyndns_system_table();
        for (; it != NULL && it->id != LAST_DNS_SYSTEM; ++it)
        {
            if (strcmp(it->system.p_key, p_cmd->argv[current_nr]) == 0)
            {
                p_dns_system = &it->system;
            }
        }    
    }

    if (p_dns_system == NULL)
    {
        return RC_DYNDNS_INVALID_OPTION;
    }

    p_self->info.p_dns_system = p_dns_system;
	
	return RC_OK;
}

static RC_TYPE get_debug_level_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{

  #define ASCII_ZERO     48

  int                    dwLevel=0;


  DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;

  if (p_self == NULL)
  {
    return RC_INVALID_POINTER;
  }

  dwLevel=*(p_cmd->argv[current_nr])-ASCII_ZERO;

  if (dwLevel<LOG_EMERG || dwLevel>LOG_DEBUG)

    return RC_DYNDNS_INVALID_OPTION;

  p_self->dbg.level=dwLevel;

  return RC_OK;
}

static RC_TYPE get_retries_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	int	retries=0;


	DYN_DNS_CLIENT *p_self=(DYN_DNS_CLIENT *) p_context;


	if (p_self==NULL)

		return RC_INVALID_POINTER;


	if (sscanf(p_cmd->argv[current_nr], "%d",&retries) != 1)

		return RC_DYNDNS_INVALID_OPTION;


	if (retries<0 || retries>100)

		return RC_DYNDNS_INVALID_OPTION;


	p_self->net_retries=retries;


	return RC_OK;
}

static RC_TYPE get_retry_interval_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{
	int	retry_interval=0;


	DYN_DNS_CLIENT *p_self=(DYN_DNS_CLIENT *) p_context;


	if (p_self==NULL)

		return RC_INVALID_POINTER;


	if (sscanf(p_cmd->argv[current_nr], "%d",&retry_interval) != 1)

		return RC_DYNDNS_INVALID_OPTION;


	if (retry_interval<0 || retry_interval>30000)

		return RC_DYNDNS_INVALID_OPTION;


	p_self->net_retries=retry_interval;


	return RC_OK;
}

static RC_TYPE get_lang_file_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{

	DYN_DNS_CLIENT *p_self=(DYN_DNS_CLIENT *) p_context;


	if (p_self==NULL)

		return RC_INVALID_POINTER;


	if (!(is_file(p_cmd->argv[current_nr]))) { /*ignore if invalid*/

		DBG_PRINTF((LOG_WARNING,"W:" MODULE_TAG "Could not open default override language strings file, %s...\n",p_cmd->argv[current_nr]));

		return RC_OK;
	}

#ifdef _WIN32

	nt_console_name2(&(p_self->lang_file),p_cmd->argv[current_nr]);

#else

	p_self->lang_file=safe_malloc(strlen(p_cmd->argv[current_nr])+1);

	strcpy(p_self->lang_file,p_cmd->argv[current_nr]);

#endif


	return RC_OK;
}

static RC_TYPE push_in_buffer(char* p_src, int src_len, char *p_buffer, int* p_act_len, int max_len)
{
    if (*p_act_len + src_len > max_len)
    {
        return RC_FILE_IO_OUT_OF_BUFFER;
    }
    memcpy(p_buffer + *p_act_len,p_src, src_len);
    *p_act_len += src_len;
    return RC_OK;
}

typedef enum 
{
    NEW_LINE,
    COMMENT,
    DATA,
    SPACE,
    ESCAPE
} PARSER_STATE;

typedef struct 
{
    FILE *p_file;
    PARSER_STATE state;
} OPTION_FILE_PARSER;

typedef RC_TYPE (*UTF_PARSE_FUNC)(OPTION_FILE_PARSER *p_cfg, char *p_buffer, int maxlen);

static RC_TYPE parser_init(OPTION_FILE_PARSER *p_cfg, FILE *p_file)
{
    memset(p_cfg, 0, sizeof(*p_cfg));
    p_cfg->state = NEW_LINE;
    p_cfg->p_file = p_file;
    return RC_OK;
}

static RC_TYPE do_parser_read_option(OPTION_FILE_PARSER *p_cfg,
										char *p_buffer,
										int maxlen,
										char *ch,
										int *count,
										BOOL *parse_end)
{

	RC_TYPE rc = RC_OK;	


	switch (p_cfg->state)
	{
		case NEW_LINE:

			if (!(strcmp(ch,"\n")) || !(strcmp(ch,"\r")))
			{
				return rc;
			}

			if (!(strcmp(ch,"\\")))
			{
				p_cfg->state = ESCAPE;

				return rc;
			}

			if (!(strcmp(ch,"#"))) /*comment*/
			{
				p_cfg->state = COMMENT;

				return rc;
			}

			if ((strcmp(ch," ")) && (strcmp(ch,"	")))
			{
				if (strcmp(ch,"-"))   /*add '--' to first word in line*/
				{
					if ((rc = push_in_buffer("--", 2, p_buffer, count, maxlen)) != RC_OK)
					{
						return rc;
					}
				}
				if ((rc = push_in_buffer(ch, strlen(ch), p_buffer, count, maxlen)) != RC_OK)
				{
					return rc;
				}

				p_cfg->state = DATA;

				return rc;
			}

			/*skip actual leading  spaces*/
            return rc;

		case SPACE:

			if (!(strcmp(ch,"\\")))
			{
				p_cfg->state = ESCAPE;

				return rc;
			}

			if (!(strcmp(ch,"#"))) /*comment*/
			{
				p_cfg->state = COMMENT;

				return rc;
			}

			if (!(strcmp(ch,"\n")) || !(strcmp(ch,"\r")))
			{
				p_cfg->state = NEW_LINE;

				return rc;
			}

			if ((strcmp(ch," ")) && (strcmp(ch,"	")))
			{
				if ((rc = push_in_buffer(ch, strlen(ch), p_buffer, count, maxlen)) != RC_OK)
				{
					return rc;
				}
				p_cfg->state = DATA;

				return rc;
			}

			return rc;

		case COMMENT:

			if (!(strcmp(ch,"\n")) || !(strcmp(ch,"\r")))
			{
				p_cfg->state = NEW_LINE;
			}

			/*skip comments*/
			return rc;
            
		case DATA:

			if (!(strcmp(ch,"\\")))
			{
				p_cfg->state = ESCAPE;

				return rc;
			}

			if (!(strcmp(ch,"#")))
			{
				p_cfg->state = COMMENT;

				return rc;
			}

			if (!(strcmp(ch,"\n")) || !(strcmp(ch,"\r")))
			{

				p_cfg->state = NEW_LINE;
				*parse_end = TRUE;

				return rc;
			}

			if (!(strcmp(ch," ")) || !(strcmp(ch,"	")))
			{
				p_cfg->state = SPACE;
				*parse_end = TRUE;

				return rc;
			}

			/*actual data*/

			if ((rc = push_in_buffer(ch, strlen(ch), p_buffer, count, maxlen)) != RC_OK)
			{
				return rc;
			}

			return rc;

		case ESCAPE:

			if ((rc = push_in_buffer(ch, strlen(ch), p_buffer, count, maxlen)) != RC_OK)
			{
				return rc;
			}

			p_cfg->state = DATA;

			return rc;

			default:

			rc = RC_CMD_PARSER_INVALID_OPTION; 
	}

	return rc;
}

/** Read one single option from utf-16 file into the given buffer.
	When the first separator is encountered it returns.
	Actions:
		- read chars while not eof
		- skip comments (parts beginning with '#' and ending with '\n')
		- switch to DATA STATE if non space char is encountered
		- assume first name in lines to be a long option name by adding '--' if necesssary
		- add data to buffer
		- do not forget a 0 at the end
 * States:
 * NEW_LINE - wait here until some option. Add '--' if not already there
 * SPACE - between options. Like NEW_LINE but no additions
 * DATA - real data. Stop on space.
 * COMMENT - everything beginning with # until EOLine
 * ESCAPE - everything that is otherwise (incl. spaces). Next char is raw copied.
*/
static RC_TYPE parser_utf_8_read_option(OPTION_FILE_PARSER *p_cfg, char *p_buffer, int maxlen)
{
    RC_TYPE rc = RC_OK;
	BOOL parse_end = FALSE;
    int count = 0;	
    char c_buff[7];


    *p_buffer = 0;	

   
	while(!parse_end)		
	{

		memset(c_buff,0,7);		

		if (!(utf_read_utf_8(c_buff,p_cfg->p_file)))
		{
			if (feof(p_cfg->p_file))
			{
				break;
			}

			rc = RC_FILE_IO_READ_ERROR;		

			break;
		}

		rc=do_parser_read_option(p_cfg,p_buffer,maxlen,c_buff,&count,&parse_end);

		if (!(rc == RC_OK))

			return rc;
	}


	rc = push_in_buffer("\0",1,p_buffer,&count,maxlen);


    return rc;
}

/** Read one single option from utf-16 file into the given buffer.
	When the first separator is encountered it returns.
	Actions:
		- read chars while not eof
		- skip comments (parts beginning with '#' and ending with '\n')
		- switch to DATA STATE if non space char is encountered
		- assume first name in lines to be a long option name by adding '--' if necesssary
		- add data to buffer
		- do not forget a 0 at the end
 * States:
 * NEW_LINE - wait here until some option. Add '--' if not already there
 * SPACE - between options. Like NEW_LINE but no additions
 * DATA - real data. Stop on space.
 * COMMENT - everything beginning with # until EOLine
 * ESCAPE - everything that is otherwise (incl. spaces). Next char is raw copied.
*/

#ifdef _WIN32

/** Read one single option from utf-16 file into the given buffer.
	When the first separator is encountered it returns.
	Actions:
		- read chars while not eof
		- skip comments (parts beginning with '#' and ending with '\n')
		- switch to DATA STATE if non space char is encountered
		- assume first name in lines to be a long option name by adding '--' if necesssary
		- add data to buffer
		- do not forget a 0 at the end
 * States:
 * NEW_LINE - wait here until some option. Add '--' if not already there
 * SPACE - between options. Like NEW_LINE but no additions
 * DATA - real data. Stop on space.
 * COMMENT - everything beginning with # until EOLine
 * ESCAPE - everything that is otherwise (incl. spaces). Next char is raw copied.
*/
static RC_TYPE parser_utf_16_read_option(OPTION_FILE_PARSER *p_cfg, char *p_buffer, int maxlen)
{
    RC_TYPE rc = RC_OK;
	BOOL parse_end = FALSE;
    int count = 0;
	char utf_8[18];
    char c_buff[4]={0,0,0,0};

    *p_buffer = 0;	

 
	while(!parse_end)
		
	{
		if (feof(p_cfg->p_file))
		{
			break;
		}

		c_buff[0]=fgetc(p_cfg->p_file);

		if ((c_buff[0]==EOF))
		{
			rc = RC_FILE_IO_READ_ERROR;		

			break;
		}

		if (feof(p_cfg->p_file))
		{
			rc = RC_FILE_IO_READ_ERROR;

			break;
		}

		c_buff[1]=fgetc(p_cfg->p_file);

		if ((c_buff[1]==EOF))
		{
			rc = RC_FILE_IO_READ_ERROR;		

			break;
		}

		/*convert to utf-8, and pass to parser*/

		rc=do_parser_read_option(p_cfg,p_buffer,maxlen,utf_16_to_8(utf_8,(wchar_t *) c_buff),&count,&parse_end);

		if (!(rc == RC_OK))

			return rc;		
	}


	{	
		char ch = 0;
		rc = push_in_buffer(&ch, 1, p_buffer, &count, maxlen);		
	}

    return rc;
}

#endif

static RC_TYPE do_get_options_from_file(CMD_DATA *p_cmd,int current_nr,void *p_context)
{

	RC_TYPE rc = RC_OK;	
	FILE *p_file = NULL;
	char *p_tmp_buffer = NULL;
	const int buffer_size = DYNDNS_SERVER_NAME_LENGTH;
	OPTION_FILE_PARSER parser;

	UTF_PARSE_FUNC parse_func=parser_utf_8_read_option;

#ifdef _WIN32

	int		is_bom=0;	/*win32 utf byte order mark?*/
	int		is_bom_8=0;
#endif


  do 
  {
		p_tmp_buffer = safe_malloc(buffer_size);
		if (!p_tmp_buffer)
		{
 			rc = RC_OUT_OF_MEMORY;
 			break;
  		}

#ifndef _WIN32

  		p_file = fopen(p_cmd->argv[current_nr], "r");
#else

		if (!(isWinNT()))

  #ifndef UNICOWS

			p_file = fopen(p_cmd->argv[current_nr], "r");

  #else

			;

  #endif

  #ifndef UNICOWS

		else

  #endif
  

		{

			/*Win NT, or Win32s with UNICOWS defined*/


			wchar_t *utf_16;


			utf_8_to_16(utf_malloc_8_to_16(&utf_16,p_cmd->argv[current_nr]),p_cmd->argv[current_nr]);


			if (!(utf_is_win_utf_file(utf_16,&is_bom)))

				is_bom_8=is_bom;				

			else

				parse_func=parser_utf_16_read_option;


			p_file = _wfopen(utf_16, L"r");


			free(utf_16);
		}

#endif
  		if (!(p_file))

  		{
			DBG_PRINTF((LOG_ERR,"W:" MODULE_TAG "Cannot open cfg file:%s\n", p_cmd->argv[current_nr]));
  			rc = RC_FILE_IO_OPEN_ERROR;
  			break;
  		}

#ifdef _WIN32

		if (is_bom_8)
		
			fseek(p_file,3,0);

		else

			if (is_bom)

				fseek(p_file,2,0);

#endif

		if ((rc = parser_init(&parser, p_file)) != RC_OK)
		{
			break;
		}

   		while(!feof(p_file))
		{		 		
			rc = parse_func(&parser,p_tmp_buffer, buffer_size);

	 		if (rc != RC_OK)
	 		{
	 			break;
	 		}

			if (!strlen(p_tmp_buffer))
			{
				break;
			}

	 		rc = cmd_add_val(p_cmd, p_tmp_buffer);
	 		if (rc != RC_OK)
	 		{
	 			break;
	 		}
   		} 	
 
  }
  while(0);
  

  if (p_file)
  {
	fclose(p_file);
  }
  if (p_tmp_buffer)
  {
	free(p_tmp_buffer);
  }

  return rc;
}

/**
	This handler reads the data in the passed file name.
	Then appends the words in the table (cutting all spaces) to the existing cmd line options.
	It adds to the CMD_DATA struct.
	Actions:
		- open file
		- read characters and cut spaces away
		- add values one by one to the existing p_cmd data
*/
static RC_TYPE get_options_from_file_handler(CMD_DATA *p_cmd, int current_nr, void *p_context)
{

	RC_TYPE		rc=RC_OK;
	int			curr_argc=0;
	int			i;
	int			destIndex;

	char		**arg_list;

	
	DYN_DNS_CLIENT *p_self = (DYN_DNS_CLIENT *) p_context;    
	
	if (!p_self || !p_cmd)
	{
		return RC_INVALID_POINTER;
	}

	curr_argc=p_cmd->argc;

	rc=do_get_options_from_file(p_cmd,current_nr,p_context);

	if (!(rc==RC_OK))

		return rc;

	/*fix up so file arguments have been inserted after current_nr*/

	arg_list=safe_malloc(p_cmd->argc*sizeof(char *));

	current_nr++;

	for (i=0;i<current_nr;i++) {

		arg_list[i]=safe_malloc(strlen(p_cmd->argv[i])+1);

		strcpy(arg_list[i],p_cmd->argv[i]);

		free(p_cmd->argv[i]);
	}

	for (i=curr_argc,destIndex=current_nr;i<p_cmd->argc;i++,destIndex++) {

		arg_list[destIndex]=safe_malloc(strlen(p_cmd->argv[i])+1);

		strcpy(arg_list[destIndex],p_cmd->argv[i]);

		free(p_cmd->argv[i]);
	}

	for (i=current_nr;i<curr_argc;i++,destIndex++) {

		arg_list[destIndex]=safe_malloc(strlen(p_cmd->argv[i])+1);

		strcpy(arg_list[destIndex],p_cmd->argv[i]);

		free(p_cmd->argv[i]);
	}

	free(p_cmd->argv);

	p_cmd->argv=arg_list;


	return rc;
}

RC_TYPE do_get_default_infile_config(char *in_file,DYN_DNS_CLIENT *p_self,CMD_OPTION_ERR_HANDLER_FUNC pf_err_handler)
{

	RC_TYPE		rc = RC_OK;
	int			custom_argc = 3;


	char **custom_argv = safe_malloc(sizeof(char **)*3);


	custom_argv[0] = safe_malloc(2);
	strcpy(custom_argv[0],"");

	custom_argv[1] = safe_malloc(strlen(DYNDNS_INPUT_FILE_OPT_STRING)+1);
	strcpy(custom_argv[1],DYNDNS_INPUT_FILE_OPT_STRING);

	custom_argv[2] = safe_malloc(strlen(in_file)+1);
	strcpy(custom_argv[2],in_file);


	DBG_PRINTF((LOG_INFO,"I:" MODULE_TAG "Attempting using default config file %s\n", in_file));	

	
	rc = get_cmd_parse_data_with_error_handling(custom_argv, custom_argc, cmd_options_table,pf_err_handler);

	free(custom_argv[0]);
	free(custom_argv[1]);
	free(custom_argv[2]);

	free(custom_argv);


	return rc;
}

/*
	try inadyn config file first
	and if it's not there, try mt 
	config file
*/
RC_TYPE get_default_infile_config(DYN_DNS_CLIENT *p_self,CMD_OPTION_ERR_HANDLER_FUNC pf_err_handler)
{

	RC_TYPE rc = RC_OK;


	rc = do_get_default_infile_config(DYNDNS_DEFAULT_CONFIG_FILE,p_self,pf_err_handler);


	if (!(rc==RC_OK)) {

		rc = do_get_default_infile_config(DYNDNS_MT_DEFAULT_CONFIG_FILE,p_self,pf_err_handler);
	}


	return rc;
}

/* 
	Set up all details:
		- ip server name
		- dns server name
		- username, passwd
		- ...
	Implementation:
		- load defaults
		- parse cmd line
        - assign settings that may change due to cmd line options
		- check data
	Note:
		- if no argument is specified tries to call the cmd line parser
	with the default cfg file path.
*/
RC_TYPE get_config_data_with_error_handling(DYN_DNS_CLIENT *p_self, int argc, char** argv,CMD_OPTION_ERR_HANDLER_FUNC pf_err_handler)
{
	RC_TYPE rc = RC_OK;
	FILE *fp;
	char cached_time[80];
	int dif;

	do
	{
		/*load default data */
		rc = get_default_config_data(p_self);
		if (rc != RC_OK)
		{	
			break;
		}
		/*set up the context pointers */
		{
			CMD_DESCRIPTION_TYPE *it = cmd_options_table;			
			while( it->p_option != NULL)
			{
				it->p_handler.p_context = (void*) p_self;
				++it;
			}
		}
		/* in case of no options, assume the default cfg file may be present */
		if (argc == 1)
		{

			rc = get_default_infile_config(p_self,pf_err_handler);
		}
		else
		{
			rc = get_cmd_parse_data_with_error_handling(argv, argc, cmd_options_table,pf_err_handler);
		}

		if (rc != RC_OK ||
			p_self->abort)
		{
			break;
		}	

        /*settings that may change due to cmd line options*/
        {
    		/*ip server*/
            if (strlen(p_self->info.ip_server_name.name) == 0)
            {
                if (sizeof(p_self->info.ip_server_name.name) < strlen(p_self->info.p_dns_system->p_ip_server_name))
                {
                    rc = RC_DYNDNS_BUFFER_TOO_SMALL;
                    break;
                }
                strcpy(p_self->info.ip_server_name.name, p_self->info.p_dns_system->p_ip_server_name);

                if (sizeof(p_self->info.ip_server_url) < strlen(p_self->info.p_dns_system->p_ip_server_url))
                {
                    rc = RC_DYNDNS_BUFFER_TOO_SMALL;
                    break;
                }
                strcpy(p_self->info.ip_server_url, p_self->info.p_dns_system->p_ip_server_url);				
            }

    		/*dyndns server*/
            if (strlen(p_self->info.dyndns_server_name.name) == 0)
            {
        		if (sizeof(p_self->info.dyndns_server_name.name) < strlen(p_self->info.p_dns_system->p_dyndns_server_name))
        		{
        			rc = RC_DYNDNS_BUFFER_TOO_SMALL;
        			break;
        		}
        		strcpy(p_self->info.dyndns_server_name.name, p_self->info.p_dns_system->p_dyndns_server_name);

        		if (sizeof(p_self->info.dyndns_server_url) < strlen(p_self->info.p_dns_system->p_dyndns_server_url))
        		{
        			rc = RC_DYNDNS_BUFFER_TOO_SMALL;
        			break;
        		}
        		strcpy(p_self->info.dyndns_server_url, p_self->info.p_dns_system->p_dyndns_server_url);
            }
        }

		/*check if the neccessary params have been provided*/
		if ( 
			(strlen(p_self->info.dyndns_server_name.name) == 0)  ||
			(strlen(p_self->info.ip_server_name.name)	  == 0)  ||
			(p_self->alias_info.count == 0)
			)
		{
			rc = RC_DYNDNS_INVALID_OR_MISSING_PARAMETERS;
			break;
		}
		/*forced update*/
		if ((fp=fopen(p_self->time_cache, "r")))
		{
			fgets (cached_time, sizeof (cached_time), fp);
			fclose(fp);
			dif = time(NULL) - atoi(cached_time);
			p_self->forced_update_period_sec -= dif;
		}
		p_self->times_since_last_update = 0;
		p_self->forced_update_times = p_self->forced_update_period_sec / p_self->sleep_sec;

	}
	while(0);

	return rc;
}

RC_TYPE get_config_data(DYN_DNS_CLIENT *p_self, int argc, char** argv)
{

  return get_config_data_with_error_handling(p_self,argc,argv,NULL);
}
