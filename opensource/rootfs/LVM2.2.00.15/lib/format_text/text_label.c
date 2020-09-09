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

#include "lib.h"
#include "format-text.h"
#include "layout.h"
#include "label.h"
#include "xlate.h"

#include <sys/stat.h>
#include <fcntl.h>

static int _can_handle(struct labeller *l, char *buf, uint64_t sector)
{
	struct label_header *lh = (struct label_header *) buf;

	if (!strncmp(lh->type, LVM2_LABEL, sizeof(lh->type)))
		return 1;

	return 0;
}

static int _write(struct label *label, char *buf)
{
	struct label_header *lh = (struct label_header *) buf;
	struct pv_header *pvhdr;
	struct lvmcache_info *info;
	struct disk_locn *pvh_dlocn_xl;
	struct list *mdash, *dash;
	struct metadata_area *mda;
	struct mda_context *mdac;
	struct data_area_list *da;

	/* FIXME Move to where label is created */
	strncpy(label->type, LVM2_LABEL, sizeof(label->type));

	strncpy(lh->type, label->type, sizeof(label->type));

	pvhdr = (struct pv_header *) ((void *) buf + xlate32(lh->offset_xl));
	info = (struct lvmcache_info *) label->info;
	pvhdr->device_size_xl = xlate64(info->device_size);
	memcpy(pvhdr->pv_uuid, &info->dev->pvid, sizeof(struct id));

	pvh_dlocn_xl = &pvhdr->disk_areas_xl[0];

	/* List of data areas (holding PEs) */
	list_iterate(dash, &info->das) {
		da = list_item(dash, struct data_area_list);

		pvh_dlocn_xl->offset = xlate64(da->disk_locn.offset);
		pvh_dlocn_xl->size = xlate64(da->disk_locn.size);
		pvh_dlocn_xl++;
	}

	/* NULL-termination */
	pvh_dlocn_xl->offset = xlate64(UINT64_C(0));
	pvh_dlocn_xl->size = xlate64(UINT64_C(0));
	pvh_dlocn_xl++;

	/* List of metadata area header locations */
	list_iterate(mdash, &info->mdas) {
		mda = list_item(mdash, struct metadata_area);
		mdac = (struct mda_context *) mda->metadata_locn;

		if (mdac->area.dev != info->dev)
			continue;

		pvh_dlocn_xl->offset = xlate64(mdac->area.start);
		pvh_dlocn_xl->size = xlate64(mdac->area.size);
		pvh_dlocn_xl++;
	}

	/* NULL-termination */
	pvh_dlocn_xl->offset = xlate64(UINT64_C(0));
	pvh_dlocn_xl->size = xlate64(UINT64_C(0));

	return 1;
}

int add_da(const struct format_type *fmt, struct pool *mem, struct list *das,
	   uint64_t start, uint64_t size)
{
	struct data_area_list *dal;

	if (!mem) {
		if (!(dal = dbg_malloc(sizeof(*dal)))) {
			log_error("struct data_area_list allocation failed");
			return 0;
		}
	} else {
		if (!(dal = pool_alloc(mem, sizeof(*dal)))) {
			log_error("struct data_area_list allocation failed");
			return 0;
		}
	}

	dal->disk_locn.offset = start;
	dal->disk_locn.size = size;

	list_add(das, &dal->list);

	return 1;
}

void del_das(struct list *das)
{
	struct list *dah, *tmp;
	struct data_area_list *da;

	list_iterate_safe(dah, tmp, das) {
		da = list_item(dah, struct data_area_list);
		list_del(&da->list);
		dbg_free(da);
	}
}

int add_mda(const struct format_type *fmt, struct pool *mem, struct list *mdas,
	    struct device *dev, uint64_t start, uint64_t size)
{
/* FIXME List size restricted by pv_header SECTOR_SIZE */
	struct metadata_area *mdal;
	struct mda_lists *mda_lists = (struct mda_lists *) fmt->private;
	struct mda_context *mdac;

	if (!mem) {
		if (!(mdal = dbg_malloc(sizeof(struct metadata_area)))) {
			log_error("struct mda_list allocation failed");
			return 0;
		}

		if (!(mdac = dbg_malloc(sizeof(struct mda_context)))) {
			log_error("struct mda_context allocation failed");
			dbg_free(mdal);
			return 0;
		}
	} else {
		if (!(mdal = pool_alloc(mem, sizeof(struct metadata_area)))) {
			log_error("struct mda_list allocation failed");
			return 0;
		}

		if (!(mdac = pool_alloc(mem, sizeof(struct mda_context)))) {
			log_error("struct mda_context allocation failed");
			return 0;
		}
	}

	mdal->ops = mda_lists->raw_ops;
	mdal->metadata_locn = mdac;

	mdac->area.dev = dev;
	mdac->area.start = start;
	mdac->area.size = size;
	memset(&mdac->rlocn, 0, sizeof(mdac->rlocn));

	list_add(mdas, &mdal->list);
	return 1;
}

void del_mdas(struct list *mdas)
{
	struct list *mdah, *tmp;
	struct metadata_area *mda;

	list_iterate_safe(mdah, tmp, mdas) {
		mda = list_item(mdah, struct metadata_area);
		dbg_free(mda->metadata_locn);
		list_del(&mda->list);
		dbg_free(mda);
	}
}

static int _initialise_label(struct labeller *l, struct label *label)
{
	strncpy(label->type, LVM2_LABEL, sizeof(label->type));

	return 1;
}

static int _read(struct labeller *l, struct device *dev, char *buf,
		 struct label **label)
{
	struct label_header *lh = (struct label_header *) buf;
	struct pv_header *pvhdr;
	struct lvmcache_info *info;
	struct disk_locn *dlocn_xl;
	uint64_t offset;
	struct list *mdah;
	struct metadata_area *mda;
	char vgnamebuf[NAME_LEN + 2];
	struct mda_context *mdac;

	pvhdr = (struct pv_header *) ((void *) buf + xlate32(lh->offset_xl));

	if (!(info = lvmcache_add(l, pvhdr->pv_uuid, dev, NULL, NULL)))
		return 0;
	*label = info->label;

	info->device_size = xlate64(pvhdr->device_size_xl);

	if (info->das.n)
		del_das(&info->das);
	list_init(&info->das);

	if (info->mdas.n)
		del_mdas(&info->mdas);
	list_init(&info->mdas);

	/* Data areas holding the PEs */
	dlocn_xl = pvhdr->disk_areas_xl;
	while ((offset = xlate64(dlocn_xl->offset))) {
		add_da(info->fmt, NULL, &info->das, offset,
		       xlate64(dlocn_xl->size));
		dlocn_xl++;
	}

	/* Metadata area headers */
	dlocn_xl++;
	while ((offset = xlate64(dlocn_xl->offset))) {
		add_mda(info->fmt, NULL, &info->mdas, dev, offset,
			xlate64(dlocn_xl->size));
		dlocn_xl++;
	}

	list_iterate(mdah, &info->mdas) {
		mda = list_item(mdah, struct metadata_area);
		mdac = (struct mda_context *) mda->metadata_locn;
		if (vgname_from_mda(info->fmt, &mdac->area, vgnamebuf,
				    sizeof(vgnamebuf))) {
			lvmcache_update_vgname(info, vgnamebuf);
		}
	}

	info->status &= ~CACHE_INVALID;

	return 1;
}

static void _destroy_label(struct labeller *l, struct label *label)
{
	struct lvmcache_info *info = (struct lvmcache_info *) label->info;

	if (info->mdas.n)
		del_mdas(&info->mdas);
	if (info->das.n)
		del_das(&info->das);
}

static void _destroy(struct labeller *l)
{
	dbg_free(l);
}

struct label_ops _text_ops = {
	can_handle:_can_handle,
	write:_write,
	read:_read,
	verify:_can_handle,
	initialise_label:_initialise_label,
	destroy_label:_destroy_label,
	destroy:_destroy
};

struct labeller *text_labeller_create(const struct format_type *fmt)
{
	struct labeller *l;

	if (!(l = dbg_malloc(sizeof(*l)))) {
		log_err("Couldn't allocate labeller object.");
		return NULL;
	}

	l->ops = &_text_ops;
	l->private = (const void *) fmt;

	return l;
}
