/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*******************************************************************************
Copyright (C) Marvell International Ltd. and its affiliates

********************************************************************************
Marvell GPL License Option

If you received this File from Marvell, you may opt to use, redistribute and/or 
modify this File in accordance with the terms and conditions of the General 
Public License Version 2, June 1991 (the "GPL License"), a copy of which is 
available along with the File in the license.txt file or by writing to the Free 
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or 
on the worldwide web at http://www.gnu.org/licenses/gpl.txt. 

THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED 
WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY 
DISCLAIMED.  The GPL License provides additional details about this warranty 
disclaimer.

*******************************************************************************/

#include <common.h>
#include "mvTypes.h"
#include "mvBoardEnvLib.h"
#include "mvCpuIf.h"
#include "mvCtrlEnvLib.h"
#include "mv_mon_init.h"
#include "mvDebug.h"
#include "device/mvDevice.h"
#include "twsi/mvTwsi.h"
#include "eth/mvEth.h"
#include "pex/mvPex.h"
#include "gpp/mvGpp.h"
#include "sys/mvSysUsb.h"

#ifdef MV_INCLUDE_RTC
#include "rtc/integ_rtc/mvRtc.h"
#elif CONFIG_RTC_DS1338_DS1339
#include "rtc/ext_rtc/mvDS133x.h"
#endif

#if defined(MV_INCLUDE_XOR)
#include "xor/mvXor.h"
#endif
#if defined(MV_INCLUDE_IDMA)
#include "sys/mvSysIdma.h"
#include "idma/mvIdma.h"
#endif
#if defined(MV_INCLUDE_USB)
#include "usb/mvUsb.h"
#endif

#include "cpu/mvCpu.h"

#ifdef CONFIG_PCI
#   include <pci.h>
#endif
#include "pci/mvPciRegs.h"

#include <asm/arch/vfpinstr.h>
#include <asm/arch/vfp.h>

#include "net.h"
#include <command.h>

/* #define MV_DEBUG */
#ifdef MV_DEBUG
#define DB(x) x
#else
#define DB(x)
#endif

#ifdef SHASTA_BOARD
#include <jffs2/jffs2.h>
#include <nand.h>
#include "gpp/lcm-def.h"
#define GPP_GROUP(gpp)  gpp/32
#define GPP_ID(gpp)     gpp%32
#define GPP_BIT(gpp)    0x1 << GPP_ID(gpp)
#endif

/* CPU address decode table. */
MV_CPU_DEC_WIN mvCpuAddrWinMap[] = MV_CPU_IF_ADDR_WIN_MAP_TBL;

#if defined(RD_88F6281A) || defined(RD_88F6192A) || defined(RD_88F6190A)
static void mvHddPowerCtrl(void);
#endif

#if (CONFIG_COMMANDS & CFG_CMD_RCVR)
static void recoveryDetection(void);
void recoveryHandle(void);
static u32 rcvrflag = 0;
#endif
void mv_cpu_init(void);
#if defined(MV_INCLUDE_CLK_PWR_CNTRL)
int mv_set_power_scheme(void);
#endif

#ifdef	CFG_FLASH_CFI_DRIVER
MV_VOID mvUpdateNorFlashBaseAddrBank(MV_VOID);
int mv_board_num_flash_banks;
extern flash_info_t	flash_info[]; /* info for FLASH chips */
extern unsigned long flash_add_base_addr (uint flash_index, ulong flash_base_addr);
#endif	/* CFG_FLASH_CFI_DRIVER */

#if defined(MV_INCLUDE_UNM_ETH) || defined(MV_INCLUDE_GIG_ETH)
extern MV_VOID mvBoardEgigaPhySwitchInit(void);
#endif 

#if (CONFIG_COMMANDS & CFG_CMD_NAND)
/* Define for SDK 2.0 */
int __aeabi_unwind_cpp_pr0(int a,int b,int c) {return 0;}
int __aeabi_unwind_cpp_pr1(int a,int b,int c) {return 0;}
#endif

MV_VOID mvMppModuleTypePrint(MV_VOID);

/* Define for SDK 2.0 */
int raise(void) {return 0;}

void print_mvBanner(void)
{
#ifdef CONFIG_SILENT_CONSOLE
	DECLARE_GLOBAL_DATA_PTR;
	gd->flags |= GD_FLG_SILENT;
#endif
	printf("\n");
	printf("         __  __                      _ _\n");
	printf("        |  \\/  | __ _ _ ____   _____| | |\n");
	printf("        | |\\/| |/ _` | '__\\ \\ / / _ \\ | |\n");
	printf("        | |  | | (_| | |   \\ V /  __/ | |\n");
	printf("        |_|  |_|\\__,_|_|    \\_/ \\___|_|_|\n");
	printf(" _   _     ____              _\n");
	printf("| | | |   | __ )  ___   ___ | |_ \n");
	printf("| | | |___|  _ \\ / _ \\ / _ \\| __| \n");
	printf("| |_| |___| |_) | (_) | (_) | |_ \n");
	printf(" \\___/    |____/ \\___/ \\___/ \\__| ");
#if !defined(MV_NAND_BOOT)
#if defined(MV_INCLUDE_MONT_EXT)
    mvMPPConfigToSPI();
	if(!enaMonExt())
		printf(" ** LOADER **"); 
	else
		printf(" ** MONITOR **");
    mvMPPConfigToDefault();
#else

	printf(" ** Forcing LOADER mode only **"); 
#endif /* MV_INCLUDE_MONT_EXT */
#endif

#ifdef SHASTA_BOARD
#define UBOOT_FW_VER	"v0.1.6" 
{
	int i;
	printf(" ** uboot_ver:"UBOOT_FW_VER" **\n"); 
        /* add 2.5 seconds delay time for HDD */
        for ( i = 0 ; i<2500; i++)
              udelay(1000);
}
	/* powerup second HDD */
        mvGppTypeSet(GPP_GROUP(28), GPP_BIT(28),(1 << GPP_ID(28)) & MV_GPP_OUT );
	mvGppValueSet(GPP_GROUP(28), GPP_BIT(28), 1 << GPP_ID(28));
	
#endif

	return;
}

void print_dev_id(void){
	static char boardName[30];

	mvBoardNameGet(boardName);
	
#if defined(MV_CPU_BE)
	printf("\n ** MARVELL BOARD: %s BE ",boardName);
#else
	printf("\n ** MARVELL BOARD: %s LE ",boardName);
#endif

    return;
}


void maskAllInt(void)
{
        /* mask all external interrupt sources */
        MV_REG_WRITE(CPU_MAIN_IRQ_MASK_REG, 0);
        MV_REG_WRITE(CPU_MAIN_FIQ_MASK_REG, 0);
        MV_REG_WRITE(CPU_ENPOINT_MASK_REG, 0);
        MV_REG_WRITE(CPU_MAIN_IRQ_MASK_HIGH_REG, 0);
        MV_REG_WRITE(CPU_MAIN_FIQ_MASK_HIGH_REG, 0);
        MV_REG_WRITE(CPU_ENPOINT_MASK_HIGH_REG, 0);
}

/* init for the Master*/
void misc_init_r_dec_win(void)
{
#if defined(MV_INCLUDE_USB)
	{
		char *env;

		env = getenv("usb0Mode");
		if((!env) || (strcmp(env,"device") == 0) || (strcmp(env,"Device") == 0) )
		{
			printf("USB 0: device mode\n");	
			mvUsbInit(0, MV_FALSE);
		}
		else
		{
			printf("USB 0: host mode\n");	
			mvUsbInit(0, MV_TRUE);
		}
	}
#endif/* #if defined(MV_INCLUDE_USB) */

#if defined(MV_INCLUDE_XOR)
	mvXorInit();
#endif

#if defined(MV_INCLUDE_CLK_PWR_CNTRL)
	mv_set_power_scheme();
#endif

    return;
}


/*
 * Miscellaneous platform dependent initialisations
 */

extern	MV_STATUS mvEthPhyRegRead(MV_U32 phyAddr, MV_U32 regOffs, MV_U16 *data);
extern	MV_STATUS mvEthPhyRegWrite(MV_U32 phyAddr, MV_U32 regOffs, MV_U16 data);

/* golabal mac address for yukon EC */
unsigned char yuk_enetaddr[6];
extern int interrupt_init (void);
extern void i2c_init(int speed, int slaveaddr);

#if defined(SHASTA_BOARD)

#define LCM_BUS_E       45      //MPP45 use for E
#define E_ENGAGE        1       //engage level high

#define LCM_BUS_RW      44      //MPP44 use for R/W
#define WRITE_LEVEL     0
#define READ_LEVEL      !WRITE_LEVEL

//BOARD_1A #define LCM_BUS_RS      17      //MPP17 use for RS
static int LCM_BUS_RS = 35;	//BOARD_1A

#define LCM_DATA_BITS   8
// Data pin mapping
//                                 D0  D1  D2  D3  D4  D5  D6  D7
//--------------------------------------------------------------
static int data_pin_mapping[8] = { 36, 37, 38, 39, 40, 41, 42, 43 };

typedef struct wix_lcm_ioctl_s
{
        unsigned char rs;
        unsigned char data;
} wix_lcm_ioctl_t;

static int lcm_data_bus_mode = -1;

void set_data_bus_mode(int mode)
{
        int     i;

        if ( lcm_data_bus_mode  != mode) {
                for ( i=0; i<LCM_DATA_BITS; i++) {
                    mvGppTypeSet(GPP_GROUP(data_pin_mapping[i]), \
                         GPP_BIT(data_pin_mapping[i]), \
                         (1 << GPP_ID(data_pin_mapping[i])) \
                         & (mode ? MV_GPP_IN : MV_GPP_OUT ));
                }
                lcm_data_bus_mode = mode;
        }

}
void write_lcm(wix_lcm_ioctl_t *Info)
{
        int i;
        unsigned char tmp;

        set_data_bus_mode(0);

        tmp = Info->data;
        //set E pin down
         mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), 0);

        //prepare data
        for ( i=0; i<LCM_DATA_BITS; i++) {
                mvGppValueSet(GPP_GROUP(data_pin_mapping[i]),
                              GPP_BIT(data_pin_mapping[i]),
                                (tmp&0x01) << GPP_ID(data_pin_mapping[i]));
                tmp = tmp >> 1;
        }

        //prepare RS
         mvGppValueSet(GPP_GROUP(LCM_BUS_RS), GPP_BIT(LCM_BUS_RS), ((Info->rs & 0x01) << GPP_ID(LCM_BUS_RS)) );
        udelay(10);
        //prepare R/W
         mvGppValueSet(GPP_GROUP(LCM_BUS_RW), GPP_BIT(LCM_BUS_RW), ((WRITE_LEVEL&0x01) << GPP_ID(LCM_BUS_RW)) );

        udelay(10);
        //issue E
         mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), (1 << GPP_ID(LCM_BUS_E)) );

        //delay for it completed
        udelay(100);
        //de-issue E
        mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), ( 0 << GPP_ID(LCM_BUS_E)));

        //delay for it completed
        udelay(100);

        return;
}
/* LCM initial code */
#define INIT_PACKAGE_SIZE       3
 
LCM_CMD_PACKET initial_lcm_pkg[INIT_PACKAGE_SIZE]={
                {CMD_FUNC_SET|CMD_FUNC_SET_DL_8|CMD_FUNC_SET_N_2|CMD_FUNC_SET_F_8, RW_WRITE, REG_SEL_CMD},
                {CMD_DISP_CNTL|CMD_DISP_CTRL_D_ON, RW_WRITE, REG_SEL_CMD},
                {CMD_CLR, RW_WRITE, REG_SEL_CMD},
        };

int delay_tbl[8]={2000,2000,50,50,50,50,50,50};
/* lcm_delay */
/* This code will add delay for LCM MCU processing */
int lcm_delay(LCM_CMD_PACKET *pkg)
{
        int bit_locate;

        if ( (pkg->rw == RW_READ) || (pkg->rs == REG_SEL_DDRAM)){
                //need delay 43us at least
                udelay(50 );
        } else {
                bit_locate=7;
                if ( pkg->data == 0) bit_locate=0;
                else
                    do {
                        if( pkg->data >> bit_locate ) break;
                        bit_locate--;
                    } while (bit_locate >=0 );
                udelay(delay_tbl[bit_locate] );
        }
}

int send_pkg(LCM_CMD_PACKET *pkg)
{
        int     ret;
	wix_lcm_ioctl_t buf;

        buf.rs = pkg->rs;
        buf.data = pkg->data;
	write_lcm(&buf);
        lcm_delay(pkg);
        return ret;

}


int initialize_lcm(void)
{
        int     i;
        int     ret;

        for (i = 0; i< INIT_PACKAGE_SIZE; i++) {
                ret=send_pkg(&initial_lcm_pkg[i]);
                if (ret < 0 ) break;
        }
        return ret;
}

int set_ddram_address(int xx, int yy)
{
        LCM_CMD_PACKET pkg;
        int ret;

        pkg.rs=REG_SEL_CMD;
        pkg.rw=RW_WRITE;
        pkg.data= CMD_SET_DDRAM_ADDR |
                  (yy * LCM_ACTUALLY_ROW_LEN) + xx;
        ret=send_pkg(&pkg);
        return ret;
}

void show_lcm(char *buf1, char *buf2) {
	int i;
        LCM_CMD_PACKET pkg;
        int ret;

	set_ddram_address(0,0);

        for ( i=0; i<LCM_DISP_ROW_LEN; i++) {
                pkg.rw=RW_WRITE;
                pkg.data=buf1[i];
                pkg.rs=REG_SEL_DDRAM;
                ret=send_pkg(&pkg);
                if (ret < 0 ) break;
        }
	set_ddram_address(0,1);
        for ( i=0; i<LCM_DISP_ROW_LEN; i++) {
                pkg.rw=RW_WRITE;
                pkg.data=buf2[i];
                pkg.rs=REG_SEL_DDRAM;
                ret=send_pkg(&pkg);
                if (ret < 0 ) break;
        }
}

#define SHIFT_DATA      12      //MPP12 use data in
#define SHIFT_CLK       13      //MPP13 use data in

void set_led(unsigned char ttt)
{
	int i;
	for ( i=0; i<8; i++) {

        mvGppValueSet(GPP_GROUP(SHIFT_DATA),\
        GPP_BIT(SHIFT_DATA),(ttt&0x01) << GPP_ID(SHIFT_DATA));

        mvGppValueSet(GPP_GROUP(SHIFT_CLK), \
        GPP_BIT(SHIFT_CLK),1 << GPP_ID(SHIFT_CLK));

        mvGppValueSet(GPP_GROUP(SHIFT_CLK),\
        GPP_BIT(SHIFT_CLK),0 << GPP_ID(SHIFT_CLK));

        ttt = ttt >> 1;
        }
}
void set_shift_led_default(void)
{
	int i;
	unsigned char ttt;

	ttt=0x81;
        mvGppTypeSet(GPP_GROUP(SHIFT_CLK), GPP_BIT(SHIFT_CLK), \
                         (1 << GPP_ID(SHIFT_CLK)) & MV_GPP_OUT );
        mvGppTypeSet(GPP_GROUP(SHIFT_DATA), GPP_BIT(SHIFT_DATA), \
                         (1 << GPP_ID(SHIFT_DATA)) & MV_GPP_OUT );
        mvGppValueSet(GPP_GROUP(SHIFT_CLK), GPP_BIT(SHIFT_CLK), 0);

	set_led(ttt);

}

void	display_lcm(void)
{
	char row1_buf[17], row2_buf[17];
        //set E pin to output mode
        mvGppTypeSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), \
                         (1 << GPP_ID(LCM_BUS_E)) & MV_GPP_OUT );

        //set RW pin to output mode
        mvGppTypeSet(GPP_GROUP(LCM_BUS_RW), GPP_BIT(LCM_BUS_RW), \
                         (1 << GPP_ID(LCM_BUS_RW)) & MV_GPP_OUT );

        //set RS pin to output mode
        mvGppTypeSet(GPP_GROUP(LCM_BUS_RS), GPP_BIT(LCM_BUS_RS), \
                         (1 << GPP_ID(LCM_BUS_RS)) & MV_GPP_OUT );

        //set E pin down
        mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), 0);
        lcm_data_bus_mode = -1; //unknow mode

	initialize_lcm();
	//	          "0123456789abcdef"
	sprintf(row1_buf, "   Seagate      ");
	sprintf(row2_buf, "   Booting...   ");
	show_lcm(row1_buf, row2_buf);
}
//BOARD_1A +
void change_back_to_sample_b(void) {
 
        MV_U32 tmpVal = 0;
 
	//Set MPP14 as GPIO (switch button down)
	tmpVal = MV_REG_READ(mvCtrlMppRegGet(1));
	tmpVal &= 0xf0ffffff;
	MV_REG_WRITE(mvCtrlMppRegGet(1), tmpVal);
	//Set MPP17 as GPIO (LCM RS)
	tmpVal = MV_REG_READ(mvCtrlMppRegGet(2));
	tmpVal &= 0xffffff0f;
	MV_REG_WRITE(mvCtrlMppRegGet(2), tmpVal);
	LCM_BUS_RS = 17;

}
//BOARD_1A -
#endif
int board_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;
#if defined(MV_INCLUDE_TWSI)
	MV_TWSI_ADDR slave;
#endif
	unsigned int i;

	maskAllInt();

	/* must initialize the int in order for udelay to work */
	interrupt_init();

#if defined(MV_INCLUDE_TWSI)
	slave.type = ADDR7_BIT;
	slave.address = 0;
	mvTwsiInit(0, CFG_I2C_SPEED, CFG_TCLK, &slave, 0);
#endif
 
	/* Init the Board environment module (device bank params init) */
	mvBoardEnvInit();
   	
	/* Init the Controlloer environment module (MPP init) */
	mvCtrlEnvInit();

	mvBoardDebugLed(3);

        /* Init the Controller CPU interface */
	mvCpuIfInit(mvCpuAddrWinMap);

        /* arch number of Integrator Board */
        gd->bd->bi_arch_number = 527;
 
        /* adress of boot parameters */
        gd->bd->bi_boot_params = 0x00000100;

	/* relocate the exception vectors */
	/* U-Boot is running from DRAM at this stage */
	for(i = 0; i < 0x100; i+=4)
	{
		*(unsigned int *)(0x0 + i) = *(unsigned int*)(TEXT_BASE + i);
	}
	
	/* Update NOR flash base address bank for CFI driver */
#ifdef	CFG_FLASH_CFI_DRIVER
	mvUpdateNorFlashBaseAddrBank();
#endif	/* CFG_FLASH_CFI_DRIVER */

#if defined(MV_INCLUDE_UNM_ETH) || defined(MV_INCLUDE_GIG_ETH)
	/* Init the PHY or Switch of the board */
	mvBoardEgigaPhySwitchInit();
#endif /* #if defined(MV_INCLUDE_UNM_ETH) || defined(MV_INCLUDE_GIG_ETH) */

	mvBoardDebugLed(4);
#ifdef SHASTA_BOARD
	set_shift_led_default();
//BOARD_1A	display_lcm();
#endif	
	return 0;
}
#ifdef SHASTA_BOARD
extern nand_info_t nand_info[];       /* info for NAND chips */
unsigned long prer_off_addr;
unsigned long krnl_off_addr;
unsigned long root_off_addr;
/* move to include/nand.h
typedef struct {
	char sign[4];
	unsigned long start_addr;
	unsigned long len;
	unsigned long partition_size;
	char env_name[10];
	unsigned long actually_offset;
	unsigned long crc32_data;
	
} nand_layout_t;
*/
nand_layout_t nand_layout_info[]= {
			{"PrEr", 0xc0000, 0x200000, 0x200000, "prer_off", 0, 0},
			{"KrNl", 0x2c0000, 0x200000, 0x280000, "krnl_off", 0, 0},
			{"RoOt", 0x540000, 0x200000, 0x1a00000, "root_off", 0, 0},
//			{"MISC", 0x100000, 0x100000},
			{"NULL", 0,0, 0, 0, 0},
			};
		
#endif
void misc_init_r_env(void){
        char *env;
        char tmp_buf[10];
        unsigned int malloc_len;
        DECLARE_GLOBAL_DATA_PTR;

        unsigned int flashSize =0 , secSize =0, ubootSize =0;
	char buff[256];

#ifdef SHASTA_BOARD
        nand_info_t *nand;
 	nand_read_options_t opts;
	u_char *buf;
	nand_layout_t *nand_layout_ptr, *nand_layout_ptr_next;
	int sign_found;
	unsigned long *lll_ptr;
        u_char tmp_str[10];

	buf = malloc(nand->oobblock + nand->oobsize);
#endif
	
#if defined(MV_BOOTSIZE_4M)
	flashSize = _4M;
#elif defined(MV_BOOTSIZE_8M)
	flashSize = _8M;
#elif defined(MV_BOOTSIZE_16M)
	flashSize = _16M;
#elif defined(MV_BOOTSIZE_32M)
	flashSize = _32M;
#elif defined(MV_BOOTSIZE_64M)
	flashSize = _64M;
#endif

#if defined(MV_SEC_64K)
	secSize = _64K;
#if defined(MV_TINY_IMAGE)
	ubootSize = _256K;
#else
	ubootSize = _512K;
#endif
#elif defined(MV_SEC_128K)
	secSize = _128K;
#if defined(MV_TINY_IMAGE)
	ubootSize = _128K * 3;
#else
	ubootSize = _128K * 5;
#endif
#elif defined(MV_SEC_256K)
	secSize = _256K;
#if defined(MV_TINY_IMAGE)
	ubootSize = _256K * 3;
#else
	ubootSize = _256K * 3;
#endif
#endif

	if ((0 == flashSize) || (0 == secSize) || (0 == ubootSize))
	{
		env = getenv("console");
		if(!env)
			setenv("console","console=ttyS0,115200");
	}
#ifndef SHASTA_BOARD
	else
	{
		sprintf(buff,"console=ttyS0,115200 mtdparts=cfi_flash:0x%x(root),0x%x(uboot)ro",
			flashSize - ubootSize, ubootSize);
		env = getenv("console");
		if(!env)
			setenv("console",buff);
	}
#endif
		
        /* Linux open port support */
	env = getenv("mainlineLinux");
	if(env && ((strcmp(env,"yes") == 0) ||  (strcmp(env,"Yes") == 0)))
		setenv("mainlineLinux","yes");
	else
		setenv("mainlineLinux","no");

	env = getenv("mainlineLinux");
	if(env && ((strcmp(env,"yes") == 0) ||  (strcmp(env,"Yes") == 0)))
	{
	    /* arch number for open port Linux */
	    env = getenv("arcNumber");
	    if(!env )
	    {
		/* arch number according to board ID */
		int board_id = mvBoardIdGet();	
		switch(board_id){
		    case(DB_88F6281A_BP_ID):
			sprintf(tmp_buf,"%d", DB_88F6281_BP_MLL_ID);
			board_id = DB_88F6281_BP_MLL_ID; 
			break;
		    case(RD_88F6192A_ID):
			sprintf(tmp_buf,"%d", RD_88F6192_MLL_ID);
			board_id = RD_88F6192_MLL_ID; 
			break;
		    case(RD_88F6281A_ID):
			sprintf(tmp_buf,"%d", RD_88F6281_MLL_ID);
			board_id = RD_88F6281_MLL_ID; 
			break;
		    default:
			sprintf(tmp_buf,"%d", board_id);
			board_id = board_id; 
			break;
		}
		gd->bd->bi_arch_number = board_id;
		setenv("arcNumber", tmp_buf);
	    }
	    else
	    {
		gd->bd->bi_arch_number = simple_strtoul(env, NULL, 10);
	    }
	}

        /* update the CASset env parameter */
        env = getenv("CASset");
        if(!env )
        {
#ifdef MV_MIN_CAL
                setenv("CASset","min");
#else
                setenv("CASset","max");
#endif
        }
        /* Monitor extension */
#ifdef MV_INCLUDE_MONT_EXT
        env = getenv("enaMonExt");
        if(/* !env || */ ( (strcmp(env,"yes") == 0) || (strcmp(env,"Yes") == 0) ) )
                setenv("enaMonExt","yes");
        else
#endif
                setenv("enaMonExt","no");

#if defined (MV_INC_BOARD_NOR_FLASH)
        env = getenv("enaFlashBuf");
        if( ( (strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) ) )
                setenv("enaFlashBuf","no");
        else
                setenv("enaFlashBuf","yes");
#endif

	/* CPU streaming */
        env = getenv("enaCpuStream");
        if(!env || ( (strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) ) )
                setenv("enaCpuStream","no");
        else
                setenv("enaCpuStream","yes");

	/* Write allocation */
	env = getenv("enaWrAllo");
	if( !env || ( ((strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) )))
		setenv("enaWrAllo","no");
	else
		setenv("enaWrAllo","yes");

	/* Pex mode */
	env = getenv("pexMode");
	if( env && ( ((strcmp(env,"EP") == 0) || (strcmp(env,"ep") == 0) )))
		setenv("pexMode","EP");
	else
		setenv("pexMode","RC");

    	env = getenv("disL2Cache");
    	if(!env || ( (strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) ) )
        	setenv("disL2Cache","no"); 
    	else
        	setenv("disL2Cache","yes");

    	env = getenv("setL2CacheWT");
    	if(!env || ( (strcmp(env,"yes") == 0) || (strcmp(env,"Yes") == 0) ) )
        	setenv("setL2CacheWT","yes"); 
    	else
        	setenv("setL2CacheWT","no");

    	env = getenv("disL2Prefetch");
    	if(!env || ( (strcmp(env,"yes") == 0) || (strcmp(env,"Yes") == 0) ) )
	{
        	setenv("disL2Prefetch","yes"); 
	
		/* ICache Prefetch */
		env = getenv("enaICPref");
		if( env && ( ((strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) )))
			setenv("enaICPref","no");
		else
			setenv("enaICPref","yes");
		
		/* DCache Prefetch */
		env = getenv("enaDCPref");
		if( env && ( ((strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) )))
			setenv("enaDCPref","no");
		else
			setenv("enaDCPref","yes");
	}
    	else
	{
        	setenv("disL2Prefetch","no");
		setenv("enaICPref","no");
		setenv("enaDCPref","no");
	}


        env = getenv("sata_dma_mode");
        if( env && ((strcmp(env,"No") == 0) || (strcmp(env,"no") == 0) ) )
                setenv("sata_dma_mode","no");
        else
                setenv("sata_dma_mode","yes");


        /* Malloc length */
        env = getenv("MALLOC_len");
        malloc_len =  simple_strtoul(env, NULL, 10) << 20;
        if(malloc_len == 0){
                sprintf(tmp_buf,"%d",CFG_MALLOC_LEN>>20);
                setenv("MALLOC_len",tmp_buf);
	}
         
        /* primary network interface */
        env = getenv("ethprime");
	if(!env)
    {
        if(mvBoardIdGet() == RD_88F6281A_ID)
            setenv("ethprime","egiga1");
        else
            setenv("ethprime",ENV_ETH_PRIME);
    }
 
	/* netbsd boot arguments */
        env = getenv("netbsd_en");
	if( !env || ( ((strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) )))
		setenv("netbsd_en","no");
	else
	{
	    setenv("netbsd_en","yes");
	    env = getenv("netbsd_gw");
	    if(!env)
                setenv("netbsd_gw","192.168.0.254");

	    env = getenv("netbsd_mask");
	    if(!env)
                setenv("netbsd_mask","255.255.255.0");

	    env = getenv("netbsd_fs");
	    if(!env)
                setenv("netbsd_fs","nfs");

	    env = getenv("netbsd_server");
	    if(!env)
                setenv("netbsd_server","192.168.0.1");

	    env = getenv("netbsd_ip");
	    if(!env)
	    {
		env = getenv("ipaddr");
               	setenv("netbsd_ip",env);
	    }

	    env = getenv("netbsd_rootdev");
	    if(!env)
                setenv("netbsd_rootdev","mgi0");

	    env = getenv("netbsd_add");
	    if(!env)
                setenv("netbsd_add","0x800000");

	    env = getenv("netbsd_get");
	    if(!env)
                setenv("netbsd_get","tftpboot $(netbsd_add) $(image_name)");

#if defined(MV_INC_BOARD_QD_SWITCH)
	    env = getenv("netbsd_netconfig");
	    if(!env)
	    setenv("netbsd_netconfig","mv_net_config=<((mgi0,00:00:11:22:33:44,0)(mgi1,00:00:11:22:33:55,1:2:3:4)),mtu=1500>");
#endif
	    env = getenv("netbsd_set_args");
	    if(!env)
	    	setenv("netbsd_set_args","setenv bootargs nfsroot=$(netbsd_server):$(rootpath) fs=$(netbsd_fs) \
ip=$(netbsd_ip) serverip=$(netbsd_server) mask=$(netbsd_mask) gw=$(netbsd_gw) rootdev=$(netbsd_rootdev) \
ethaddr=$(ethaddr) $(netbsd_netconfig)");

	    env = getenv("netbsd_boot");
	    if(!env)
	    	setenv("netbsd_boot","bootm $(netbsd_add) $(bootargs)");

	    env = getenv("netbsd_bootcmd");
	    if(!env)
	    	setenv("netbsd_bootcmd","run netbsd_get ; run netbsd_set_args ; run netbsd_boot");
	}

	/* vxWorks boot arguments */
        env = getenv("vxworks_en");
	if( !env || ( ((strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) )))
		setenv("vxworks_en","no");
	else
	{
	    char* buff = 0x1100;
	    setenv("vxworks_en","yes");
		
	    sprintf(buff,"mgi(0,0) host:vxWorks.st");
	    env = getenv("serverip");
	    strcat(buff, " h=");
	    strcat(buff,env);
	    env = getenv("ipaddr");
	    strcat(buff, " e=");
	    strcat(buff,env);
	    strcat(buff, ":ffff0000 u=anonymous pw=target ");

	    setenv("vxWorks_bootargs",buff);
	}
#ifdef SHASTA_BOARD
	nand = &nand_info[0];
//	printf("nand->oobblock=%x\n", nand->oobblock);
//	printf("nand->erasesize=%x\n", nand->erasesize);
	nand_layout_ptr = nand_layout_info;
        
        printf ("Scanning partition header:\n");
	do {
		
	    /* Scanning the nand flash header for each partitions*/
            if ((env=getenv (nand_layout_ptr->env_name)) == NULL) {
	    	opts.offset = nand_layout_ptr->start_addr;	
	    	sign_found=0;
	    	for ( ; opts.offset < (nand_layout_ptr->start_addr+0x100000); opts.offset+=nand->erasesize) {//Search 1M is enough	
		    memset(buf, 0, nand->oobblock ); 
                    opts.buffer     = (u_char*) buf;
                    opts.length     = nand->oobblock;
                    opts.quiet      = 1;
                    if( nand_read_opts(nand, &opts)) continue;
//		    printf("offset=%x  buf=%c%c%c%c\n", opts.offset, buf[0], buf[1], buf[2], buf[3]);
		    if (! strncmp(nand_layout_ptr->sign, buf, 4)) {
		    	printf("Found sign %s at %x\n", nand_layout_ptr->sign, opts.offset);
			lll_ptr=(void*)&buf[4];
			nand_layout_ptr->len = *lll_ptr;
		    	sprintf(tmp_str,"0x%8.8x", opts.offset);
			sign_found=1;
			lll_ptr=(void*)&buf[8];
			nand_layout_ptr->crc32_data = *lll_ptr;
			lll_ptr=(void*)&buf[12];
			if (*lll_ptr != 0) 
			    nand_layout_ptr->partition_size = *lll_ptr;
		    	break;
		    } //if
	    	} //for
	        if(! sign_found) {
		    printf("Warning: singn %s not found, use default offset %8.8x\n", nand_layout_ptr->sign, nand_layout_ptr->start_addr);
		    sprintf(tmp_str,"0x%8.8x", nand_layout_ptr->start_addr);
	    	} //if
//	    	setenv(nand_layout_ptr->env_name, tmp_str);	
		nand_layout_ptr->actually_offset = simple_strtoul(tmp_str, NULL, 10);
	    } else { //if
		env = getenv(nand_layout_ptr->env_name);
		nand_layout_ptr->actually_offset = simple_strtoul(env, NULL, 10);
	    } //if	
	    nand_layout_ptr++;
	} while ( strncmp( nand_layout_ptr->sign, "NULL", 4)); 

        /* linux boot arguments */
        env = getenv("bootargs_root");
        if(!env)
                setenv("bootargs_root","root=/dev/ram0 rootfstype=cramfs init=/etc/rc.preroot initrd=0x800000,0x4800000");
#else		
        /* linux boot arguments */
        env = getenv("bootargs_root");
        if(!env)
                setenv("bootargs_root","root=/dev/nfs rw");
#endif 
	/* For open Linux we set boot args differently */
	env = getenv("mainlineLinux");
	if(env && ((strcmp(env,"yes") == 0) ||  (strcmp(env,"Yes") == 0)))
	{
        	env = getenv("bootargs_end");
        	if(!env)
                setenv("bootargs_end",":::orion:eth0:none");
	}
	else
	{
        	env = getenv("bootargs_end");
        	if(!env)
#if defined(MV_INC_BOARD_QD_SWITCH)
                setenv("bootargs_end",CFG_BOOTARGS_END_SWITCH);
#else
                setenv("bootargs_end",CFG_BOOTARGS_END);
#endif
	}
	
        env = getenv("image_name");
        if(!env)
                setenv("image_name","uImage");
 

#if (CONFIG_BOOTDELAY >= 0)
        env = getenv("bootcmd");
//        if(!env)
#if defined(MV_INCLUDE_TDM) && defined(MV_INC_BOARD_QD_SWITCH)
	    setenv("bootcmd","tftpboot 0x2000000 $(image_name);\
setenv bootargs $(console) $(bootargs_root) nfsroot=$(serverip):$(rootpath) \
ip=$(ipaddr):$(serverip)$(bootargs_end) $(mvNetConfig) $(mvPhoneConfig);  bootm 0x2000000; ");
#elif defined(MV_INC_BOARD_QD_SWITCH)
            setenv("bootcmd","tftpboot 0x2000000 $(image_name);\
setenv bootargs $(console) $(bootargs_root) nfsroot=$(serverip):$(rootpath) \
ip=$(ipaddr):$(serverip)$(bootargs_end) $(mvNetConfig);  bootm 0x2000000; ");
#elif defined(MV_INCLUDE_TDM)
            setenv("bootcmd","tftpboot 0x2000000 $(image_name);\
setenv bootargs $(console) $(bootargs_root) nfsroot=$(serverip):$(rootpath) \
ip=$(ipaddr):$(serverip)$(bootargs_end) $(mvNetConfig) $(mvPhoneConfig);  bootm 0x2000000; ");
#else

#if defined(SHASTA_BOARD)
{	
	char tmp_s[256];
	unsigned long tmp_offset;
	int nnn; 
	nand_layout_t *tmp_nand_layout_ptr;

	sprintf(buff,"console=ttyS0,115200 mtdparts=nand_mtd:0xa0000@0x0(uboot),");	//this is fix address
	sprintf(tmp_s, "0x%8.8x@0x%8.8x(param),", CFG_ENV_SECT_SIZE*4, CFG_ENV_OFFSET); // environment starts here  //
	strcat(buff, tmp_s);								//u-boot parameter

	nand_layout_ptr = nand_layout_info;
	nand_layout_ptr_next=nand_layout_ptr+1;

	tmp_nand_layout_ptr = nand_layout_info;
	tmp_offset = 0xc0000;

	if ( tmp_offset < nand_layout_ptr->actually_offset) {
		tmp_offset=nand_layout_ptr->actually_offset;
	}	
	sprintf(tmp_s, "0x%8.8x@0x%8.8x(preroot),", \
//		nand_layout_ptr_next->actually_offset - nand_layout_ptr->actually_offset, \
		nand_layout_ptr->actually_offset); // preroot  //
		nand_layout_ptr->partition_size, \
		tmp_offset); // preroot  //
	strcat(buff, tmp_s);								//preroot
	nand_layout_ptr = nand_layout_ptr_next;
	nand_layout_ptr_next=nand_layout_ptr+1;

	tmp_offset += tmp_nand_layout_ptr->partition_size;
	tmp_nand_layout_ptr++;

	if ( tmp_offset < nand_layout_ptr->actually_offset) {
		tmp_offset=nand_layout_ptr->actually_offset;
	}	
	sprintf(tmp_s, "0x%8.8x@0x%8.8x(uimage),", \
//		nand_layout_ptr_next->actually_offset - nand_layout_ptr->actually_offset, \
		nand_layout_ptr->actually_offset); // uimage  //
		nand_layout_ptr->partition_size, \
		tmp_offset); // uimage  //
	strcat(buff, tmp_s);								//uimage
	nand_layout_ptr = nand_layout_ptr_next;
//	nand_layout_ptr_next=nand_layout_ptr+1;

	tmp_offset += tmp_nand_layout_ptr->partition_size;
	tmp_nand_layout_ptr++;

	if ( tmp_offset < nand_layout_ptr->actually_offset) {
		tmp_offset=nand_layout_ptr->actually_offset;
	}	
	sprintf(tmp_s, "0x%8.8x@0x%8.8x(rootfs),", \
//		nand_layout_ptr->partition_size, \
		nand_layout_ptr->actually_offset); // rootfs  //
		nand_layout_ptr->partition_size, \
		tmp_offset); // rootfs  //
	strcat(buff, tmp_s);								//rootfs
//	nand_layout_ptr = nand_layout_ptr_next;
	tmp_offset += tmp_nand_layout_ptr->partition_size;

	sprintf(tmp_s, "0x%8.8x@0x%8.8x(misc),32m@0x0(flash) ", \
//		nand->size - (nand_layout_ptr->actually_offset + nand_layout_ptr->partition_size), \
		nand_layout_ptr->actually_offset + nand_layout_ptr->partition_size); // misc  //
		nand->size - tmp_offset, tmp_offset); // misc  //
	strcat(buff, tmp_s);								//misc

//	printf("buff=%s\n", buff);
//	env = getenv("console");
//	if(!env)
		setenv("console",buff);

	//setting bootcmd
	nand_layout_ptr = nand_layout_info;
	//point to preroot
	sprintf(buff, "nand reset; nand read.e 0x800000 0x%8.8x 0x%8.8x; ", \
	    nand_layout_ptr->actually_offset + nand->erasesize, (nand_layout_ptr->len + nand->oobblock)& 0xfffffe00);
	strcat(buff, "check_crc32 PrEr; ");
	setenv("bootcmd_f", buff); // for fail-safe
	nand_layout_ptr++;	//point to kernel info
	sprintf(tmp_s, "nand reset; nand read.e 0x40000 0x%8.8x 0x%8.8x; ", \
	    nand_layout_ptr->actually_offset + nand->erasesize, ((nand_layout_ptr->len + nand->oobblock)& 0xfffffe00));
	strcat(tmp_s, "check_crc32 KrNl; ");
	strcat(buff, tmp_s);
	strcat(buff, "setenv bootargs $(console) $(bootargs_root);  bootm 0x40000; ");
	setenv("bootcmd", buff);

}
#else
            setenv("bootcmd","tftpboot 0x2000000 $(image_name);\
setenv bootargs $(console) $(bootargs_root) nfsroot=$(serverip):$(rootpath) \
ip=$(ipaddr):$(serverip)$(bootargs_end);  bootm 0x2000000; ");
#endif
#endif
#endif /* (CONFIG_BOOTDELAY >= 0) */

        env = getenv("standalone");
        if(!env)
#if defined(MV_INCLUDE_TDM) && defined(MV_INC_BOARD_QD_SWITCH)
	    setenv("standalone","fsload 0x2000000 $(image_name);setenv bootargs $(console) root=/dev/mtdblock0 rw \
ip=$(ipaddr):$(serverip)$(bootargs_end) $(mvNetConfig) $(mvPhoneConfig); bootm 0x2000000;");
#elif defined(MV_INC_BOARD_QD_SWITCH)
            setenv("standalone","fsload 0x2000000 $(image_name);setenv bootargs $(console) root=/dev/mtdblock0 rw \
ip=$(ipaddr):$(serverip)$(bootargs_end) $(mvNetConfig); bootm 0x2000000;");
#elif defined(MV_INCLUDE_TDM)
            setenv("standalone","fsload 0x2000000 $(image_name);setenv bootargs $(console) root=/dev/mtdblock0 rw \
ip=$(ipaddr):$(serverip)$(bootargs_end) $(mvPhoneConfig); bootm 0x2000000;");
#else
            setenv("standalone","fsload 0x2000000 $(image_name);setenv bootargs $(console) root=/dev/mtdblock0 rw \
ip=$(ipaddr):$(serverip)$(bootargs_end); bootm 0x2000000;");
#endif
                 
       /* Set boodelay to 3 sec, if Monitor extension are disabled */
        if(!enaMonExt()){
                setenv("bootdelay","3");
		setenv("disaMvPnp","no");
	}

	/* Disable PNP config of Marvel memory controller devices. */
        env = getenv("disaMvPnp");
        if(!env)
                setenv("disaMvPnp","no");

#if (defined(MV_INCLUDE_GIG_ETH) || defined(MV_INCLUDE_UNM_ETH))
	/* Generate random ip and mac address */
	/* Read DRAM FTDLL register to create random data for enc */
	unsigned int xi, xj, xk, xl, i;
	char ethaddr_0[30];
	char ethaddr_1[30];

	MV_U32 random[16];
	unsigned char digest[16];

	MV_REG_BIT_SET(0x1478, BIT7);
	for(i=0; i < 16;i++)
		random[i] = MV_REG_READ(0x1470);
	
	/* Run MD5 over the ftdll buffer */
	mvMD5((unsigned char*)random, 64, digest);

	xi = (digest[0]%254);
	/* No valid ip with one of the fileds has the value 0 */
	if (xi == 0)
		xi+=2;

	xj = (digest[1]%254);
	/* No valid ip with one of the fileds has the value 0 */
	if (xj == 0)
		xj+=2;

	/* Check if the ip address is the same as the server ip */
	if ((xj == 1) && (xi == 11))
		xi+=2;

	xk = digest[2];
	xl = digest[3];

	sprintf(ethaddr_0,"00:50:43:%02x:%02x:%02x",xk,xi,xj);
	sprintf(ethaddr_1,"00:50:43:%02x:%02x:%02x",xl,xi,xj);

	/* MAC addresses */
        env = getenv("ethaddr");
        if(!env)
                setenv("ethaddr",ethaddr_0);
        
#if !defined(MV_INC_BOARD_QD_SWITCH)
/* ETH1ADDR not define in GWAP boards */
	if ((mvBoardMppGroupTypeGet(MV_BOARD_MPP_GROUP_1) == MV_BOARD_RGMII) || 
			(mvBoardMppGroupTypeGet(MV_BOARD_MPP_GROUP_2) == MV_BOARD_RGMII))
	{
#ifdef SHASTA_BOARD
		//always fill with ethaddr+1
		unsigned long eth_no;
		int i;
		MV_U8 in_loop_macHex[6]; 
		unsigned char *envptr;
		
        	env = getenv("ethaddr");
		envptr=env;
		if (*(envptr+2)!=':') {
		    	
		    sprintf(ethaddr_0,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",
		     *envptr, *(envptr+1), *(envptr+2),*(envptr+3), 
		     *(envptr+4), *(envptr+5), *(envptr+6),*(envptr+7), 
		     *(envptr+8), *(envptr+9), *(envptr+10),*(envptr+11));
                    setenv("ethaddr",ethaddr_0);
		}

        	env = getenv("ethaddr");
		mvMacStrToHex(env, in_loop_macHex);
//		for(i=0;i<6;i++) printf("%2.2x", in_loop_macHex[i]);
		eth_no=0;
		for(i=0;i<3;i++) ethaddr_1[i]=in_loop_macHex[i];
		for(i=3;i<6;i++) {
		    eth_no |= in_loop_macHex[i] << ( (5-i)*8);
		}
		eth_no++;
		for(i=3;i<6;i++) { 
		    in_loop_macHex[i] = ( eth_no >> ((5-i)*8)) & 0xff;
		}
//		printf("\neth1addr=");
//		for(i=0;i<6;i++) { 
//		    printf("%2.2x", in_loop_macHex[i]);
//		}
		sprintf(ethaddr_1,"%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",
		 in_loop_macHex[0], in_loop_macHex[1], in_loop_macHex[2],
		 in_loop_macHex[3], in_loop_macHex[4], in_loop_macHex[5]);
		
             	setenv("eth1addr",ethaddr_1);
#else
        	env = getenv("eth1addr");
        	if(!env)
               		setenv("eth1addr",ethaddr_1);
#endif
	}
#endif
#if defined(MV_INCLUDE_TDM)
        /* Set mvPhoneConfig env parameter */
        env = getenv("mvPhoneConfig");
        if(!env )
            setenv("mvPhoneConfig","mv_phone_config=dev0:fxs,dev1:fxo");
#endif
        /* Set mvNetConfig env parameter */
        env = getenv("mvNetConfig");
        if(!env )
		    setenv("mvNetConfig","mv_net_config=(00:11:88:0f:62:81,0:1:2:3),mtu=1500");
#endif /*  (MV_INCLUDE_GIG_ETH) || defined(MV_INCLUDE_UNM_ETH) */

#if defined(MV_INCLUDE_USB)
	/* USB Host */
	env = getenv("usb0Mode");
	if(!env)
		setenv("usb0Mode",ENV_USB0_MODE);
#endif  /* (MV_INCLUDE_USB) */

#if defined(YUK_ETHADDR)
	env = getenv("yuk_ethaddr");
	if(!env)
		setenv("yuk_ethaddr",YUK_ETHADDR);

	{
		int i;
		char *tmp = getenv ("yuk_ethaddr");
		char *end;

		for (i=0; i<6; i++) {
			yuk_enetaddr[i] = tmp ? simple_strtoul(tmp, &end, 16) : 0;
			if (tmp)
				tmp = (*end) ? end+1 : end;
		}
	}
#endif /* defined(YUK_ETHADDR) */

#if defined(RD_88F6281A) || defined(RD_88F6192A) || defined(RD_88F6190A)
	mvHddPowerCtrl();
#endif
#if (CONFIG_COMMANDS & CFG_CMD_RCVR)
	env = getenv("netretry");
	if (!env)
		setenv("netretry","no");

	env = getenv("rcvrip");
	if (!env)
		setenv("rcvrip",RCVR_IP_ADDR);

	env = getenv("loadaddr");
	if (!env)
		setenv("loadaddr",RCVR_LOAD_ADDR);

	env = getenv("autoload");
	if (!env)
		setenv("autoload","no");

	/* Check the recovery trigger */
	recoveryDetection();
#endif
#if defined(SHASTA_BOARD)
	env = getenv("uboot_ver");
	if (!env) {
	    printf("Env: Save default environment.\n");
	    setenv("uboot_ver", UBOOT_FW_VER);
	    saveenv();
	} else {
		char *tmp = getenv ("uboot_ver");
		if (strcmp(tmp, UBOOT_FW_VER)) {
	            printf("Env: U-boot version changed, Save default environment.n");
	    	    setenv("uboot_ver", UBOOT_FW_VER);
	            saveenv();
		}
	}
#endif	
        return;
}

#ifdef BOARD_LATE_INIT
int board_late_init (void)
{
	/* Check if to use the LED's for debug or to use single led for init and Linux heartbeat */
	mvBoardDebugLed(0);
	return 0;
}
#endif

#if defined(SHASTA_BOARD)
void wix_fw_update(void){
        char *env,*bootcmd_f;
	char buff[256];

        env = getenv("fw_update");
        if(/* !env || */ (strcmp(env,"1") == 0 )  ){
        	bootcmd_f = getenv("bootcmd_f");
		sprintf(buff,"%s%s%s",bootcmd_f,"ide reset; ext2load boot;","setenv bootargs $(console) $(bootargs_root); bootm 0x40000");
		setenv("bootcmd",buff);
	}

	return;

}
#endif

int misc_init_r (void)
{
	char name[128];
	
	mvBoardDebugLed(5);

	mvCpuNameGet(name);
	printf("\nCPU : %s\n",  name);

	/* init special env variables */
	misc_init_r_env();

	mv_cpu_init();

#if defined(MV_INCLUDE_MONT_EXT)
	if(enaMonExt()){
			printf("\n      Marvell monitor extension:\n");
			mon_extension_after_relloc();
	}
    	printf("\n");		
#endif /* MV_INCLUDE_MONT_EXT */

	/* print detected modules */
	mvMppModuleTypePrint();

    	printf("\n");		
	/* init the units decode windows */
	misc_init_r_dec_win();

#ifdef CONFIG_PCI
#if !defined(MV_MEM_OVER_PCI_WA) && !defined(MV_MEM_OVER_PEX_WA)
       pci_init();
#endif
#endif

	mvBoardDebugLed(6);

	mvBoardDebugLed(7);

#if defined(SHASTA_BOARD)
	wix_fw_update();	
#endif
	
	return 0;
}

MV_U32 mvTclkGet(void)
{
        DECLARE_GLOBAL_DATA_PTR;
        /* get it only on first time */
        if(gd->tclk == 0)
                gd->tclk = mvBoardTclkGet();

        return gd->tclk;
}

MV_U32 mvSysClkGet(void)
{
        DECLARE_GLOBAL_DATA_PTR;
        /* get it only on first time */
        if(gd->bus_clk == 0)
                gd->bus_clk = mvBoardSysClkGet();

        return gd->bus_clk;
}
 
#ifndef MV_TINY_IMAGE
/* exported for EEMBC */
MV_U32 mvGetRtcSec(void)
{
        MV_RTC_TIME time;
#ifdef MV_INCLUDE_RTC
        mvRtcTimeGet(&time);
#elif CONFIG_RTC_DS1338_DS1339
        mvRtcDS133xTimeGet(&time);
#endif
	return (time.minutes * 60) + time.seconds;
}
#endif

void reset_cpu(void)
{
	mvBoardReset();
}

void mv_cpu_init(void)
{
	char *env;
	volatile unsigned int temp;

	/*CPU streaming & write allocate */
	env = getenv("enaWrAllo");
	if(env && ((strcmp(env,"yes") == 0) ||  (strcmp(env,"Yes") == 0)))  
	{
		__asm__ __volatile__("mrc    p15, 1, %0, c15, c1, 0" : "=r" (temp));
		temp |= BIT28;
		__asm__ __volatile__("mcr    p15, 1, %0, c15, c1, 0" :: "r" (temp));
		
	}
	else
	{
		__asm__ __volatile__("mrc    p15, 1, %0, c15, c1, 0" : "=r" (temp));
		temp &= ~BIT28;
		__asm__ __volatile__("mcr    p15, 1, %0, c15, c1, 0" :: "r" (temp));
	}

	env = getenv("enaCpuStream");
	if(!env || (strcmp(env,"no") == 0) ||  (strcmp(env,"No") == 0) )
	{
		__asm__ __volatile__("mrc    p15, 1, %0, c15, c1, 0" : "=r" (temp));
		temp &= ~BIT29;
		__asm__ __volatile__("mcr    p15, 1, %0, c15, c1, 0" :: "r" (temp));
	}
	else
	{
		__asm__ __volatile__("mrc    p15, 1, %0, c15, c1, 0" : "=r" (temp));
		temp |= BIT29;
		__asm__ __volatile__("mcr    p15, 1, %0, c15, c1, 0" :: "r" (temp));
	}
		
	/* Verifay write allocate and streaming */
	printf("\n");
	__asm__ __volatile__("mrc    p15, 1, %0, c15, c1, 0" : "=r" (temp));
	if (temp & BIT29)
		printf("Streaming enabled \n");
	else
		printf("Streaming disabled \n");
	if (temp & BIT28)
		printf("Write allocate enabled\n");
	else
		printf("Write allocate disabled\n");

	/* DCache Pref  */
	env = getenv("enaDCPref");
	if(env && ((strcmp(env,"yes") == 0) ||  (strcmp(env,"Yes") == 0)))
	{
		MV_REG_BIT_SET( CPU_CONFIG_REG , CCR_DCACH_PREF_BUF_ENABLE);
	}

	if(env && ((strcmp(env,"no") == 0) ||  (strcmp(env,"No") == 0)))
        {
		MV_REG_BIT_RESET( CPU_CONFIG_REG , CCR_DCACH_PREF_BUF_ENABLE);
	}

	/* ICache Pref  */
	env = getenv("enaICPref");
        if(env && ((strcmp(env,"yes") == 0) ||  (strcmp(env,"Yes") == 0)))
	{
		MV_REG_BIT_SET( CPU_CONFIG_REG , CCR_ICACH_PREF_BUF_ENABLE);
	}
	
	if(env && ((strcmp(env,"no") == 0) ||  (strcmp(env,"No") == 0)))
        {
		MV_REG_BIT_RESET( CPU_CONFIG_REG , CCR_ICACH_PREF_BUF_ENABLE);
	}

	/* Set L2C WT mode - Set bit 4 */
	temp = MV_REG_READ(CPU_L2_CONFIG_REG);
    	env = getenv("setL2CacheWT");
    	if(!env || ( (strcmp(env,"yes") == 0) || (strcmp(env,"Yes") == 0) ) )
	{
		temp |= BIT4;
	}
	else
		temp &= ~BIT4;
	MV_REG_WRITE(CPU_L2_CONFIG_REG, temp);


	/* L2Cache settings */
	asm ("mrc p15, 1, %0, c15, c1, 0":"=r" (temp));

	/* Disable L2C pre fetch - Set bit 24 */
    	env = getenv("disL2Prefetch");
    	if(env && ( (strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) ) )
		temp &= ~BIT24;
	else
		temp |= BIT24;

	/* enable L2C - Set bit 22 */
    	env = getenv("disL2Cache");
    	if(!env || ( (strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) ) )
		temp |= BIT22;
	else
		temp &= ~BIT22;
	
	asm ("mcr p15, 1, %0, c15, c1, 0": :"r" (temp));


	/* Enable i cache */
	asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (temp));
	temp |= BIT12;
	asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (temp));

	/* Change reset vector to address 0x0 */
	asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (temp));
	temp &= ~BIT13;
	asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (temp));
}
/*******************************************************************************
* mvBoardMppModuleTypePrint - print module detect
*
* DESCRIPTION:
*
* INPUT:
*
* OUTPUT:
*       None.
*
* RETURN:
*
*******************************************************************************/
MV_VOID mvMppModuleTypePrint(MV_VOID)
{

	MV_BOARD_MPP_GROUP_CLASS devClass;
	MV_BOARD_MPP_TYPE_CLASS mppGroupType;
	MV_U32 devId;
	MV_U32 maxMppGrp = 1;
	
	devId = mvCtrlModelGet();

	switch(devId){
		case MV_6281_DEV_ID:
			maxMppGrp = MV_6281_MPP_MAX_MODULE;
			break;
		case MV_6192_DEV_ID:
			maxMppGrp = MV_6192_MPP_MAX_MODULE;
			break;
        case MV_6190_DEV_ID:
            maxMppGrp = MV_6190_MPP_MAX_MODULE;
            break;
		case MV_6180_DEV_ID:
			maxMppGrp = MV_6180_MPP_MAX_MODULE;
			break;		
	}

	for (devClass = 0; devClass < maxMppGrp; devClass++)
	{
		mppGroupType = mvBoardMppGroupTypeGet(devClass);

		switch(mppGroupType)
		{
			case MV_BOARD_TDM:
                if(devId != MV_6190_DEV_ID)
                    printf("Module %d is TDM\n", devClass);
				break;
			case MV_BOARD_AUDIO:
                if(devId != MV_6190_DEV_ID)
                    printf("Module %d is AUDIO\n", devClass);
				break;
			case MV_BOARD_RGMII:
                if(devId != MV_6190_DEV_ID)
                    printf("Module %d is RGMII\n", devClass);
				break;
			case MV_BOARD_GMII:
                if(devId != MV_6190_DEV_ID)
                    printf("Module %d is GMII\n", devClass);
				break;
			case MV_BOARD_TS:
                if(devId != MV_6190_DEV_ID)
                    printf("Module %d is TS\n", devClass);
				break;
			default:
				break;
		}
	}
}

/* Set unit in power off mode acording to the detection of MPP */
#if defined(MV_INCLUDE_CLK_PWR_CNTRL)
int mv_set_power_scheme(void)
{
	int mppGroupType1 = mvBoardMppGroupTypeGet(MV_BOARD_MPP_GROUP_1);
	int mppGroupType2 = mvBoardMppGroupTypeGet(MV_BOARD_MPP_GROUP_2);
	MV_U32 devId = mvCtrlModelGet();
	MV_U32 boardId = mvBoardIdGet();

	if (devId == MV_6180_DEV_ID)
	{
		/* Sata power down */
		mvCtrlPwrMemSet(SATA_UNIT_ID, 1, MV_FALSE);
		mvCtrlPwrMemSet(SATA_UNIT_ID, 0, MV_FALSE);
		mvCtrlPwrClckSet(SATA_UNIT_ID, 1, MV_FALSE);
		mvCtrlPwrClckSet(SATA_UNIT_ID, 0, MV_FALSE);
		/* Sdio power down */
		mvCtrlPwrMemSet(SDIO_UNIT_ID, 0, MV_FALSE);
		mvCtrlPwrClckSet(SDIO_UNIT_ID, 0, MV_FALSE);
	}

    if (boardId == RD_88F6281A_ID)
    {
		DB(printf("Warning: TS is Powered Off\n"));
		mvCtrlPwrClckSet(TS_UNIT_ID, 0, MV_FALSE);
        return MV_OK;
    }
	
	/* Close egiga 1 */
	if ((mppGroupType1 != MV_BOARD_GMII) && (mppGroupType1 != MV_BOARD_RGMII) && (mppGroupType2 != MV_BOARD_RGMII))
	{
		DB(printf("Warning: Giga1 is Powered Off\n"));
		mvCtrlPwrMemSet(ETH_GIG_UNIT_ID, 1, MV_FALSE);
		mvCtrlPwrClckSet(ETH_GIG_UNIT_ID, 1, MV_FALSE);
	}

	/* Close TDM */
	if ((mppGroupType1 != MV_BOARD_TDM) && (mppGroupType2 != MV_BOARD_TDM))
	{
		DB(printf("Warning: TDM is Powered Off\n"));
		mvCtrlPwrClckSet(TDM_UNIT_ID, 0, MV_FALSE);
	}

	/* Close AUDIO */
	if ((mppGroupType1 != MV_BOARD_AUDIO) && (mppGroupType2 != MV_BOARD_AUDIO))
	{
		DB(printf("Warning: AUDIO is Powered Off\n"));
		mvCtrlPwrMemSet(AUDIO_UNIT_ID, 0, MV_FALSE);
		mvCtrlPwrClckSet(AUDIO_UNIT_ID, 0, MV_FALSE);
	}

	/* Close TS */
	if ((mppGroupType1 != MV_BOARD_TS) && (mppGroupType2 != MV_BOARD_TS))
	{
		DB(printf("Warning: TS is Powered Off\n"));
		mvCtrlPwrClckSet(TS_UNIT_ID, 0, MV_FALSE);
	}

	return MV_OK;
}

#endif /* defined(MV_INCLUDE_CLK_PWR_CNTRL) */

/*******************************************************************************
* mvUpdateNorFlashBaseAddrBank - 
*
* DESCRIPTION:
*       This function update the CFI driver base address bank with on board NOR
*       devices base address.
*
* INPUT:
*
* OUTPUT:
*
* RETURN:
*       None
*
*******************************************************************************/
#ifdef	CFG_FLASH_CFI_DRIVER
MV_VOID mvUpdateNorFlashBaseAddrBank(MV_VOID)
{
    
    MV_U32 devBaseAddr;
    MV_U32 devNum = 0;
    int i;

    /* Update NOR flash base address bank for CFI flash init driver */
    for (i = 0 ; i < CFG_MAX_FLASH_BANKS_DETECT; i++)
    {
	devBaseAddr = mvBoardGetDeviceBaseAddr(i,BOARD_DEV_NOR_FLASH);
	if (devBaseAddr != 0xFFFFFFFF)
	{
	    flash_add_base_addr (devNum, devBaseAddr);
	    devNum++;
	}
    }
    mv_board_num_flash_banks = devNum;

    /* Update SPI flash count for CFI flash init driver */
    /* Assumption only 1 SPI flash on board */
    for (i = 0 ; i < CFG_MAX_FLASH_BANKS_DETECT; i++)
    {
    	devBaseAddr = mvBoardGetDeviceBaseAddr(i,BOARD_DEV_SPI_FLASH);
    	if (devBaseAddr != 0xFFFFFFFF)
		mv_board_num_flash_banks += 1;
    }
}
#endif	/* CFG_FLASH_CFI_DRIVER */


/*******************************************************************************
* mvHddPowerCtrl - 
*
* DESCRIPTION:
*       This function set HDD power on/off acording to env or wait for button push
* INPUT:
*	None
* OUTPUT:
*	None
* RETURN:
*       None
*
*******************************************************************************/
#if defined(RD_88F6281A) || defined(RD_88F6192A) || defined(RD_88F6190A)
static void mvHddPowerCtrl(void)
{

    MV_32 hddPowerBit;
    MV_32 fanPowerBit;
	MV_32 hddHigh = 0;
	MV_32 fanHigh = 0;
	char* env;
	
    if(RD_88F6281A_ID == mvBoardIdGet())
    {
        hddPowerBit = mvBoarGpioPinNumGet(BOARD_GPP_HDD_POWER, 0);
        fanPowerBit = mvBoarGpioPinNumGet(BOARD_GPP_FAN_POWER, 0);
        if (hddPowerBit > 31)
    	{
    		hddPowerBit = hddPowerBit % 32;
    		hddHigh = 1;
    	}
    
    	if (fanPowerBit > 31)
    	{
    		fanPowerBit = fanPowerBit % 32;
    		fanHigh = 1;
    	}
    }

	if ((RD_88F6281A_ID == mvBoardIdGet()) || (RD_88F6192A_ID == mvBoardIdGet()) || 
        (RD_88F6190A_ID == mvBoardIdGet()))
	{
		env = getenv("hddPowerCtrl");
 		if(!env || ( (strcmp(env,"no") == 0) || (strcmp(env,"No") == 0) ) )
                	setenv("hddPowerCtrl","no");
        	else
                	setenv("hddPowerCtrl","yes");

        if(RD_88F6281A_ID == mvBoardIdGet())
        {
            mvBoardFanPowerControl(MV_TRUE);
            mvBoardHDDPowerControl(MV_TRUE);
        }
        else
        {
            /* FAN power on */
    		MV_REG_BIT_SET(GPP_DATA_OUT_REG(fanHigh),(1<<fanPowerBit));
    		MV_REG_BIT_RESET(GPP_DATA_OUT_EN_REG(fanHigh),(1<<fanPowerBit));
            /* HDD power on */
            MV_REG_BIT_SET(GPP_DATA_OUT_REG(hddHigh),(1<<hddPowerBit));
            MV_REG_BIT_RESET(GPP_DATA_OUT_EN_REG(hddHigh),(1<<hddPowerBit));
        }
        
	}
}
#endif


#if (CONFIG_COMMANDS & CFG_CMD_RCVR)
static void recoveryDetection(void)
{
    	MV_32 stateButtonBit = mvBoarGpioPinNumGet(BOARD_GPP_WPS_BUTTON,0);
	MV_32 buttonHigh = 0;
	char* env;

	/* Check if auto recovery is en */	
	env = getenv("enaAutoRecovery");
 	if(!env || ( (strcmp(env,"yes") == 0) || (strcmp(env,"Yes") == 0) ) )
               	setenv("enaAutoRecovery","yes");
        else
	{
               	setenv("enaAutoRecovery","no");
		rcvrflag = 0;
		return;
	}

	if (stateButtonBit == MV_ERROR)
	{	
		rcvrflag = 0;
		return;
	}

	if (stateButtonBit > 31)
	{
		stateButtonBit = stateButtonBit % 32;
		buttonHigh = 1;
	}

	/* Set state input indication pin as input */
	MV_REG_BIT_SET(GPP_DATA_OUT_EN_REG(buttonHigh),(1<<stateButtonBit));

	/* check if recovery triggered - button is pressed */
	if (!(MV_REG_READ(GPP_DATA_IN_REG(buttonHigh)) & (1 << stateButtonBit)))
	{	
		rcvrflag = 1;
	}
}

extern int do_bootm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

void recoveryHandle(void)
{
	char cmd[256];
	char img[10];
	char * argv[3];
	char *env;
	MV_32 imagAddr = 0x400000;	
	MV_32 imagSize = 0;	
	char ip[16]= {"dhcp"};


	/* Show Recovery start indication - both BLUE and RED blinking */

	/* Perform the DHCP */
	printf("Aquiring an IP address using DHCP...\n");
	if (NetLoop(DHCP) == -1)
	{
		mvOsDelay(1000);
		if (NetLoop(DHCP) == -1)
		{
			mvOsDelay(1000);
			if (NetLoop(DHCP) == -1)
			{
				ulong tmpip;
				printf("Failed to retreive an IP address assuming default (%s)!\n", getenv("rcvrip"));
				tmpip = getenv_IPaddr ("rcvrip");
				NetCopyIP(&NetOurIP, &tmpip);			
				sprintf(ip, "static");
			}
		}
	}

	/* Perform the recovery */
	printf("Starting the Recovery process to retreive the file...\n");
	if ((imagSize = NetLoop(RCVR)) == -1)
	{
		printf("Failed\n");
		return;
	}

	/* Boot the downloaded image */	
	env = getenv("loadaddr");
	if (!env)
		printf("Missing loadaddr environment variable assuming default (0x400000)!\n");
	else
		imagAddr = simple_strtoul(env, NULL, 16); /* get the loadaddr env var */

    /* This assignment to cmd should execute prior to the RD setenv and saveenv below*/
    printf("Update bootcmd\n");
    sprintf(cmd,"setenv bootargs $(console) root=/dev/ram0 rootfstype=squashfs initrd=0x%x,0x%x ramdisk_size=%d recovery=%s ; bootm 0x%x;", imagAddr + 0x200000, (imagSize - 0x300000), (imagSize - 0x300000)/1024, ip, imagAddr);

    if(RD_88F6281A_ID == mvBoardIdGet())
    {
        setenv("bootcmd","setenv bootargs $(console) rootfstype=squashfs root=/dev/mtdblock2 $(mvNetConfig) $(mvPhoneConfig); nand read $(loadaddr) 0x100000 0x200000; bootm $(loadaddr);");
    	setenv("console","console=ttyS0,115200");
        saveenv();
    }
    else if(RD_88F6192A_ID == mvBoardIdGet())
    {
        setenv("console","console=ttyS0,115200 mtdparts=spi_flash:0x100000@0x0(uboot)ro,0x200000@0x100000(uimage),0xb80000@0x300000(rootfs),0x180000@0xe80000(varfs),0xf00000@0x100000(flash) varfs=/dev/mtdblock3");
    	setenv("bootcmd","setenv bootargs $(console) rootfstype=squashfs root=/dev/mtdblock2; bootm 0xf8100000;");
        saveenv();
    }

	printf("\nbootcmd: %s\n", cmd);
	setenv("bootcmd", cmd);

	printf("Booting the image (@ 0x%x)...\n", imagAddr);

	sprintf(cmd, "boot");
	sprintf(img, "0x%x", imagAddr);
	argv[0] = cmd;
	argv[1] = img;

	do_bootd(NULL, 0, 2, argv);
}

void recoveryCheck(void)
{
	/* Start the recovery process if indicated by user */
	if (rcvrflag)
		recoveryHandle();
}
#endif

