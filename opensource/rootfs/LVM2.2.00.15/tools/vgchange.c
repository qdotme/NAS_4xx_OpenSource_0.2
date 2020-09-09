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

static int _activate_lvs_in_vg(struct cmd_context *cmd,
			       struct volume_group *vg, int lock)
{
	struct lv_list *lvl;
	struct logical_volume *lv;
	struct physical_volume *pv;
	int count = 0;

	list_iterate_items(lvl, &vg->lvs) {
		lv = lvl->lv;

		/* Only request activation of snapshot origin devices */
		if (lv_is_cow(lv))
			continue;

		/* Can't deactive a pvmove LV */
		if ((lock == LCK_LV_DEACTIVATE) && (lv->status & PVMOVE))
			continue;

		if (!lock_vol(cmd, lv->lvid.s, lock | LCK_NONBLOCK))
			continue;

		if ((lv->status & PVMOVE) &&
		    (pv = get_pvmove_pv_from_lv_mirr(lv))) {
			log_verbose("Spawning background process for %s %s",
				    lv->name, dev_name(pv->dev));
			pvmove_poll(cmd, dev_name(pv->dev), 1);
			continue;
		}

		count++;
	}

	return count;
}

static int _vgchange_available(struct cmd_context *cmd,
			       struct volume_group *vg)
{
	int lv_open, active;
	int available = !strcmp(arg_str_value(cmd, available_ARG, "n"), "y");

	/* FIXME: Force argument to deactivate them? */
	if (!available && (lv_open = lvs_in_vg_opened(vg))) {
		log_error("Can't deactivate volume group \"%s\" with %d open "
			  "logical volume(s)", vg->name, lv_open);
		return ECMD_FAILED;
	}

	if (available && (active = lvs_in_vg_activated(vg)))
		log_verbose("%d logical volume(s) in volume group \"%s\" "
			    "already active", active, vg->name);

	if (available && _activate_lvs_in_vg(cmd, vg, LCK_LV_ACTIVATE))
		log_verbose("Activated logical volumes in "
			    "volume group \"%s\"", vg->name);

	if (!available && _activate_lvs_in_vg(cmd, vg, LCK_LV_DEACTIVATE))
		log_verbose("Deactivated logical volumes in "
			    "volume group \"%s\"", vg->name);

	log_print("%d logical volume(s) in volume group \"%s\" now active",
		  lvs_in_vg_activated(vg), vg->name);
	return ECMD_PROCESSED;
}

static int _vgchange_resizeable(struct cmd_context *cmd,
				struct volume_group *vg)
{
	int resizeable = !strcmp(arg_str_value(cmd, resizeable_ARG, "n"), "y");

	if (resizeable && (vg->status & RESIZEABLE_VG)) {
		log_error("Volume group \"%s\" is already resizeable",
			  vg->name);
		return ECMD_FAILED;
	}

	if (!resizeable && !(vg->status & RESIZEABLE_VG)) {
		log_error("Volume group \"%s\" is already not resizeable",
			  vg->name);
		return ECMD_FAILED;
	}

	if (!archive(vg))
		return ECMD_FAILED;

	if (resizeable)
		vg->status |= RESIZEABLE_VG;
	else
		vg->status &= ~RESIZEABLE_VG;

	if (!vg_write(vg) || !vg_commit(vg))
		return ECMD_FAILED;

	backup(vg);

	log_print("Volume group \"%s\" successfully changed", vg->name);

	return ECMD_PROCESSED;
}

static int _vgchange_logicalvolume(struct cmd_context *cmd,
				   struct volume_group *vg)
{
	uint32_t max_lv = arg_uint_value(cmd, logicalvolume_ARG, 0);

	if (!(vg->status & RESIZEABLE_VG)) {
		log_error("Volume group \"%s\" must be resizeable "
			  "to change MaxLogicalVolume", vg->name);
		return ECMD_FAILED;
	}

	if (!(vg->fid->fmt->features & FMT_UNLIMITED_VOLS)) {
		if (!max_lv)
			max_lv = 255;
		else if (max_lv > 255) {
			log_error("MaxLogicalVolume limit is 255");
			return ECMD_FAILED;
		}
	}

	if (max_lv && max_lv < vg->lv_count) {
		log_error("MaxLogicalVolume is less than the current number "
			  "%d of logical volume(s) for \"%s\"", vg->lv_count,
			  vg->name);
		return ECMD_FAILED;
	}

	if (!archive(vg))
		return ECMD_FAILED;

	vg->max_lv = max_lv;

	if (!vg_write(vg) || !vg_commit(vg))
		return ECMD_FAILED;

	backup(vg);

	log_print("Volume group \"%s\" successfully changed", vg->name);

	return ECMD_PROCESSED;
}

static int _vgchange_tag(struct cmd_context *cmd, struct volume_group *vg,
			 int arg)
{
	const char *tag;

	if (!(tag = arg_str_value(cmd, arg, NULL))) {
		log_error("Failed to get tag");
		return ECMD_FAILED;
	}

	if (!(vg->fid->fmt->features & FMT_TAGS)) {
		log_error("Volume group %s does not support tags", vg->name);
		return ECMD_FAILED;
	}

	if (!archive(vg))
		return ECMD_FAILED;

	if ((arg == addtag_ARG)) {
		if (!str_list_add(cmd->mem, &vg->tags, tag)) {
			log_error("Failed to add tag %s to volume group %s",
				  tag, vg->name);
			return ECMD_FAILED;
		}
	} else {
		if (!str_list_del(&vg->tags, tag)) {
			log_error("Failed to remove tag %s from volume group "
				  "%s", tag, vg->name);
			return ECMD_FAILED;
		}
	}

	if (!vg_write(vg) || !vg_commit(vg))
		return ECMD_FAILED;

	backup(vg);

	log_print("Volume group \"%s\" successfully changed", vg->name);

	return ECMD_PROCESSED;
}

static int _vgchange_uuid(struct cmd_context *cmd, struct volume_group *vg)
{
	struct lv_list *lvl;

	if (lvs_in_vg_activated(vg)) {
		log_error("Volume group has active logical volumes");
		return ECMD_FAILED;
	}

	if (!archive(vg))
		return ECMD_FAILED;

	id_create(&vg->id);

	list_iterate_items(lvl, &vg->lvs) {
		memcpy(&lvl->lv->lvid, &vg->id, sizeof(vg->id));
	}

	if (!vg_write(vg) || !vg_commit(vg))
		return ECMD_FAILED;

	backup(vg);

	log_print("Volume group \"%s\" successfully changed", vg->name);

	return ECMD_PROCESSED;
}

static int vgchange_single(struct cmd_context *cmd, const char *vg_name,
			   struct volume_group *vg, int consistent,
			   void *handle)
{
	int r = ECMD_FAILED;

	if (!vg) {
		log_error("Unable to find volume group \"%s\"", vg_name);
		return ECMD_FAILED;
	}

	if (!consistent) {
		unlock_vg(cmd, vg_name);
		dev_close_all();
		log_error("Volume group \"%s\" inconsistent", vg_name);
		if (!(vg = recover_vg(cmd, vg_name, LCK_VG_WRITE)))
			return ECMD_FAILED;
	}

	if (!(vg->status & LVM_WRITE) && !arg_count(cmd, available_ARG)) {
		log_error("Volume group \"%s\" is read-only", vg->name);
		return ECMD_FAILED;
	}

	if (vg->status & EXPORTED_VG) {
		log_error("Volume group \"%s\" is exported", vg_name);
		return ECMD_FAILED;
	}

	if (arg_count(cmd, available_ARG))
		r = _vgchange_available(cmd, vg);

	else if (arg_count(cmd, resizeable_ARG))
		r = _vgchange_resizeable(cmd, vg);

	else if (arg_count(cmd, logicalvolume_ARG))
		r = _vgchange_logicalvolume(cmd, vg);

	else if (arg_count(cmd, addtag_ARG))
		r = _vgchange_tag(cmd, vg, addtag_ARG);

	else if (arg_count(cmd, deltag_ARG))
		r = _vgchange_tag(cmd, vg, deltag_ARG);

	else if (arg_count(cmd, uuid_ARG))
		r = _vgchange_uuid(cmd, vg);

	return r;
}

int vgchange(struct cmd_context *cmd, int argc, char **argv)
{
	if (!
	    (arg_count(cmd, available_ARG) + arg_count(cmd, logicalvolume_ARG) +
	     arg_count(cmd, resizeable_ARG) + arg_count(cmd, deltag_ARG) +
	     arg_count(cmd, addtag_ARG) + arg_count(cmd, uuid_ARG))) {
		log_error("One of -a, -l, -x, --addtag, --deltag or --uuid "
			  "options required");
		return EINVALID_CMD_LINE;
	}

	if (arg_count(cmd, available_ARG) + arg_count(cmd, logicalvolume_ARG) +
	    arg_count(cmd, resizeable_ARG) + arg_count(cmd, deltag_ARG) +
	    arg_count(cmd, addtag_ARG) + arg_count(cmd, uuid_ARG) > 1) {
		log_error("Only one of -a, -l, -x, --addtag, --deltag or --uuid"
			  " options allowed");
		return EINVALID_CMD_LINE;
	}

	if (arg_count(cmd, ignorelockingfailure_ARG) &&
	    !arg_count(cmd, available_ARG)) {
		log_error("--ignorelockingfailure only available with -a");
		return EINVALID_CMD_LINE;
	}

	if (arg_count(cmd, available_ARG) == 1
	    && arg_count(cmd, autobackup_ARG)) {
		log_error("-A option not necessary with -a option");
		return EINVALID_CMD_LINE;
	}

	return process_each_vg(cmd, argc, argv,
			       (arg_count(cmd, available_ARG)) ?
			       LCK_VG_READ : LCK_VG_WRITE, 0, NULL,
			       &vgchange_single);
}
