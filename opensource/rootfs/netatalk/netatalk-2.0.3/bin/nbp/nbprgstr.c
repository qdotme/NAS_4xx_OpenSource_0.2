/*
 * $Id: nbprgstr.c,v 1.1.1.1 2008/06/18 10:55:40 jason Exp $
 *
 * Copyright (c) 1990,1993 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>

#include <netatalk/endian.h>
#include <netatalk/at.h>
#include <atalk/netddp.h>
#include <atalk/nbp.h>
#include <atalk/util.h>
#include <atalk/unicode.h>

void Usage( av0 )
    char	*av0;
{
    char	*p;

    if (( p = strrchr( av0, '/' )) == 0 ) {
	p = av0;
    } else {
	p++;
    }

    fprintf( stderr, "Usage: %s [ -A address ] [-m Mac charset] [ -p port] obj:type@zone\n", p );
    exit( 1 );
}

int main( ac, av )
    int		ac;
    char	**av;
{
    struct sockaddr_at	addr;
    struct at_addr      ataddr;
    char		*Obj = 0, *Type = 0, *Zone = 0;
    char		*convname = 0;
    int			s, c, port = 0;
    charset_t		chMac = CH_MAC;
    
    extern char		*optarg;
    extern int		optind;

    memset(&ataddr, 0, sizeof(ataddr));
    while (( c = getopt( ac, av, "p:A:m:" )) != EOF ) {
	switch ( c ) {
	case 'A':
	    if (!atalk_aton(optarg, &ataddr)) {
	        fprintf(stderr, "Bad address.\n");
		exit(1);
	    }
	    break;

        case 'm':
            if ((charset_t)-1 == (chMac = add_charset(optarg)) ) {
                fprintf(stderr, "Invalid Mac charset.\n");
                exit(1);
            }
            break;

	case 'p' :
	    port = atoi( optarg );
	    break;

	default :
	    Usage( av[ 0 ] );
	}
    }

    if ( ac - optind != 1 ) {
	Usage( av[ 0 ] );
    }

    /* Convert the name */
    if ((size_t)(-1) == convert_string_allocate(CH_UNIX, chMac,
                        av[optind], strlen(av[optind]), &convname))
        convname = av[optind];

    /*
     * Get the name. If Type or Obj aren't specified, error.
     */
    if ( nbp_name( convname, &Obj, &Type, &Zone ) || !Obj || !Type ) {
	Usage( av[ 0 ] );
    }

    memset(&addr, 0, sizeof(addr));
    memcpy(&addr.sat_addr, &ataddr, sizeof(addr.sat_addr));
    if ((s = netddp_open(&addr, NULL)) < 0)
	return( -1 );

    if ( port ) {
	addr.sat_port = port;
    }

    if ( nbp_rgstr( &addr, Obj, Type, Zone ) < 0 ) {
	perror( "nbp_rgstr" );
	fprintf( stderr, "Can't register %s:%s@%s\n", Obj, Type,
		Zone ? Zone : "*" );
	exit( 1 );
    }
    netddp_close(s);

    return 0;
}
