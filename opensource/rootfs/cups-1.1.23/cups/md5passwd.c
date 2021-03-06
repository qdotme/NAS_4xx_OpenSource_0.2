/*
 * "$Id: md5passwd.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
 *
 *   MD5 password support for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 1997-2005 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights are outlined in the file
 *   "LICENSE.txt" which should have been included with this file.  If this
 *   file is missing or damaged please contact Easy Software Products
 *   at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636 USA
 *
 *       Voice: (301) 373-9600
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   httpMD5()       - Compute the MD5 sum of the username:group:password.
 *   httpMD5Nonce()  - Combine the MD5 sum of the username, group, and password
 *                     with the server-supplied nonce value.
 *   httpMD5String() - Convert an MD5 sum to a character string.
 */

/*
 * Include necessary headers...
 */

#include "http.h"
#include "string.h"


/*
 * 'httpMD5()' - Compute the MD5 sum of the username:group:password.
 */

char *				/* O - MD5 sum */
httpMD5(const char *username,	/* I - User name */
        const char *realm,	/* I - Realm name */
        const char *passwd,	/* I - Password string */
	char       md5[33])	/* O - MD5 string */
{
  md5_state_t	state;		/* MD5 state info */
  md5_byte_t	sum[16];	/* Sum data */
  char		line[256];	/* Line to sum */


 /*
  * Compute the MD5 sum of the user name, group name, and password.
  */

  snprintf(line, sizeof(line), "%s:%s:%s", username, realm, passwd);
  md5_init(&state);
  md5_append(&state, (md5_byte_t *)line, strlen(line));
  md5_finish(&state, sum);

 /*
  * Return the sum...
  */

  return (httpMD5String(sum, md5));
}


/*
 * 'httpMD5Final()' - Combine the MD5 sum of the username, group, and password
 *                    with the server-supplied nonce value, method, and
 *                    request-uri.
 */

char *					/* O - New sum */
httpMD5Final(const char *nonce,		/* I - Server nonce value */
             const char *method,	/* I - METHOD (GET, POST, etc.) */
	     const char *resource,	/* I - Resource path */
             char       md5[33])	/* IO - MD5 sum */
{
  md5_state_t	state;			/* MD5 state info */
  md5_byte_t	sum[16];		/* Sum data */
  char		line[1024];		/* Line of data */
  char		a2[33];			/* Hash of method and resource */


 /*
  * First compute the MD5 sum of the method and resource...
  */

  snprintf(line, sizeof(line), "%s:%s", method, resource);
  md5_init(&state);
  md5_append(&state, (md5_byte_t *)line, strlen(line));
  md5_finish(&state, sum);
  httpMD5String(sum, a2);

 /*
  * Then combine A1 (MD5 of username, realm, and password) with the nonce
  * and A2 (method + resource) values to get the final MD5 sum for the
  * request...
  */

  snprintf(line, sizeof(line), "%s:%s:%s", md5, nonce, a2);

  md5_init(&state);
  md5_append(&state, (md5_byte_t *)line, strlen(line));
  md5_finish(&state, sum);

  return (httpMD5String(sum, md5));
}


/*
 * 'httpMD5String()' - Convert an MD5 sum to a character string.
 */

char *					/* O - MD5 sum in hex */
httpMD5String(const md5_byte_t *sum,	/* I - MD5 sum data */
              char             md5[33])	/* O - MD5 sum in hex */
{
  int		i;			/* Looping var */
  char		*md5ptr;		/* Pointer into MD5 string */
  static const char hex[] = "0123456789abcdef";
					/* Hex digits */


 /*
  * Convert the MD5 sum to hexadecimal...
  */

  for (i = 16, md5ptr = md5; i > 0; i --, sum ++)
  {
    *md5ptr++ = hex[*sum >> 4];
    *md5ptr++ = hex[*sum & 15];
  }

  *md5ptr = '\0';

  return (md5);
}


/*
 * End of "$Id: md5passwd.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
 */
