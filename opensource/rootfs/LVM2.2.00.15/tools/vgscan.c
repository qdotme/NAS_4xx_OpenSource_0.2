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

#include "tools.h"

static int vgscan_single(struct cmd_context *cmd, const char *vg_name,
			 struct volume_group *vg, int consistent, void *handle)
{
	if (!vg) {
		log_error("Volume group \"%s\" not found", vg_name);
		return ECMD_FAILED;
	}

	if (!consistent) {
		unlock_vg(cmd, vg_name);
		dev_close_all();
		log_error("Volume group \"%s\" inconsistent", vg_name);
		/* Don't allow partial switch to this program */
		if (!(vg = recover_vg(cmd, vg_name, LCK_VG_WRITE)))
			return ECMD_FAILED;
	}

	log_print("Found %svolume group \"%s\" using metadata type %s",
		  (vg->status & EXPORTED_VG) ? "exported " : "", vg_name,
		  vg->fid->fmt->name);

	return ECMD_PROCESSED;
}

int vgscan(struct cmd_context *cmd, int argc, char **argv)
{
	int maxret, ret;

	if (argc) {
		log_error("Too many parameters on command line");
		return EINVALID_CMD_LINE;
	}

	log_verbose("Wiping cache of LVM-capable devices");
	persistent_filter_wipe(cmd->filter);

	log_verbose("Wiping internal cache");
	lvmcache_destroy();

	log_print("Reading all physical volumes.  This may take a while...");

	maxret = process_each_vg(cmd, argc, argv, LCK_VG_READ, 1, NULL,
				 &vgscan_single);

	if (arg_count(cmd, mknodes_ARG)) {
		ret = vgmknodes(cmd, argc, argv);
		if (ret > maxret)
			maxret = ret;
	}

	return maxret;
}
