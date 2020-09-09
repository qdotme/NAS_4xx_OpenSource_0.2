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
 *
 * Assembler-only file 
 */
		.macro  addruart,rx
		mov	\rx, #0xF1000000	@ Base address for the Internal registers
		orr	\rx, \rx, #0x12000	@ Offset of the Uart TX register
		.endm

               .macro  senduart,rd,rx
                strb    \rd, [\rx]
                .endm

                .macro  waituart,rd,rx
1002:           ldrb    \rd, [\rx, #0x14]
                and     \rd, \rd, #0x60		@ check THRE and TEMT bits
                teq     \rd, #0x60
                bne     1002b
                .endm

                .macro  busyuart,rd,rx
                .endm
