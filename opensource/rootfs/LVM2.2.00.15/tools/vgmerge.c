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

static int _vgmerge_single(struct cmd_context *cmd, const char *vg_name_to,
			   const char *vg_name_from)
{
	struct volume_group *vg_to, *vg_from;
	struct lv_list *lvl1, *lvl2;
	int active;
	int consistent = 1;

	if (!strcmp(vg_name_to, vg_name_from)) {
		log_error("Duplicate volume group name \"%s\"", vg_name_from);
		return ECMD_FAILED;
	}

	log_verbose("Checking for volume group \"%s\"", vg_name_to);
	if (!lock_vol(cmd, vg_name_to, LCK_VG_WRITE)) {
		log_error("Can't get lock for %s", vg_name_to);
		return ECMD_FAILED;
	}

	if (!(vg_to = vg_read(cmd, vg_name_to, &consistent)) || !consistent) {
		log_error("Volume group \"%s\" doesn't exist", vg_name_to);
		unlock_vg(cmd, vg_name_to);
		return ECMD_FAILED;
	}

	if (vg_to->status & EXPORTED_VG) {
		log_error("Volume group \"%s\" is exported", vg_to->name);
		unlock_vg(cmd, vg_name_to);
		return ECMD_FAILED;
	}

	if (!(vg_to->status & LVM_WRITE)) {
		log_error("Volume group \"%s\" is read-only", vg_to->name);
		unlock_vg(cmd, vg_name_to);
		return ECMD_FAILED;
	}

	log_verbose("Checking for volume group \"%s\"", vg_name_from);
	if (!lock_vol(cmd, vg_name_from, LCK_VG_WRITE | LCK_NONBLOCK)) {
		log_error("Can't get lock for %s", vg_name_from);
		unlock_vg(cmd, vg_name_to);
		return ECMD_FAILED;
	}

	consistent = 1;
	if (!(vg_from = vg_read(cmd, vg_name_from, &consistent)) || !consistent) {
		log_error("Volume group \"%s\" doesn't exist", vg_name_from);
		goto error;
	}

	if (vg_from->status & EXPORTED_VG) {
		log_error("Volume group \"%s\" is exported", vg_from->name);
		goto error;
	}

	if (!(vg_from->status & LVM_WRITE)) {
		log_error("Volume group \"%s\" is read-only", vg_from->name);
		goto error;
	}

	if ((active = lvs_in_vg_activated(vg_from))) {
		log_error("Logical volumes in \"%s\" must be inactive",
			  vg_name_from);
		goto error;
	}

	/* Check compatibility */
	if (vg_to->extent_size != vg_from->extent_size) {
		log_error("Extent sizes differ: %d (%s) and %d (%s)",
			  vg_to->extent_size, vg_to->name,
			  vg_from->extent_size, vg_from->name);
		goto error;
	}

	if (vg_to->max_pv &&
	    (vg_to->max_pv < vg_to->pv_count + vg_from->pv_count)) {
		log_error("Maximum number of physical volumes (%d) exceeded "
			  " for \"%s\" and \"%s\"", vg_to->max_pv, vg_to->name,
			  vg_from->name);
		goto error;
	}

	if (vg_to->max_lv &&
	    (vg_to->max_lv < vg_to->lv_count + vg_from->lv_count)) {
		log_error("Maximum number of logical volumes (%d) exceeded "
			  " for \"%s\" and \"%s\"", vg_to->max_lv, vg_to->name,
			  vg_from->name);
		goto error;
	}

	/* Check no conflicts with LV names */
	list_iterate_items(lvl1, &vg_to->lvs) {
		char *name1 = lvl1->lv->name;

		list_iterate_items(lvl2, &vg_from->lvs) {
			char *name2 = lvl2->lv->name;

			if (!strcmp(name1, name2)) {
				log_error("Duplicate logical volume "
					  "name \"%s\" "
					  "in \"%s\" and \"%s\"",
					  name1, vg_to->name, vg_from->name);
				goto error;
			}
		}
	}

	/* FIXME List arg: vg_show_with_pv_and_lv(vg_to); */

	if (!archive(vg_from) || !archive(vg_to))
		goto error;

	/* Merge volume groups */
	while (!list_empty(&vg_from->pvs)) {
		struct list *pvh = vg_from->pvs.n;
		struct physical_volume *pv;

		list_del(pvh);
		list_add(&vg_to->pvs, pvh);

		pv = list_item(pvh, struct pv_list)->pv;
		pv->vg_name = pool_strdup(cmd->mem, vg_to->name);
	}
	vg_to->pv_count += vg_from->pv_count;

	while (!list_empty(&vg_from->lvs)) {
		struct list *lvh = vg_from->lvs.n;

		list_del(lvh);
		list_add(&vg_to->lvs, lvh);
	}

	while (!list_empty(&vg_from->fid->metadata_areas)) {
		struct list *mdah = vg_from->fid->metadata_areas.n;

		list_del(mdah);
		list_add(&vg_to->fid->metadata_areas, mdah);
	}

	vg_to->lv_count += vg_from->lv_count;

	vg_to->extent_count += vg_from->extent_count;
	vg_to->free_count += vg_from->free_count;

	/* store it on disks */
	log_verbose("Writing out updated volume group");
	if (!vg_write(vg_to) || !vg_commit(vg_to)) {
		goto error;
	}

	/* FIXME Remove /dev/vgfrom */

	backup(vg_to);

	unlock_vg(cmd, vg_name_from);
	unlock_vg(cmd, vg_name_to);

	log_print("Volume group \"%s\" successfully merged into \"%s\"",
		  vg_from->name, vg_to->name);
	return ECMD_PROCESSED;

      error:
	unlock_vg(cmd, vg_name_from);
	unlock_vg(cmd, vg_name_to);
	return ECMD_FAILED;
}

int vgmerge(struct cmd_context *cmd, int argc, char **argv)
{
	char *vg_name_to;
	int opt = 0;
	int ret = 0, ret_max = 0;

	if (argc < 2) {
		log_error("Please enter 2 or more volume groups to merge");
		return EINVALID_CMD_LINE;
	}

	vg_name_to = argv[0];
	argc--;
	argv++;

	for (; opt < argc; opt++) {
		ret = _vgmerge_single(cmd, vg_name_to, argv[opt]);
		if (ret > ret_max)
			ret_max = ret;
	}

	return ret_max;
}
