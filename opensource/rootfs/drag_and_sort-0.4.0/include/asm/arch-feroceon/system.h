/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include "../arch/arm/mach-feroceon/config/mvSysHwConfig.h"
/* versions */
#define TEST_UBOOT_VER 0x02030100 /* 2.3.1 */

#define LSP_VERSION "2.3.1_NAS_GDP_p4"


static inline void arch_idle(void)
{
	/*
	 * This should do all the clock switching
	 * and wait for interrupt tricks
	 */
	cpu_do_idle();
}


#ifdef __BIG_ENDIAN
#define MV_ARM_32BIT_LE(X) ((((X)&0xff)<<24) |                       \
                               (((X)mvBoardReset(void);

static inline void arch_reset(char mode)
{
	mvBoardPrepareReset();	
        printk("Reseting !! \n");
	m&0xff00)<<8) |                      \
                               (((X)&0xff0000)>>8) |                    \
                               (((X)&0xff000000)>>24))
#else
#define MV_ARM_32BIT_LE(X) (X)
#endif


void mvBoardReset(void);
void mvBoardPrepareReset(void);

static inline void arch_reset(char mode)
{
	mvBoardPrepareReset();	
        printk("Reseting !! \n");
	mvBoardReset();
}

#endif
