/*
 *  linux/include/asm-arm/arch-integrator/serial.h
 *
 *  Copyright (C) 1999 ARM Limited
 *
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
#ifndef __ASM_ARCH_SERIAL_H
#define __ASM_ARCH_SERIAL_H

#include <asm/irq.h>
#include <linux/autoconf.h>
#include <linux/config.h>

#include "../arch/arm/mach-feroceon/config/mvSysHwConfig.h"

extern unsigned int mvTclk;

#undef  BASE_BAUD
#define BASE_BAUD (mvTclk / 16)

#define PORT0_BASE	(INTER_REGS_BASE + 0x12000) /* port 0 base */
#define PORT1_BASE 	(INTER_REGS_BASE + 0x12100) /* port 1 base */

#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST /* | ASYNC_SPD_VHI  115200 */ )

#ifdef CONFIG_KGDB_8250
/* Replaced with the dynamic code in LSP/core.c. This is used only for the purpose of debugging with KGDB over the serial line */
/* At this stage the TCLK is not yet calculated so we assume a frequency of 166MHz */
#define KGDB_BASE_BAUD (133333333/16)
     /* UART CLK        PORT  IRQ     FLAGS        */
#define STD_SERIAL_PORT_DEFNS \
	{ 0, KGDB_BASE_BAUD, PORT0_BASE, IRQ_UART0, STD_COM_FLAGS, iomem_reg_shift: 2, io_type: SERIAL_IO_MEM , iomem_base: (u8 *)PORT0_BASE},	/* ttyS0 */ \
	{ 1, KGDB_BASE_BAUD, PORT1_BASE, IRQ_UART1, STD_COM_FLAGS,iomem_reg_shift: 2, io_type: SERIAL_IO_MEM , iomem_base: (u8 *)PORT1_BASE},	 /* ttyS1  */

#else /* CONFIG_KGDB_8250 */
#define STD_SERIAL_PORT_DEFNS
#endif /* CONFIG_KGDB_8250 */

#define EXTRA_SERIAL_PORT_DEFNS

#endif
