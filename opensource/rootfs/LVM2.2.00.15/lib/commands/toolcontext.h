/*
 * Copyright (C) 2001-2004 Sistina Software, Inc. All rights reserved.  
 * Copyright (C) 2004 Red Hat, Inc. All rights reserved.
 *
 * This file is part of LVM2.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LVM_TOOLCONTEXT_H
#define _LVM_TOOLCONTEXT_H

#include "dev-cache.h"
#include "config.h"
#include "pool.h"
#include "metadata.h"

#include <stdio.h>
#include <limits.h>

/*
 * Config options that can be changed while commands are processed
 */
struct config_info {
	int debug;
	int verbose;
	int test;
	int syslog;
	int activation;
	int suffix;
	uint64_t unit_factor;
	char unit_type;
	const char *msg_prefix;
	int cmd_name;		/* Show command name? */

	int archive;		/* should we archive ? */
	int backup;		/* should we backup ? */

	struct format_type *fmt;

	mode_t umask;
};

/* FIXME Split into tool & library contexts */
/* command-instance-related variables needed by library */
struct cmd_context {
	struct pool *libmem;	/* For permanent config data */
	struct pool *mem;	/* Transient: Cleared between each command */

	const struct format_type *fmt;	/* Current format to use by default */
	struct format_type *fmt_backup;	/* Format to use for backups */

	struct list formats;	/* Available formats */
	const char *hostname;
	const char *kernel_vsn;

	char *cmd_line;
	struct command *command;
	struct arg *args;
	char **argv;

	struct dev_filter *filter;
	int dump_filter;	/* Dump filter when exiting? */

	struct config_tree *cft;
	struct config_info default_settings;
	struct config_info current_settings;

	/* List of defined tags */
	struct list tags;

	char sys_dir[PATH_MAX];
	char dev_dir[PATH_MAX];
	char proc_dir[PATH_MAX];
};

struct cmd_context *create_toolcontext(struct arg *the_args);
void destroy_toolcontext(struct cmd_context *cmd);
int refresh_toolcontext(struct cmd_context *cmd);

#endif
