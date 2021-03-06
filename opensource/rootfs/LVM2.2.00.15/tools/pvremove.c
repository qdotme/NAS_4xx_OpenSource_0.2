/*
 * Copyright (C) 2002-2004 Sistina Software, Inc. All rights reserved.
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

const char _really_wipe[] =
    "Really WIPE LABELS from physical volume \"%s\" of volume group \"%s\" [y/n]? ";

/*
 * Decide whether it is "safe" to wipe the labels on this device.
 * 0 indicates we may not.
 */
static int pvremove_check(struct cmd_context *cmd, const char *name)
{
	struct physical_volume *pv;

	/* is the partition type set correctly ? */
	if ((arg_count(cmd, force_ARG) < 1) && !is_lvm_partition(name)) {
		log_error("%s: Not LVM partition type: use -f to override",
			  name);
		return 0;
	}

	/* is there a pv here already */
	if (!(pv = pv_read(cmd, name, NULL, NULL)))
		return 1;

	/* orphan ? */
	if (!pv->vg_name[0])
		return 1;

	/* Allow partial & exported VGs to be destroyed. */
	/* we must have -ff to overwrite a non orphan */
	if (arg_count(cmd, force_ARG) < 2) {
		log_error("Can't pvremove physical volume \"%s\" of "
			  "volume group \"%s\" without -ff", name, pv->vg_name);
		return 0;
	}

	/* prompt */
	if (!arg_count(cmd, yes_ARG) &&
	    yes_no_prompt(_really_wipe, name, pv->vg_name) == 'n') {
		log_print("%s: physical volume label not removed", name);
		return 0;
	}

	if (arg_count(cmd, force_ARG)) {
		log_print("WARNING: Wiping physical volume label from "
			  "%s%s%s%s", name,
			  pv->vg_name[0] ? " of volume group \"" : "",
			  pv->vg_name[0] ? pv->vg_name : "",
			  pv->vg_name[0] ? "\"" : "");
	}

	return 1;
}

static int pvremove_single(struct cmd_context *cmd, const char *pv_name,
			   void *handle)
{
	struct device *dev;

	if (!lock_vol(cmd, "", LCK_VG_WRITE)) {
		log_error("Can't get lock for orphan PVs");
		return ECMD_FAILED;
	}

	if (!pvremove_check(cmd, pv_name))
		goto error;

	if (!(dev = dev_cache_get(pv_name, cmd->filter))) {
		log_error("%s: Couldn't find device.", pv_name);
		goto error;
	}

	/* Wipe existing label(s) */
	if (!label_remove(dev)) {
		log_error("Failed to wipe existing label(s) on %s", pv_name);
		goto error;
	}

	log_print("Labels on physical volume \"%s\" successfully wiped",
		  pv_name);

	unlock_vg(cmd, "");
	return ECMD_PROCESSED;

      error:
	unlock_vg(cmd, "");
	return ECMD_FAILED;
}

int pvremove(struct cmd_context *cmd, int argc, char **argv)
{
	int i, r;
	int ret = ECMD_PROCESSED;

	if (!argc) {
		log_error("Please enter a physical volume path");
		return EINVALID_CMD_LINE;
	}

	if (arg_count(cmd, yes_ARG) && !arg_count(cmd, force_ARG)) {
		log_error("Option y can only be given with option f");
		return EINVALID_CMD_LINE;
	}

	for (i = 0; i < argc; i++) {
		r = pvremove_single(cmd, argv[i], NULL);
		if (r > ret)
			ret = r;
	}

	return ret;
}
