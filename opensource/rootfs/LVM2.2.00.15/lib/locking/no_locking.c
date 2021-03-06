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

#include "lib.h"
#include "locking.h"
#include "locking_types.h"
#include "lvm-string.h"
#include "activate.h"
#include "lvmcache.h"

#include <signal.h>

/*
 * No locking
 */

static void _no_fin_locking(void)
{
	return;
}

static void _no_reset_locking(void)
{
	return;
}

static int _no_lock_resource(struct cmd_context *cmd, const char *resource,
			     int flags)
{
	switch (flags & LCK_SCOPE_MASK) {
	case LCK_VG:
		switch (flags & LCK_TYPE_MASK) {
		case LCK_UNLOCK:
			lvmcache_unlock_vgname(resource);
			break;
		default:
			lvmcache_lock_vgname(resource,
					     (flags & LCK_TYPE_MASK) ==
					     LCK_READ);
		}
		break;
	case LCK_LV:
		switch (flags & LCK_TYPE_MASK) {
		case LCK_UNLOCK:
			return lv_resume_if_active(cmd, resource);
		case LCK_READ:
			return lv_activate(cmd, resource);
		case LCK_WRITE:
			return lv_suspend_if_active(cmd, resource);
		case LCK_EXCL:
			return lv_deactivate(cmd, resource);
		default:
			break;
		}
		break;
	default:
		log_error("Unrecognised lock scope: %d",
			  flags & LCK_SCOPE_MASK);
		return 0;
	}

	return 1;
}

int init_no_locking(struct locking_type *locking, struct config_tree *cft)
{
	locking->lock_resource = _no_lock_resource;
	locking->reset_locking = _no_reset_locking;
	locking->fin_locking = _no_fin_locking;
	locking->flags = 0;

	return 1;
}
