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
#include <stdio.h>
#include <fcntl.h>
#include <nifd.h>
#include <time.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <salt/interface.h>
#include <salt/signal.h>
#include <salt/debug.h>

/* 
 * globals
 */
static sw_bool restart_interfaces = SW_TRUE;

/* 
 * Callback functions 
 */
static sw_result
timer_event_handler(
		sw_timer_handler     handler,
		sw_salt           	salt,
		sw_timer          	timer,
		sw_time              timeout,
		sw_opaque            extra);


static sw_result
sighup_command(
			sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
sigusr1_command(
			sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
sigusr2_command(
			sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


static sw_result
sigint_command(
			sw_signal_handler	handler,
         sw_salt        	salt,
         sw_signal      	signal,
         sw_opaque      	extra);


/*
 * nifd methods
 */
static sw_result
nifd_write_pidfile(nifd * self, char * dname)
{
	char 			pidfile[64];
	char 			str[16];
	int			res;
	sw_result 	err = SW_OKAY;

	umask(022);
	sprintf(pidfile, "/var/run/%s.pid", dname);

	/* open the file */
	self->m_pidfd = open(pidfile, O_RDWR|O_CREAT, 0644);
	err = sw_translate_error(self->m_pidfd != -1, errno);
	sw_check_okay_log(err, exit);
			
	/* lock it */
	res = lockf(self->m_pidfd, F_TLOCK,0);
	err = sw_translate_error(res == 0, errno);
	sw_check_okay_log(err, exit);
	
	/* write our pid */
	sprintf(str, "%d\n", getpid());
	write(self->m_pidfd, str, strlen(str)); // record pid to lockfile

exit:

	return err;
}


static sw_result
nifd_read_pidfile(nifd self, char * dname, int * pid)
{
	char 			pidfile[64];
	FILE 		*	fp  = NULL;
	int			res;
	sw_result 	err = SW_OKAY;

	sprintf(pidfile, "/var/run/%s.pid", dname);
	fp = fopen(pidfile, "rt");
	sw_check(fp, exit, err = SW_E_UNKNOWN);

	res = fscanf(fp, "%d", pid);
	err = sw_translate_error(res != -1, errno);
	sw_check_okay_log(err, exit);

exit:

	if (fp)
	{
		fclose(fp);
	}

	return err;
}

/* 
 * nifd_init()
 *
 *	initialize resources
 */
sw_result
nifd_init(nifd * self, sw_uint32 interval)
{
	sw_result err = SW_OKAY;

	self->m_salt					= NULL;
	self->m_pidfd					= -1;
	self->m_timer 					= NULL;
	self->m_sigusr1 				= NULL;
	self->m_sigusr2 				= NULL;
	self->m_sighup 				= NULL;
	self->m_sigint 				= NULL;
	self->m_nifcount 				= 0;
	self->m_nifs 					= NULL;
	self->m_polltime.m_secs		= interval;
	self->m_polltime.m_usecs	= 0;

	/* create the pid file */
	err = nifd_write_pidfile(self, "nifd");
	sw_check_okay(err, exit);

	/* initialize salt */
	err = sw_salt_init(&(self->m_salt), 0, NULL);
	sw_check_okay(err, exit);

	/* get all available interfaces */
	err = sw_network_interfaces2(&self->m_nifcount, &self->m_nifs);
	sw_check_okay(err, exit);

	/* initialize the timer */
	err = sw_timer_init(&(self->m_timer));
	sw_check_okay(err, exit);

	/* register the timer */
	sw_debug(SW_LOG_VERBOSE, "nifd: installing timer ...\n");
	err = sw_salt_register_timer(self->m_salt, self->m_timer, self->m_polltime, (sw_timer_handler) self, timer_event_handler, NULL);
	sw_check_okay(err, exit);

	/* register signal handlers */
	sw_debug(SW_LOG_VERBOSE, "nifd: registering signal handlers ...\n");
	err = sw_signal_init(&self->m_sigint, SIGINT);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(self->m_salt, self->m_sigint, (sw_signal_handler) self, sigint_command, NULL);
	sw_check_okay(err, exit);

	err = sw_signal_init(&self->m_sigusr1, SIGUSR1);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(
						self->m_salt,
						self->m_sigusr1,
						(sw_signal_handler) self,
						sigusr1_command,
						NULL);
	sw_check_okay(err, exit);

	err = sw_signal_init(&self->m_sigusr2, SIGUSR2);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(
						self->m_salt,
						self->m_sigusr2,
						(sw_signal_handler) self,
						sigusr2_command,
						NULL);
	sw_check_okay(err, exit);

	err = sw_signal_init(&self->m_sighup, SIGHUP);
	sw_check_okay(err, exit);

	err = sw_salt_register_signal(
						self->m_salt,
						self->m_sighup,
						(sw_signal_handler) self,
						sighup_command,
						NULL);
	sw_check_okay(err, exit);

exit:

	return err;
}

/* 
 * nifd_fina()
 *
 *	free resources
 */
sw_result
nifd_fina(nifd * self)
{
	sw_result err = SW_OKAY;

	/* free signals */
	if (self->m_sigusr1 != NULL)
	{
		err = sw_salt_unregister_signal(self->m_salt, self->m_sigusr1);
		sw_check_okay(err, exit);

		err = sw_signal_fina(self->m_sigusr1);
		sw_check_okay(err, exit);
	}

	if (self->m_sigusr2 != NULL)
	{
		err = sw_salt_unregister_signal(self->m_salt, self->m_sigusr2);
		sw_check_okay(err, exit);

		err = sw_signal_fina(self->m_sigusr2);
		sw_check_okay(err, exit);
	}

	if (self->m_sigint != NULL)
	{
		err = sw_salt_unregister_signal(self->m_salt, self->m_sigint);
		sw_check_okay(err, exit);

		err = sw_signal_fina(self->m_sigint);
		sw_check_okay(err, exit);
	}

	if (self->m_sighup != NULL)
	{
		err = sw_salt_unregister_signal(self->m_salt, self->m_sighup);
		sw_check_okay(err, exit);

		err = sw_signal_fina(self->m_sighup);
		sw_check_okay(err, exit);
	}

	/* free available interfaces */
	if (self->m_nifs != NULL)
	{
		err = sw_network_interfaces_fina(self->m_nifcount, self->m_nifs);
		sw_check_okay(err, exit);
	}

	/* close the pid file */
	if (self->m_pidfd != -1)
	{
		close(self->m_pidfd);
	}

	/* free timer */
	if (self->m_timer != NULL)
	{
		err = sw_timer_fina(self->m_timer);
		sw_check_okay(err, exit);
	}

	/* free salt */
	if (self->m_salt != NULL)
	{
		err = sw_salt_fina(self->m_salt);
		sw_check_okay(err, exit);
	}

exit:

	return err;
}


static sw_result
nifd_signal_daemon(nifd self, sw_string dname)
{
	int			sysres;
	int 			pid;
	sw_result	err = SW_OKAY;

	err = nifd_read_pidfile(self, dname, &pid);
	sw_check_okay(err, exit);

	sw_debug(SW_LOG_VERBOSE, "sending kill SIGUSR1 to %d\n", pid);
	sysres = kill(pid, SIGUSR1);
	err = sw_translate_error(sysres == 0, errno);
	sw_check_okay_log(err, exit);

exit:

	return err;
}


static sw_result
nifd_signal_autoipd(nifd self, sw_network_interface nif)
{
	char name[32];
	char dname[32];

	/* signal autoipd */
	sw_network_interface_name(nif, name, 32);
	sprintf(dname, "autoipd-%s", name);

	return nifd_signal_daemon(self, dname);
}

/* 
 * nifd_restart_interface()
 * 
 * call ifdown and ifup on link 
 */
static sw_result
nifd_restart_interface(
	nifd 						self, 
	sw_network_interface nif)
{
	int	sysres;
	char 	ifname[32];
	char	dname[32];
	char 	cmd[128];
	char*	cmds[2] = { "ifdown", "ifup" };
	int	i;
	sw_result	err = SW_OKAY;

	sw_network_interface_name(nif, ifname, 32);

	for (i = 0; i < 2; i++)
	{
		sprintf(cmd, "/sbin/%s %s", cmds[i], ifname);
		sysres = system(cmd);
		err = sw_translate_error(sysres == 0, errno);
		sw_check_okay_log(err, exit);
	}

exit:

	return err;
}

/*
 * timer_event_handler()
 * 	called where the main loop detects that a timeout has occurred.
 */
static sw_result
timer_event_handler(
         sw_timer_handler  handler,
         sw_salt           salt,
         sw_timer       	timer,
         sw_time           timeout,
         sw_opaque         extra)
{
   nifd						*  ifmd;
	sw_uint32						newnifc;
	sw_network_interface *	newnifs;
	sw_network_interface * 	tnifs;
	sw_bool						signal_mDNS = SW_FALSE;
	int 							i;
	sw_result 					err = SW_OKAY;

   ifmd = (nifd*) handler;

	sw_assert(ifmd != NULL);
   sw_assert(ifmd->m_timer == timer);

	/* reinstall one-shot timer                     */
	/* NOTE: do this first, otherwise we'll hang in */
	/* 	   select as a result of any error        */
	err = sw_salt_register_timer(
				salt, 
				ifmd->m_timer, 
				ifmd->m_polltime, 
				(sw_timer_handler) ifmd, 
				timer_event_handler, NULL);
	sw_check_okay(err, exit);

	sw_debug(SW_LOG_VERBOSE, "***** polling for network events ******\n");
	err = sw_network_interfaces2(&newnifc, &newnifs);
	sw_check_okay(err, exit);

	/* iterate through the niftable and compare states */
	for (i = 0; i < ifmd->m_nifcount; i++)
	{
		int 						j;
		sw_uint32					inifi;
		sw_network_interface	inif = ifmd->m_nifs[i];
		sw_bool					found = SW_FALSE;
		char 						ifname[32];

		sw_network_interface_name(inif, ifname, 32);

		/* get the nif index for i */
		err = sw_network_interface_index(inif, &inifi);
		sw_check_okay(err, exit);

		for (j = 0; j < newnifc && !found; j++)
		{
			sw_uint32					jnifi;
			sw_bool					islinked, waslinked;
			sw_ipv4_address		newip, oldip;
			sw_network_interface	jnif = newnifs[i];

			sw_debug(SW_LOG_VERBOSE, "\t checking link status on %s\n", ifname);

			/* get the nif index for j */
			err = sw_network_interface_index(jnif, &jnifi);
			sw_check_okay(err, exit);
			
			if (jnifi == inifi)
			{
				found = SW_TRUE;

				/* check current link status */
				err = sw_network_interface_linked(jnif, &islinked);
				sw_check_okay(err, exit);

				/* check previous link status */
				err = sw_network_interface_linked(inif, &waslinked);
				sw_check_okay(err, exit);

				/* check current ip address */
				err = sw_network_interface_ipv4_address(jnif, &newip);
				sw_check_okay(err, exit);

				/* check previous ip address */
				err = sw_network_interface_ipv4_address(inif, &oldip);
				sw_check_okay(err, exit);

				/* see if link status or ip address changed */
				if (islinked != waslinked)	
				{
					sw_debug(SW_LOG_VERBOSE, "link status changed to %s\n", (islinked) ? "up" : "down");
					if (islinked && restart_interfaces)
					{
						err = nifd_restart_interface(*ifmd, inif);
						sw_check_okay(err, exit);
					}
					else
					{
						/* nothing to do */
					}

					sw_debug(SW_LOG_VERBOSE, "signalling autoipd\n");
					nifd_signal_autoipd(*ifmd, inif);
					signal_mDNS = SW_TRUE;
				}

				if (!sw_ipv4_address_equals(newip, oldip))
				{
					char oldname[18];
					char newname[18];

					sw_debug(SW_LOG_VERBOSE, "ip address changed from %s to %s\n", 
						sw_ipv4_address_name(oldip, oldname, 18), 
						sw_ipv4_address_name(newip, newname, 18));
					nifd_signal_autoipd(*ifmd, inif);
					signal_mDNS = SW_TRUE;
				}
			}
		}

		/* if we don't find the nif, it's been dropped */
		if (!found)
		{
			sw_assert(SW_FALSE);

			nifd_signal_autoipd(*ifmd, inif);
			signal_mDNS = SW_TRUE;
		}
	}

	/* if we added any netifs, signal daemons */
	if (signal_mDNS || (newnifc != ifmd->m_nifcount))
	{
		sw_debug(SW_LOG_VERBOSE, "sending SIGUSR1 to mDNSResponder\n");
		nifd_signal_daemon(*ifmd, "mDNSResponder");
	}

exit:

	/* free the old nifs */
	sw_network_interfaces_fina(ifmd->m_nifcount, ifmd->m_nifs);

	/* keep the new nifs */
	ifmd->m_nifcount 	= newnifc;
	ifmd->m_nifs		= newnifs;

   return err;
}

static sw_result
nifd_refresh_timer(nifd * self)
{
	sw_result err;

	err = sw_salt_unregister_timer(self->m_salt, self->m_timer);
	sw_check_okay(err, exit);

	err = sw_salt_register_timer(
				self->m_salt, 
				self->m_timer, 
				self->m_polltime, 
				(sw_timer_handler) self, 
				timer_event_handler, NULL);
	sw_check_okay(err, exit);

exit:

	return err;
}

static sw_result
sighup_command(
				sw_signal_handler	handler,
            sw_salt           salt,
            sw_signal         signal,
            sw_opaque         extra)
{
	sw_assert(handler != NULL);

	return nifd_refresh_timer((nifd*)handler);
}


static sw_result
sigusr1_command(
				sw_signal_handler	handler,
            sw_salt           salt,
            sw_signal         signal,
            sw_opaque         extra)
{
	sw_assert(handler != NULL);

	return nifd_refresh_timer((nifd*)handler);
}


static sw_result
sigusr2_command(
				sw_signal_handler	handler,
            sw_salt           salt,
            sw_signal         signal,
            sw_opaque         extra)
{
#if !defined(NDEBUG)
	return sw_debug_memory_inuse();
#endif
}


static sw_result
sigint_command(
				sw_signal_handler	handler,
            sw_salt           salt,
            sw_signal         signal,
            sw_opaque         extra)
{
	sw_assert(handler != NULL);
	sw_assert(salt != NULL);

   return sw_salt_stop_run(salt);
}


static void
print_usage(const char * program)
{
	fprintf(stderr, "usage: %s [options]\n\n", program);
	fprintf(stderr, "Options:\n\n");
	fprintf(stderr, "-h            this help\n");
	fprintf(stderr, "-v            display version information\n");
	fprintf(stderr, "-d            run in debug mode\n");
	fprintf(stderr, "-n            don't run ifdown/ifup on relink\n");
	fprintf(stderr, "-i interval   poll interval in seconds\n");
	exit(0);
}


int
main(int argc, char ** argv)
{
	struct _nifd ifmd;
	int				opt;
	sw_uint32			interval = 1;
	sw_bool			make_daemon = SW_TRUE;
	sw_result		err = SW_OKAY;

	/* sw_debug_enable(SW_TRUE); */

	while ((opt = getopt(argc, argv, "?hvdni:")) != EOF)
	{
		switch (opt)
		{
			case '?':
			case 'h':
			{
				print_usage(argv[0]);
			}
			break;

			case 'i':
			{
				interval = atol(optarg);

				if (interval == 0)
				{
					interval = 1;
				}
			}
			break;

			case 'v':
			{
				fprintf(stderr, "nifd release " VERSION " Copyright(c) Porchdog Software 2004-2005.\n");
				exit(0);
			}
			break;

			case 'd':
			{
				make_daemon = SW_FALSE;
			}
			break;

			case 'n':
			{
				restart_interfaces = SW_FALSE;
			}
			break;

			default:
			{
				print_usage(argv[0]);
			}
			break;
		}
	}

	if (getuid() != 0)
	{
		fprintf(stderr, "nifd must be run as root\n");
		exit(1);
	}

	if (make_daemon)
	{
		int res;

		res = daemon(SW_FALSE, SW_FALSE);
		err = sw_translate_error(res == 0, errno);
		sw_check_okay_log(err, exit);
	}

	err = nifd_init(&ifmd, interval);
	sw_check_okay(err, exit);

	sw_debug(SW_LOG_NOTICE, "nifd starting up...\n");
	err = sw_salt_run(ifmd.m_salt);
	sw_check_okay(err, exit);
	sw_debug(SW_LOG_NOTICE, "nifd shutting down...\n");

exit:

	nifd_fina(&ifmd);

	return err;
}
