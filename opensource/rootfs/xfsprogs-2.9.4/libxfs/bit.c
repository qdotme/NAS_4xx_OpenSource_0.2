/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * XFS bit manipulation routines
 */

#include <xfs.h>

/*
 * Index of low bit number in byte, -1 for none set, 0..7 otherwise.
 */
static const char xfs_lowbit[256] = {
       -1, 0, 1, 0, 2, 0, 1, 0,			/* 00 .. 07 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 08 .. 0f */
	4, 0, 1, 0, 2, 0, 1, 0,			/* 10 .. 17 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 18 .. 1f */
	5, 0, 1, 0, 2, 0, 1, 0,			/* 20 .. 27 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 28 .. 2f */
	4, 0, 1, 0, 2, 0, 1, 0,			/* 30 .. 37 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 38 .. 3f */
	6, 0, 1, 0, 2, 0, 1, 0,			/* 40 .. 47 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 48 .. 4f */
	4, 0, 1, 0, 2, 0, 1, 0,			/* 50 .. 57 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 58 .. 5f */
	5, 0, 1, 0, 2, 0, 1, 0,			/* 60 .. 67 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 68 .. 6f */
	4, 0, 1, 0, 2, 0, 1, 0,			/* 70 .. 77 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 78 .. 7f */
	7, 0, 1, 0, 2, 0, 1, 0,			/* 80 .. 87 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 88 .. 8f */
	4, 0, 1, 0, 2, 0, 1, 0,			/* 90 .. 97 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* 98 .. 9f */
	5, 0, 1, 0, 2, 0, 1, 0,			/* a0 .. a7 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* a8 .. af */
	4, 0, 1, 0, 2, 0, 1, 0,			/* b0 .. b7 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* b8 .. bf */
	6, 0, 1, 0, 2, 0, 1, 0,			/* c0 .. c7 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* c8 .. cf */
	4, 0, 1, 0, 2, 0, 1, 0,			/* d0 .. d7 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* d8 .. df */
	5, 0, 1, 0, 2, 0, 1, 0,			/* e0 .. e7 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* e8 .. ef */
	4, 0, 1, 0, 2, 0, 1, 0,			/* f0 .. f7 */
	3, 0, 1, 0, 2, 0, 1, 0,			/* f8 .. ff */
};

/*
 * Index of high bit number in byte, -1 for none set, 0..7 otherwise.
 */
static const char xfs_highbit[256] = {
       -1, 0, 1, 1, 2, 2, 2, 2,			/* 00 .. 07 */
	3, 3, 3, 3, 3, 3, 3, 3,			/* 08 .. 0f */
	4, 4, 4, 4, 4, 4, 4, 4,			/* 10 .. 17 */
	4, 4, 4, 4, 4, 4, 4, 4,			/* 18 .. 1f */
	5, 5, 5, 5, 5, 5, 5, 5,			/* 20 .. 27 */
	5, 5, 5, 5, 5, 5, 5, 5,			/* 28 .. 2f */
	5, 5, 5, 5, 5, 5, 5, 5,			/* 30 .. 37 */
	5, 5, 5, 5, 5, 5, 5, 5,			/* 38 .. 3f */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 40 .. 47 */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 48 .. 4f */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 50 .. 57 */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 58 .. 5f */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 60 .. 67 */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 68 .. 6f */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 70 .. 77 */
	6, 6, 6, 6, 6, 6, 6, 6,			/* 78 .. 7f */
	7, 7, 7, 7, 7, 7, 7, 7,			/* 80 .. 87 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* 88 .. 8f */
	7, 7, 7, 7, 7, 7, 7, 7,			/* 90 .. 97 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* 98 .. 9f */
	7, 7, 7, 7, 7, 7, 7, 7,			/* a0 .. a7 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* a8 .. af */
	7, 7, 7, 7, 7, 7, 7, 7,			/* b0 .. b7 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* b8 .. bf */
	7, 7, 7, 7, 7, 7, 7, 7,			/* c0 .. c7 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* c8 .. cf */
	7, 7, 7, 7, 7, 7, 7, 7,			/* d0 .. d7 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* d8 .. df */
	7, 7, 7, 7, 7, 7, 7, 7,			/* e0 .. e7 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* e8 .. ef */
	7, 7, 7, 7, 7, 7, 7, 7,			/* f0 .. f7 */
	7, 7, 7, 7, 7, 7, 7, 7,			/* f8 .. ff */
};

/*
 * xfs_lowbit32: get low bit set out of 32-bit argument, -1 if none set.
 */
int
xfs_lowbit32(
	__uint32_t	v)
{
	int		i;

	if (v & 0x0000ffff)
		if (v & 0x000000ff)
			i = 0;
		else
			i = 8;
	else if (v & 0xffff0000)
		if (v & 0x00ff0000)
			i = 16;
		else
			i = 24;
	else
		return -1;
	return i + xfs_lowbit[(v >> i) & 0xff];
}

/*
 * xfs_highbit32: get high bit set out of 32-bit argument, -1 if none set.
 */
int
xfs_highbit32(
	__uint32_t	v)
{
	int		i;

	if (v & 0xffff0000)
		if (v & 0xff000000)
			i = 24;
		else
			i = 16;
	else if (v & 0x0000ffff)
		if (v & 0x0000ff00)
			i = 8;
		else
			i = 0;
	else
		return -1;
	return i + xfs_highbit[(v >> i) & 0xff];
}

/*
 * xfs_lowbit64: get low bit set out of 64-bit argument, -1 if none set.
 */
int
xfs_lowbit64(
	__uint64_t	v)
{
	int		i;
#if XFS_64
	if (v & 0x00000000ffffffff)
		if (v & 0x000000000000ffff)
			if (v & 0x00000000000000ff)
				i = 0;
			else
				i = 8;
		else
			if (v & 0x0000000000ff0000)
				i = 16;
			else
				i = 24;
	else if (v & 0xffffffff00000000)
		if (v & 0x0000ffff00000000)
			if (v & 0x000000ff00000000)
				i = 32;
			else
				i = 40;
		else
			if (v & 0x00ff000000000000)
				i = 48;
			else
				i = 56;
	else
		return -1;
	return i + xfs_lowbit[(v >> i) & 0xff];
#else
	__uint32_t	vw;

	if ((vw = v)) {
		if (vw & 0x0000ffff)
			if (vw & 0x000000ff)
				i = 0;
			else
				i = 8;
		else
			if (vw & 0x00ff0000)
				i = 16;
			else
				i = 24;
		return i + xfs_lowbit[(vw >> i) & 0xff];
	} else if ((vw = v >> 32)) {
		if (vw & 0x0000ffff)
			if (vw & 0x000000ff)
				i = 32;
			else
				i = 40;
		else
			if (vw & 0x00ff0000)
				i = 48;
			else
				i = 56;
		return i + xfs_lowbit[(vw >> (i - 32)) & 0xff];
	} else
		return -1;
#endif
}

/*
 * xfs_highbit64: get high bit set out of 64-bit argument, -1 if none set.
 */
int
xfs_highbit64(
	__uint64_t	v)
{
	int		i;
#if  XFS_64
	if (v & 0xffffffff00000000)
		if (v & 0xffff000000000000)
			if (v & 0xff00000000000000)
				i = 56;
			else
				i = 48;
		else
			if (v & 0x0000ff0000000000)
				i = 40;
			else
				i = 32;
	else if (v & 0x00000000ffffffff)
		if (v & 0x00000000ffff0000)
			if (v & 0x00000000ff000000)
				i = 24;
			else
				i = 16;
		else
			if (v & 0x000000000000ff00)
				i = 8;
			else
				i = 0;
	else
		return -1;
	return i + xfs_highbit[(v >> i) & 0xff];
#else
	__uint32_t	vw;

	if ((vw = v >> 32)) {
		if (vw & 0xffff0000)
			if (vw & 0xff000000)
				i = 56;
			else
				i = 48;
		else
			if (vw & 0x0000ff00)
				i = 40;
			else
				i = 32;
		return i + xfs_highbit[(vw >> (i - 32)) & 0xff];
	} else if ((vw = v)) {
		if (vw & 0xffff0000)
			if (vw & 0xff000000)
				i = 24;
			else
				i = 16;
		else
			if (vw & 0x0000ff00)
				i = 8;
			else
				i = 0;
		return i + xfs_highbit[(vw >> i) & 0xff];
	} else
		return -1;
#endif
}
