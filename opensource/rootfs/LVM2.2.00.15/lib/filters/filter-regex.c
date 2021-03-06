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
#include "pool.h"
#include "filter-regex.h"
#include "matcher.h"
#include "device.h"
#include "bitset.h"
#include "list.h"

struct rfilter {
	struct pool *mem;
	bitset_t accept;
	struct matcher *engine;
};

static int _extract_pattern(struct pool *mem, const char *pat,
			    char **regex, bitset_t accept, int ix)
{
	char sep, *r, *ptr;

	/*
	 * is this an accept or reject pattern
	 */
	switch (*pat) {
	case 'a':
		bit_set(accept, ix);
		break;

	case 'r':
		bit_clear(accept, ix);
		break;

	default:
		log_info("pattern must begin with 'a' or 'r'");
		return 0;
	}
	pat++;

	/*
	 * get the separator
	 */
	switch (*pat) {
	case '(':
		sep = ')';
		break;

	case '[':
		sep = ']';
		break;

	case '{':
		sep = '}';
		break;

	default:
		sep = *pat;
	}
	pat++;

	/*
	 * copy the regex
	 */
	if (!(r = pool_strdup(mem, pat))) {
		stack;
		return 0;
	}

	/*
	 * trim the trailing character, having checked it's sep.
	 */
	ptr = r + strlen(r) - 1;
	if (*ptr != sep) {
		log_info("invalid separator at end of regex");
		return 0;
	}
	*ptr = '\0';

	regex[ix] = r;
	return 1;
}

static int _build_matcher(struct rfilter *rf, struct config_value *val)
{
	struct pool *scratch;
	struct config_value *v;
	char **regex;
	unsigned count = 0;
	int i, r = 0;

	if (!(scratch = pool_create(1024))) {
		stack;
		return 0;
	}

	/*
	 * count how many patterns we have.
	 */
	for (v = val; v; v = v->next) {

		if (v->type != CFG_STRING) {
			log_info("filter patterns must be enclosed in quotes");
			goto out;
		}

		count++;
	}

	/*
	 * allocate space for them
	 */
	if (!(regex = pool_alloc(scratch, sizeof(*regex) * count))) {
		stack;
		goto out;
	}

	/*
	 * create the accept/reject bitset
	 */
	rf->accept = bitset_create(rf->mem, count);

	/*
	 * fill the array back to front because we
	 * want the opposite precedence to what
	 * the matcher gives.
	 */
	for (v = val, i = count - 1; v; v = v->next, i--)
		if (!_extract_pattern(scratch, v->v.str, regex, rf->accept, i)) {
			log_info("invalid filter pattern");
			goto out;
		}

	/*
	 * build the matcher.
	 */
	if (!(rf->engine = matcher_create(rf->mem, (const char **) regex,
					  count)))
		stack;
	r = 1;

      out:
	pool_destroy(scratch);
	return r;
}

static int _accept_p(struct dev_filter *f, struct device *dev)
{
	struct list *ah;
	int m, first = 1, rejected = 0;
	struct rfilter *rf = (struct rfilter *) f->private;
	struct str_list *sl;

	list_iterate(ah, &dev->aliases) {
		sl = list_item(ah, struct str_list);
		m = matcher_run(rf->engine, sl->str);

		if (m >= 0) {
			if (bit(rf->accept, m)) {

				if (!first) {
					log_debug("%s: New preferred name",
						  sl->str);
					list_del(&sl->list);
					list_add_h(&dev->aliases, &sl->list);
				}

				return 1;
			}

			rejected = 1;
		}

		first = 0;
	}

	/*
	 * pass everything that doesn't match
	 * anything.
	 */
	return !rejected;
}

static void _destroy(struct dev_filter *f)
{
	struct rfilter *rf = (struct rfilter *) f->private;
	pool_destroy(rf->mem);
}

struct dev_filter *regex_filter_create(struct config_value *patterns)
{
	struct pool *mem = pool_create(10 * 1024);
	struct rfilter *rf;
	struct dev_filter *f;

	if (!mem) {
		stack;
		return NULL;
	}

	if (!(rf = pool_alloc(mem, sizeof(*rf)))) {
		stack;
		goto bad;
	}

	rf->mem = mem;

	if (!_build_matcher(rf, patterns)) {
		stack;
		goto bad;
	}

	if (!(f = pool_zalloc(mem, sizeof(*f)))) {
		stack;
		goto bad;
	}

	f->passes_filter = _accept_p;
	f->destroy = _destroy;
	f->private = rf;
	return f;

      bad:
	pool_destroy(mem);
	return NULL;
}
