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

#include "howl_config.h"
#include "../autoip.h"
#include <salt/interface.h>
#include <salt/signal.h>
#include <salt/debug.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

/*
 *	globals
 */
static sw_bool								g_make_daemon = SW_TRUE;
static sw_salt								g_salt;
static sw_network_interface			g_nif;
static sw_autoip_network_interface	g_anif;
static sw_signal							g_sighup;
static sw_signal							g_sigint;
static sw_signal							g_sigusr1;
static sw_signal							g_sigusr2;
static sw_int8								g_ifname[256];
static int									g_pid_fd;
static sw_saddr							g_ipaddr = 0;

/* 
 * Callback functions 
 */
static sw_result
autoipd_sighup_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
autoipd_sigint_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
autoipd_sigusr1_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
autoipd_sigusr2_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
autoipd_make_daemon();


static sw_result
autoip_write_pidfile(char * dname)
{
	char 			pidfile[64];
	char 			str[16];
	int			res;
	sw_result 	err = SW_OKAY;

	umask(022);
	sprintf(pidfile, "/var/run/%s.pid", dname);

	/*
	 * open the file
	 */
	g_pid_fd = open(pidfile, O_RDWR|O_CREAT, 0644);
	err = sw_translate_error(g_pid_fd != -1, errno);
	sw_check_okay_log(err, exit);
			
	/*
	 * lock it
	 */
	res = lockf(g_pid_fd, F_TLOCK, 0);
	err = sw_translate_error(res == 0, errno);
	sw_check_okay_log(err, exit);
	
	/*
	 * write our pid
	 */
	sprintf(str, "%d\n", getpid());
	write(g_pid_fd, str, strlen(str));

exit:

	return err;
}


static sw_result
autoipd_sighup_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra)
{
	sw_autoip_network_interface_restart_probing(g_anif);
	return SW_OKAY;
}


static sw_result
autoipd_sigint_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra)
{
	return sw_salt_stop_run(salt);
}


static sw_result
autoipd_sigusr1_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra)
{
	sw_autoip_network_interface_restart_probing(g_anif);
	return SW_OKAY;
}


static sw_result
autoipd_sigusr2_command(
         sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra)
{
	sw_autoip_network_interface_restart_probing(g_anif);
	return SW_OKAY;
}


static sw_result
autoip_make_daemon()
{
	sw_result err = SW_OKAY;

	if (g_make_daemon)
	{
		int res;

		res = daemon(SW_FALSE, SW_FALSE);
		err = sw_translate_error(res == 0, errno);
		sw_check_okay_log(err, exit);
	}

exit:

	return err;
}


static void
print_usage(const char * program)
{
	fprintf(stderr, "usage: %s [options]\n\n", program);
	fprintf(stderr, "Options:\n\n");
	fprintf(stderr, "-h          this help\n");
	fprintf(stderr, "-v          display version information\n");
	fprintf(stderr, "-d          run in debug mode\n");
	fprintf(stderr, "-i ifname   use interface ifname\n");
	fprintf(stderr, "-s addr     start at IP address addr\n");
	fprintf(stderr, "-m addr     use this mac address\n\n");
	exit(0);
}


static void
set_mac(const char * s)
{
	static char filler[7] = "012345";

	if (strlen(s) < strlen(filler))
	{
		filler[strlen(filler) - strlen(s)] = 0;
		strcat(filler, s);
	}
	else
	{
		strncpy(filler, s, strlen(filler));
	}

	sw_platform_autoip_network_interface_set_mac_address(filler);
}


sw_saddr
sw_platform_autoip_network_interface_initial_address()
{
	return g_ipaddr;
}


int
main(int argc, char ** argv)
{
	int									result;
	int									opt;
	char 									pidname[32];
	sw_result							err = SW_OKAY;

	/*
	 * default the interface to eth0
	 */
	strcpy(g_ifname, sw_platform_autoip_network_interface_default_interface_name());

	/*
	 * parse command line
	 */
	while ((opt = getopt(argc, argv, "?hvdi:s:m:")) != EOF)
	{
		switch (opt)
		{
			case '?':
			case 'h':
			{
				print_usage(argv[0]);
			}
			break;

			case 'v':
			{
				fprintf(stderr, "autoipd release " VERSION " Copyright(c) Porchdog Software 2004-2005.\n");
				exit(0);
			}
			break;

			case 'd':
			{
				g_make_daemon = SW_FALSE;
			}
			break;

			case 'i':
			{
				sw_strcpy(g_ifname, optarg);
			}
			break;

			case 's':
			{
				g_ipaddr = inet_addr(optarg);

				if (g_ipaddr == INADDR_NONE)
				{
					sw_debug(SW_LOG_WARNING, "bad name: %s\n", optarg);
				}
			}
			break;

			case 'm':
			{
				set_mac(optarg);
			}
			break;

			default:
			{
				print_usage(argv[0]);
			}
			break;
		}
	}

	/*
	 * must be run as root
	 */
	if (getuid() != 0)
	{
		fprintf(stderr, "autoipd must be run as root\n");
		exit(1);
	}

	if (g_make_daemon)
	{
		err = autoip_make_daemon();
		sw_check_okay(err, exit);
	}

	/*
	 * initialize salt
	 */
	err = sw_salt_init(&g_salt, 0, NULL);
	sw_check_okay(err, exit);

	/*
	 * initialize signal handling
	 */

	err = sw_signal_init(&g_sighup, SIGHUP);
	sw_check_okay(err, exit);
	err = sw_salt_register_signal(g_salt, g_sighup, (sw_signal_handler) NULL, autoipd_sighup_command, NULL);
	sw_check_okay(err, exit);

	err = sw_signal_init(&g_sigint, SIGINT);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(g_salt, g_sigint, (sw_signal_handler) NULL, autoipd_sigint_command, NULL);
	sw_check_okay(err, exit);

	err = sw_signal_init(&g_sigusr1, SIGUSR1);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(g_salt, g_sigusr1, (sw_signal_handler) NULL, autoipd_sigusr1_command, NULL);
	sw_check_okay(err, exit);

	err = sw_signal_init(&g_sigusr2, SIGUSR2);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(g_salt, g_sigusr2, (sw_signal_handler) NULL, autoipd_sigusr2_command, NULL);
	sw_check_okay(err, exit);

	/*
	 * find the correct interface
	 */
	sw_network_interface_by_name(g_ifname, &g_nif);

	/*
	 * create the pid file
	 */
	sprintf(pidname, "autoipd-%s", g_ifname);
	err = autoip_write_pidfile(pidname);
	sw_check_okay(err, exit);

	err = sw_platform_autoip_network_interface_new(&g_anif, g_salt, g_nif);
	sw_check_okay(err, exit);

	sw_debug(SW_LOG_NOTICE, "autoipd starting up...\n");

	err = sw_salt_run(g_salt);
	sw_check_okay(err, exit);

	sw_debug(SW_LOG_NOTICE, "autoipd shutting down...\n");

	sw_platform_autoip_network_interface_delete(g_anif);

	sw_salt_unregister_signal(g_salt, g_sighup);
	sw_signal_fina(g_sighup);
	sw_salt_unregister_signal(g_salt, g_sigint);
	sw_signal_fina(g_sigint);
	sw_salt_unregister_signal(g_salt, g_sigusr1);
	sw_signal_fina(g_sigusr1);
	sw_salt_unregister_signal(g_salt, g_sigusr2);
	sw_signal_fina(g_sigusr2);

	sw_salt_fina(g_salt);

exit: 

	return 0;
}
