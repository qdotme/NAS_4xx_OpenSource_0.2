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
#include "toolcontext.h"
#include "memlock.h"

#include <signal.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

static struct locking_type _locking;
static sigset_t _oldset;

static int _vg_lock_count = 0;		/* Number of locks held */
static int _vg_write_lock_held = 0;	/* VG write lock held? */
static int _signals_blocked = 0;

static void _block_signals(int flags)
{
	sigset_t set;

	if (_signals_blocked)
		return;

	if (sigfillset(&set)) {
		log_sys_error("sigfillset", "_block_signals");
		return;
	}

	if (sigprocmask(SIG_SETMASK, &set, &_oldset)) {
		log_sys_error("sigprocmask", "_block_signals");
		return;
	}

	_signals_blocked = 1;

	return;
}

static void _unblock_signals(void)
{
	/* Don't unblock signals while any locks are held */
	if (!_signals_blocked || _vg_lock_count)
		return;

	if (sigprocmask(SIG_SETMASK, &_oldset, NULL)) {
		log_sys_error("sigprocmask", "_block_signals");
		return;
	}

	_signals_blocked = 0;

	return;
}

static void _lock_memory(int flags)
{
	if (!(_locking.flags & LCK_PRE_MEMLOCK))
		return;

	if ((flags & (LCK_SCOPE_MASK | LCK_TYPE_MASK)) == LCK_LV_SUSPEND)
		memlock_inc();
}

static void _unlock_memory(int flags)
{
	if (!(_locking.flags & LCK_PRE_MEMLOCK))
		return;

	if ((flags & (LCK_SCOPE_MASK | LCK_TYPE_MASK)) == LCK_LV_RESUME)
		memlock_dec();
}

void reset_locking(void)
{
	int was_locked = _vg_lock_count;

	_vg_lock_count = 0;
	_vg_write_lock_held = 0;

	_locking.reset_locking();

	if (was_locked)
		_unblock_signals();
}

static inline void _update_vg_lock_count(int flags)
{
	if ((flags & LCK_SCOPE_MASK) != LCK_VG)
		return;

	if ((flags & LCK_TYPE_MASK) == LCK_UNLOCK)
		_vg_lock_count--;
	else
		_vg_lock_count++;

	/* We don't bother to reset this until all VG locks are dropped */
	if ((flags & LCK_TYPE_MASK) == LCK_WRITE)
		_vg_write_lock_held = 1;
	else if (!_vg_lock_count)
		_vg_write_lock_held = 0;
}

/*
 * Select a locking type
 */
int init_locking(int type, struct config_tree *cft)
{
	switch (type) {
	case 0:
		init_no_locking(&_locking, cft);
		log_print("WARNING: Locking disabled. Be careful! "
			  "This could corrupt your metadata.");
		return 1;

	case 1:
		if (!init_file_locking(&_locking, cft))
			break;
		log_very_verbose("File-based locking enabled.");
		return 1;

#ifdef HAVE_LIBDL
	case 2:
		if (!init_external_locking(&_locking, cft))
			break;
		log_very_verbose("External locking enabled.");
		return 1;
#endif

	default:
		log_error("Unknown locking type requested.");
		return 0;
	}

	if (!ignorelockingfailure())
		return 0;

	/* FIXME Ensure only read ops are permitted */
	log_verbose("Locking disabled - only read operations permitted.");

	init_no_locking(&_locking, cft);

	return 1;
}

void fin_locking(void)
{
	_locking.fin_locking();
}

/*
 * Does the LVM1 driver know of this VG name?
 */
int check_lvm1_vg_inactive(struct cmd_context *cmd, const char *vgname)
{
	struct stat info;
	char path[PATH_MAX];

	/* We'll allow operations on orphans */
	if (!*vgname)
		return 1;

	if (lvm_snprintf(path, sizeof(path), "%s/lvm/VGs/%s", cmd->proc_dir,
			 vgname) < 0) {
		log_error("LVM1 proc VG pathname too long for %s", vgname);
		return 0;
	}

	if (stat(path, &info) == 0) {
		log_error("%s exists: Is the original LVM driver using "
			  "this volume group?", path);
		return 0;
	} else if (errno != ENOENT && errno != ENOTDIR) {
		log_sys_error("stat", path);
		return 0;
	}

	return 1;
}

/*
 * VG locking is by VG name.
 * FIXME This should become VG uuid.
 */
static int _lock_vol(struct cmd_context *cmd, const char *resource, int flags)
{
	_block_signals(flags);
	_lock_memory(flags);

	if (!(_locking.lock_resource(cmd, resource, flags))) {
		_unlock_memory(flags);
		_unblock_signals();
		return 0;
	}

	_update_vg_lock_count(flags);
	_unlock_memory(flags);
	_unblock_signals();

	return 1;
}

int lock_vol(struct cmd_context *cmd, const char *vol, int flags)
{
	char resource[258];

	switch (flags & LCK_SCOPE_MASK) {
	case LCK_VG:
		/* Lock VG to change on-disk metadata. */
		/* If LVM1 driver knows about the VG, it can't be accessed. */
		if (!check_lvm1_vg_inactive(cmd, vol))
			return 0;
	case LCK_LV:
		/* Suspend LV if it's active. */
		strncpy(resource, vol, sizeof(resource));
		break;
	default:
		log_error("Unrecognised lock scope: %d",
			  flags & LCK_SCOPE_MASK);
		return 0;
	}

	if (!_lock_vol(cmd, resource, flags))
		return 0;

	/* Perform immediate unlock unless LCK_HOLD set */
	if (!(flags & LCK_HOLD) && ((flags & LCK_TYPE_MASK) != LCK_UNLOCK)) {
		if (!_lock_vol(cmd, resource,
			       (flags & ~LCK_TYPE_MASK) | LCK_UNLOCK))
			return 0;
	}

	return 1;
}

/* Unlock list of LVs */
int unlock_lvs(struct cmd_context *cmd, struct list *lvs)
{
	struct list *lvh;
	struct logical_volume *lv;

	list_iterate(lvh, lvs) {
		lv = list_item(lvh, struct lv_list)->lv;
		unlock_lv(cmd, lv->lvid.s);
	}

	return 1;
}

/* Lock a list of LVs */
int lock_lvs(struct cmd_context *cmd, struct list *lvs, int flags)
{
	struct list *lvh;
	struct logical_volume *lv;

	list_iterate(lvh, lvs) {
		lv = list_item(lvh, struct lv_list)->lv;
		if (!lock_vol(cmd, lv->lvid.s, flags)) {
			log_error("Failed to suspend %s", lv->name);
			list_uniterate(lvh, lvs, lvh) {
				lv = list_item(lvh, struct lv_list)->lv;
				unlock_lv(cmd, lv->lvid.s);
			}

			return 0;
		}
	}

	return 1;
}

int vg_write_lock_held(void)
{
	return _vg_write_lock_held;
}
