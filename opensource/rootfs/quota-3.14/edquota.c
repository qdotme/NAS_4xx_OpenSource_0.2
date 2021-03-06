/*
 * Copyright (c) 1980, 1990 Regents of the University of California. All
 * rights reserved.
 * 
 * This code is derived from software contributed to Berkeley by Robert Elz at
 * The University of Melbourne.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer. 2.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 3. All advertising
 * materials mentioning features or use of this software must display the
 * following acknowledgement: This product includes software developed by the
 * University of California, Berkeley and its contributors. 4. Neither the
 * name of the University nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific
 * prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ident "$Copyright: (c) 1980, 1990 Regents of the University of California. $"
#ident "$Copyright: All rights reserved. $"
#ident "$Id: edquota.c,v 1.1.1.1 2007/01/17 07:37:19 jason Exp $"

/*
 * Disk quota editor.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <paths.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>

#include "pot.h"
#include "quotaops.h"
#include "quotasys.h"
#include "quotaio.h"
#include "common.h"

#define FL_EDIT_PERIOD 1
#define FL_EDIT_TIMES 2
#define FL_REMOTE 4
#define FL_NUMNAMES 8

char *progname;

int flags, quotatype;
int fmt = -1;
char *protoname;
char *dirname;

void usage(void)
{
#if defined(RPC_SETQUOTA)
	char *rpcflag = "[-r] ";
#else
	char *rpcflag = "";
#endif
	errstr(_("Usage:\n\tedquota %1$s[-u] [-F formatname] [-p username] [-f filesystem] username ...\n\
\tedquota %1$s-g [-F formatname] [-p groupname] [-f filesystem] groupname ...\n\
\tedquota %1$s[-u|g] [-F formatname] [-f filesystem] -t\n\
\tedquota %1$s[-u|g] [-F formatname] [-f filesystem] -T username|groupname ...\n"), rpcflag);
	fputs(_("\n\
-u, --user                    edit user data\n\
-g, --group                   edit group data\n"), stderr);
#if defined(RPC_SETQUOTA)
	fputs(_("-r, --remote                  edit remote quota (via RPC)\n"), stderr);
#endif
	fputs(_("-F, --format=formatname       edit quotas of a specific format\n\
-p, --prototype=name          copy data from a prototype user/group\n\
    --always-resolve          always try to resolve name, even if it is\n\
                              composed only of digits\n\
-f, --filesystem=filesystem   edit data only on a specific filesystem\n\
-t, --edit-period             edit grace period\n\
-T, --edit-times              edit grace time of a user/group\n\
-h, --help                    display this help text and exit\n\
-V, --version                 display version information and exit\n\n"), stderr);
	fprintf(stderr, _("Bugs to: %s\n"), MY_EMAIL);
	exit(1);
}

int parse_options(int argc, char **argv)
{
	int ret;
	struct option long_opts[] = {
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'V' },
		{ "prototype", 1, NULL, 'p' },
		{ "user", 0, NULL, 'u' },
		{ "group", 0, NULL, 'g' },
		{ "format", 1, NULL, 'F' },
		{ "filesystem", 1, NULL, 'f' },
#if defined(RPC_SETQUOTA)
		{ "remote", 0, NULL, 'r' },
#endif
		{ "always-resolve", 0, NULL, 256 },
		{ "edit-period", 0, NULL, 't' },
		{ "edit-times", 0, NULL, 'T' },
		{ NULL, 0, NULL, 0 }
	};

	if (argc < 2)
		usage();

	quotatype = USRQUOTA;
#if defined(RPC_SETQUOTA)
	while ((ret = getopt_long(argc, argv, "ugrntTVp:F:f:", long_opts, NULL)) != -1) {
#else
	while ((ret = getopt_long(argc, argv, "ugtTVp:F:f:", long_opts, NULL)) != -1) {
#endif
		switch (ret) {
		  case 'p':
			  protoname = optarg;
			  break;
		  case 'g':
			  quotatype = GRPQUOTA;
			  break;
#if defined(RPC_SETQUOTA)
		  case 'n':
		  case 'r':
			  flags |= FL_REMOTE;
			  break;
#endif
		  case 'u':
			  quotatype = USRQUOTA;
			  break;
		  case 't':
			  flags |= FL_EDIT_PERIOD;
			  break;
		  case 'T':
			  flags |= FL_EDIT_TIMES;
			  break;
		  case 'F':
			  if ((fmt = name2fmt(optarg)) == QF_ERROR)	/* Error? */
				  exit(1);
			  break;
		  case 'f':
			  dirname = optarg;
			  break;
		  case 256:
			  flags |= FL_NUMNAMES;
			  break;
		  case 'V':
			  version();
			  exit(0);
		  default:
			  usage();
		}
	}
	argc -= optind;

	if (((flags & FL_EDIT_PERIOD) && argc != 0) || ((flags & FL_EDIT_TIMES) && argc < 1))
		usage();
	if ((flags & (FL_EDIT_PERIOD | FL_EDIT_TIMES)) && protoname) {
		errstr(_("Prototype name does not make sence when editting grace period or times.\n"));
		usage();
	}
	return optind;
}

void copy_prototype(int argc, char **argv, struct quota_handle **handles)
{
	int ret, protoid, id;
	struct dquot *protoprivs, *curprivs, *pprivs, *cprivs;
	
	ret = 0;
	protoid = name2id(protoname, quotatype, !!(flags & FL_NUMNAMES), NULL);
	protoprivs = getprivs(protoid, handles, 0);
	while (argc-- > 0) {
		id = name2id(*argv++, quotatype, !!(flags & FL_NUMNAMES), NULL);
		curprivs = getprivs(id, handles, 0);

		for (pprivs = protoprivs, cprivs = curprivs; pprivs && cprivs;
		     pprivs = pprivs->dq_next, cprivs = cprivs->dq_next) {
			if (!devcmp_handles(pprivs->dq_h, cprivs->dq_h)) {
				errstr(_("fsname mismatch\n"));
				continue;
			}
			cprivs->dq_dqb.dqb_bsoftlimit =
				pprivs->dq_dqb.dqb_bsoftlimit;
			cprivs->dq_dqb.dqb_bhardlimit =
				pprivs->dq_dqb.dqb_bhardlimit;
			cprivs->dq_dqb.dqb_isoftlimit =
				pprivs->dq_dqb.dqb_isoftlimit;
			cprivs->dq_dqb.dqb_ihardlimit =
				pprivs->dq_dqb.dqb_ihardlimit;
			update_grace_times(cprivs);
		}
		if (putprivs(curprivs, COMMIT_LIMITS) == -1)
			ret = -1;
		freeprivs(curprivs);
	}
	if (dispose_handle_list(handles) == -1)
		ret = -1;
	freeprivs(protoprivs);
	exit(ret ? 1 : 0);
}

int main(int argc, char **argv)
{
	struct dquot *curprivs;
	int tmpfd, ret, id;
	struct quota_handle **handles;
	char *tmpfil, *tmpdir = NULL;

	gettexton();
	progname = basename(argv[0]);
	ret = parse_options(argc, argv);
	argc -= ret;
	argv += ret;

	init_kernel_interface();
	handles = create_handle_list(dirname ? 1 : 0, dirname ? &dirname : NULL, quotatype, fmt, 0, (flags & FL_REMOTE) ? 0 : MS_LOCALONLY);
	if (!handles[0]) {
		dispose_handle_list(handles);
		fputs(_("No filesystems with quota detected.\n"), stderr);
		return 0;
	}
	if (protoname)
		copy_prototype(argc, argv, handles);

	umask(077);
	if (getuid() == geteuid() && getgid() == getegid())
		tmpdir = getenv("TMPDIR");
	if (!tmpdir)
		tmpdir = _PATH_TMP;
	tmpfil = smalloc(strlen(tmpdir) + strlen("/EdP.aXXXXXX") + 1);
	strcpy(tmpfil, tmpdir);
	strcat(tmpfil, "/EdP.aXXXXXX");
	tmpfd = mkstemp(tmpfil);
	fchown(tmpfd, getuid(), getgid());
	ret = 0;
	if (flags & FL_EDIT_PERIOD) {
		if (writetimes(handles, tmpfd) < 0) {
			errstr(_("Cannot write grace times to file.\n"));
			ret = -1;
		}
		if (editprivs(tmpfil) < 0) {
			errstr(_("Error while editting grace times.\n"));
			ret = -1;
		}
		if (readtimes(handles, tmpfd) < 0) {
			errstr(_("Failed to parse grace times file.\n"));
			ret = -1;
		}
	}
	else if (flags & FL_EDIT_TIMES) {
		for (; argc > 0; argc--, argv++) {
			id = name2id(*argv, quotatype, !!(flags & FL_NUMNAMES), NULL);
			curprivs = getprivs(id, handles, 0);
			if (writeindividualtimes(curprivs, tmpfd, *argv, quotatype) < 0) {
				errstr(_("Cannot write individual grace times to file.\n"));
				ret = -1;
				continue;
			}
			if (editprivs(tmpfil) < 0) {
				errstr(_("Error while editting individual grace times.\n"));
				ret = -1;
				continue;
			}
			if (readindividualtimes(curprivs, tmpfd) < 0) {
				errstr(_("Cannot read individual grace times from file.\n"));
				ret = -1;
				continue;
			}
			if (putprivs(curprivs, COMMIT_TIMES) == -1)
				ret = -1;
			freeprivs(curprivs);
		}
	}
	else {
		for (; argc > 0; argc--, argv++) {
			id = name2id(*argv, quotatype, !!(flags & FL_NUMNAMES), NULL);
			curprivs = getprivs(id, handles, 0);
			if (writeprivs(curprivs, tmpfd, *argv, quotatype) < 0) {
				errstr(_("Cannot write quotas to file.\n"));
				ret = -1;
				continue;
			}
			if (editprivs(tmpfil) < 0) {
				errstr(_("Error while editting quotas.\n"));
				ret = -1;
				continue;
			}
			close(tmpfd);
			if ((tmpfd = open(tmpfil, O_RDONLY)) < 0)
				die(1, _("Cannot reopen!"));
			if (readprivs(curprivs, tmpfd) < 0) {
				errstr(_("Cannot read quotas from file.\n"));
				ret = -1;
				continue;
			}
			if (putprivs(curprivs, COMMIT_LIMITS) == -1)
				ret = -1;
			freeprivs(curprivs);
		}
	}
	if (dispose_handle_list(handles) == -1)
		ret = -1;

	close(tmpfd);
	unlink(tmpfil);
	free(tmpfil);
	return ret ? 1 : 0;
}
