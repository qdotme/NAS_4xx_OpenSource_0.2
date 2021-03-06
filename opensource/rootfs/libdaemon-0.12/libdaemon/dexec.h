#ifndef foodexechfoo
#define foodexechfoo

/* $Id: dexec.h,v 1.1.1.1 2008/10/06 10:01:36 nick Exp $ */

/*
 * This file is part of libdaemon.
 *
 * libdaemon is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * libdaemon is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libdaemon; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \file
 *
 * Contains a robust API for running sub processes with STDOUT and
 * STDERR redirected to syslog
 */

/** This variable is defined to 1 iff daemon_exec() is supported.*/
#define DAEMON_EXEC_AVAILABLE 1

#if defined(__GNUC__) && ! defined(DAEMON_GCC_SENTINEL)
/** A macro for making use of GCCs printf compilation warnings */
#define DAEMON_GCC_SENTINEL __attribute__ ((sentinel))
#else
#define DAEMON_GCC_SENTINEL
#endif
    
/** Run the specified executable with the specified arguments in the
 * specified directory and return the return value of the program in
 * the specified pointer. The calling process is blocked until the
 * child finishes and all child output (either STDOUT or STDIN) has
 * been written to syslog. Running this function requires that
 * daemon_signal() has been called with SIGCHLD as argument.
 * 
 * @param dir Working directory for the process.
 * @param ret A pointer to an integer to write the return value of the program to.
 * @param prog The path to the executable
 * @param ... The arguments to be passed to the program, followed by a (char *) NULL
 * @return Nonzero on failure, zero on success
 */
int daemon_exec(const char *dir, int *ret, const char *prog, ...) DAEMON_GCC_SENTINEL;

/** This variable is defined to 1 iff daemon_execv() is supported.*/
#define DAEMON_EXECV_AVAILABLE 1

/** The same as daemon_exec, but without variadic arguments */
int daemon_execv(const char *dir, int *ret, const char *prog, va_list ap);
    
#ifdef __cplusplus
}
#endif

#endif
