#include <linux/types.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/ctype.h>

#include <linux/platform_device.h>
#include <linux/cdev.h>
#include "gpp/mvGpp.h"

#define SUPPORT_LCM
#ifdef SUPPORT_LCM
#include "lcm_ioctl.h"
#endif

#include <asm/uaccess.h>
#include <asm/io.h>

#define MPP_GROUP(mpp)  mpp/8
#define MPP_ID(mpp)  	mpp%8
#define MPP_BITS(mpp)	0xf << (MPP_ID(mpp)*4)

#define GPP_GROUP(gpp)  gpp/32
#define GPP_ID(gpp)     gpp%32
#define GPP_BIT(gpp)    0x1 << GPP_ID(gpp)

//#define WIX_DEBUG
//add_shift #define	MAX_PIO	50
#define	MAX_PIO	51

static struct piostruct {
	int proc_num;
	char proc_name[16];
} pio[MAX_PIO];

static struct proc_dir_entry *proc_pio;


//for /proc_sata_pmode +
static struct proc_dir_entry *proc_pio2; 
extern int sata_pmode; /* 1=activity, 0=standby */
unsigned long pmode_time_stamp;
//for /proc_sata_pmode -
//add for 6281 HDD present +
//static struct proc_dir_entry *proc_pio3; 
//add for 6281 HDD present -

#ifdef SUPPORT_LCM

#define WIX_LCM_VERSION "0.1"
#define DEVICE_NAME "wix_lcm"

int wix_lcm_open(struct inode *inode, struct file *filp);
int wix_lcm_release(struct inode *inode, struct file *filp);
int wix_lcm_ioctl(struct inode *inode, struct file *filp,
                     unsigned int cmd, unsigned long arg);

/*
 * The internal representation of our device.
 */
typedef struct
{
    	spinlock_t      lock;
    	dev_t           devnum;
    	int		data_bus_mode;
} lcm_device_t;

static struct cdev          lcm_cdev;
static lcm_device_t STATIC_DEVICE;

// Linux file operations
static struct file_operations file_ops =
{
        .owner  =       THIS_MODULE,
        .open   =       wix_lcm_open,
        .release =      wix_lcm_release,
        .ioctl  =       wix_lcm_ioctl,
};

//add_shift +
#define SHIFT_DATA	12	//MPP12 use data in
#define SHIFT_CLK	13	//MPP13 use data in

//timer=500ms
static struct timer_list led_timer;
 
#define LED_PINS	6
static int sync_signal;
typedef struct {
		char name[6];
		unsigned int bit_mask;	
		unsigned char status;	//0==>ON 1==>OFF
		unsigned int blinking;	//0==>non-blinking 1==>blinking
		unsigned int wait_sync; //0==>no wait for blink sync 1==>wait blink sync
} shift_led_t;
#define SHIFT_LED_DEFAULT 0xf3
static shift_led_t shift_led_tbl[LED_PINS] = {
			{"syserr", 3, 0, 0, 0},
			{"syssts", 2, 0, 1, 0},
			{"disk_0", 1, 1, 0, 0},
			{"disk_1", 6, 1, 0, 0},
			{"disk_2", 5, 1, 0, 0},
			{"disk_3", 4, 1, 0, 0},};
 
//add_shift -

#define LCM_BACKLIGHT	7	//MPP7 use for LCM backlight
#define LCM_BUS_E	45	//MPP45 use for E
#define E_ENGAGE	1	//engage level high

#define LCM_BUS_RW	44	//MPP44 use for R/W
#define WRITE_LEVEL	0
#define READ_LEVEL	!WRITE_LEVEL

//BOARD_1A #define LCM_BUS_RS	17	//MPP17 use for RS
static int LCM_BUS_RS;		//MPP17 use for RS	//BOARD_1A

#define LCM_DATA_BITS	8
// Data pin mapping
//			           D0  D1  D2  D3  D4  D5  D6  D7	
//--------------------------------------------------------------
static int data_pin_mapping[8] = { 36, 37, 38, 39, 40, 41, 42, 43 };

void set_data_bus_mode(int mode)
{
        lcm_device_t *lcm = &STATIC_DEVICE;
	int	i;

#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "set_data_bus_mode to %d\n", mode);
#endif
	if ( lcm->data_bus_mode != mode) {
        	for ( i=0; i<LCM_DATA_BITS; i++) {
#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "GROUP=%d GPP_BIT=0x%x data=0x%x\n",GPP_GROUP(data_pin_mapping[i]), \
		GPP_BIT(data_pin_mapping[i]), (1 << GPP_ID(data_pin_mapping[i])) & (mode ? MV_GPP_IN : MV_GPP_OUT ));
#endif
               	    mvGppTypeSet(GPP_GROUP(data_pin_mapping[i]), \
                         GPP_BIT(data_pin_mapping[i]), \
                         (1 << GPP_ID(data_pin_mapping[i])) \
			 & (mode ? MV_GPP_IN : MV_GPP_OUT ));
        	}
                lcm->data_bus_mode = mode;
	}

}


void write_lcm(wix_lcm_ioctl_t *Info)
{
	int i;
	unsigned char tmp;
        unsigned long flags;
	lcm_device_t *lcm = &STATIC_DEVICE;

        spin_lock_irqsave(&lcm->lock, flags);

#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "write_lcm: set_data_bus to 0\n");
#endif
	set_data_bus_mode(0);

	tmp = Info->data;
	//set E pin down
	 mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), 0);

#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "write_lcm: prepare_data_bus\n");
#endif
	//prepare data 
	for ( i=0; i<LCM_DATA_BITS; i++) {	
#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "GROUP=%d GPP_BIT=0x%x data=0x%x\n",GPP_GROUP(data_pin_mapping[i]), \
		GPP_BIT(data_pin_mapping[i]), (tmp&0x01) << GPP_ID(data_pin_mapping[i]));
#endif
		mvGppValueSet(GPP_GROUP(data_pin_mapping[i]), 
			      GPP_BIT(data_pin_mapping[i]), 
				(tmp&0x01) << GPP_ID(data_pin_mapping[i]));
 
		tmp = tmp >> 1;
	}
	
#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "Prepare RS: GROUP=%d GPP_BIT=0x%x data=0x%x\n", \
			GPP_GROUP(LCM_BUS_RS), GPP_BIT(LCM_BUS_RS), ((Info->rs & 0x01) << GPP_ID(LCM_BUS_RS)));
#endif
	//prepare RS 
	 mvGppValueSet(GPP_GROUP(LCM_BUS_RS), GPP_BIT(LCM_BUS_RS), ((Info->rs & 0x01) << GPP_ID(LCM_BUS_RS)) );
	udelay(10);	
#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "Prepare RW: GROUP=%d GPP_BIT=0x%x data=0x%x\n", \
	 		GPP_GROUP(LCM_BUS_RW), GPP_BIT(LCM_BUS_RW), ((WRITE_LEVEL&0x01) << GPP_ID(LCM_BUS_RW)) );
#endif
	//prepare R/W 
	 mvGppValueSet(GPP_GROUP(LCM_BUS_RW), GPP_BIT(LCM_BUS_RW), ((WRITE_LEVEL&0x01) << GPP_ID(LCM_BUS_RW)) );

#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "Issue E: GROUP=%d GPP_BIT=0x%x data=0x%x\n", \
	 			GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), (1 << GPP_ID(LCM_BUS_E)) );
#endif
	udelay(10);	
	//issue E
	 mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), (1 << GPP_ID(LCM_BUS_E)) );

	//delay for it completed
	udelay(100);
	
#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "De-Issue E: GROUP=%d GPP_BIT=0x%x data=0x%x\n", \
	 			GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), (0 << GPP_ID(LCM_BUS_E)) );
#endif
	//de-issue E
	mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), ( 0 << GPP_ID(LCM_BUS_E)));

	//delay for it completed
	udelay(100);

//	set_data_bus_mode(1);	//nick use-less
        spin_unlock_irqrestore(&lcm->lock, flags);

	return;
}

void read_lcm(wix_lcm_ioctl_t *Info)
{
        int i;
        unsigned char tmp;
        unsigned long flags;
	MV_32	tmp_data;

        lcm_device_t *lcm = &STATIC_DEVICE;

        spin_lock_irqsave(&lcm->lock, flags);

	set_data_bus_mode(1);
	
	//prepare RS 
	 mvGppValueSet(GPP_GROUP(LCM_BUS_RS), GPP_BIT(LCM_BUS_RS), ((Info->rs & 0x01) << GPP_ID(LCM_BUS_RS)) );
udelay(10);	

        //prepare R/W
         mvGppValueSet(GPP_GROUP(LCM_BUS_RW), GPP_BIT(LCM_BUS_RW), (READ_LEVEL&0x01) << GPP_ID(LCM_BUS_RW));

	udelay(10);	
        //issue E
         mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), (1 << GPP_ID(LCM_BUS_E)) );

        //delay for it completed
        udelay(100);


        //read data back
	tmp_data=tmp=0;
	i = LCM_DATA_BITS-1;
	do {
                tmp_data = mvGppValueGet(GPP_GROUP(data_pin_mapping[i]),
                              GPP_BIT(data_pin_mapping[i]));
#ifdef WIX_DEBUG
    		printk(KERN_NOTICE "pin=%d tmp_data=0x%x\n",data_pin_mapping[i],\
				 (tmp_data >> GPP_ID(data_pin_mapping[i])) & 0x01);
#endif
                tmp |= ((tmp_data >> GPP_ID(data_pin_mapping[i])) & 0x01) << i;
        } while (--i >= 0);

	udelay(10);	
        //de-issue E
        mvGppValueSet(GPP_GROUP(LCM_BUS_E), GPP_BIT(LCM_BUS_E), ( 0 << GPP_ID(LCM_BUS_E)));

        spin_unlock_irqrestore(&lcm->lock, flags);

        Info->data = tmp;

        return;
}

/**
 * Open.
 *
 * @param inode
 * @param filp
 * @return
 */
int wix_lcm_open(struct inode *inode, struct file *filp)
{
    /* Do nothing */
#ifdef WIX_DEBUG
    printk(KERN_NOTICE "wix_lcm_open called\n");
#endif
    return 0;
}

/**
 * Release.
 *
 * @param inode
 * @param filp
 * @return
 */
int wix_lcm_release(struct inode *inode, struct file *filp)
{
    /* Do nothing */
#ifdef WIX_DEBUG
    printk(KERN_NOTICE "wix_lcm_release called\n");
#endif
    return 0;
}

/**
 * Ioctl.
 *
 * @param inode
 * @param filp
 * @param cmd
 * @param arg
 * @return
 */
int wix_lcm_ioctl(struct inode *inode, struct file *filp,
                     unsigned int cmd, unsigned long arg)
{
    wix_lcm_ioctl_t Info;

    if ( copy_from_user(&Info, (wix_lcm_ioctl_t*)arg,
                        sizeof(wix_lcm_ioctl_t)) )  {
        printk(DEVICE_NAME "wix_lcm_ioctl could not copy user space data\n");
        return -EFAULT;
    }
    switch ( cmd ) {
        case IOCTL_WIX_CMD_WRITE:
#ifdef WIX_DEBUG
                printk("wix_lcm_ioctl IOCTL_WIX_CMD_WRITE called RS=0x%2.2x DATA=0x%2.2x\n", Info.rs, Info.data);
#endif
		write_lcm(&Info);
                break;
        case IOCTL_WIX_CMD_READ:
#ifdef WIX_DEBUG
                printk("wix_lcm_ioctl IOCTL_WIX_CMD_READ called RS=0x%x ", Info.rs);
#endif
		read_lcm(&Info);
                if ( copy_to_user((wix_lcm_ioctl_t*)arg, &Info,
                        sizeof(wix_lcm_ioctl_t)) )  {
                        printk(DEVICE_NAME "wix_lcm_ioctl could not copy data to user space\n");
                        return -EFAULT;
                }
                break;
    }
    return 0;
}

#endif

//for /proc_sata_pmode +
static int proc_pmode_read(char *page, char **start, off_t off, int count, int *eof, void *data) {

        int len;
        char *p = page;
	unsigned long current_time_stamp=jiffies;

	if ( (pmode_time_stamp+ 2*HZ) < current_time_stamp )
	    sata_pmode=1;

	if (sata_pmode)
        	p+=sprintf(p, "activity");
	else
        	p+=sprintf(p, "standby");
        len = p - page;
        if (len <= off+count)
                *eof = 1;
        *start = page + off;
        len -= off;
        if (len > count)
                len = count;
        if (len < 0)
                len = 0;

        return len;

}

//for /proc_sata_pmode -

//add for 6281 HDD present +
static int proc_present0_read(char *page, char **start, off_t off, int count, int *eof, void *data) {

        int len;
        char *p = page;
	MV_32 tmp;

        tmp = mvGppValueGet(GPP_GROUP(17), GPP_BIT(17));
	tmp = ((tmp >> GPP_ID(17)) & 0x01);
	
	if (tmp) 
       		p+=sprintf(p, "off\n");
	else
       		p+=sprintf(p, "on\n");

        len = p - page;
        if (len <= off+count)
                *eof = 1;
        *start = page + off;
        len -= off;
        if (len > count)
                len = count;
        if (len < 0)
                len = 0;

        return len;
}

static int proc_present0_write(struct file *file, const char *buffer, unsigned long count, void *data) {

        char kbuffer[255];


        if (!count)
                return -EINVAL;
        if (count > 254)
                count = 254;
        if (copy_from_user(kbuffer, buffer, count))
                return -EFAULT;
        if (kbuffer[count - 1] == 0x0A)
                kbuffer[count - 1] = 0;
        kbuffer[count] = 0;

        if ((!strcmp(kbuffer, "off")) || (!strcmp(kbuffer, "OFF"))) {
                mvGppValueSet(GPP_GROUP(17), GPP_BIT(17),
                                        1 << GPP_ID(17));

        } else if ((!strcmp(kbuffer, "on")) || (!strcmp(kbuffer, "ON"))) {
                mvGppValueSet(GPP_GROUP(17), GPP_BIT(17), 0 << GPP_ID(17));
        }

        return count;
}


static int proc_present1_read(char *page, char **start, off_t off, int count, int *eof, void *data) {

        int len;
        char *p = page;
	MV_32 tmp;

        tmp = mvGppValueGet(GPP_GROUP(14), GPP_BIT(14));
	tmp = ((tmp >> GPP_ID(14)) & 0x01);
	
	if (tmp) 
       		p+=sprintf(p, "off\n");
	else
       		p+=sprintf(p, "on\n");
        len = p - page;
        if (len <= off+count)
                *eof = 1;
        *start = page + off;
        len -= off;
        if (len > count)
                len = count;
        if (len < 0)
                len = 0;

        return len;

}
static int proc_present1_write(struct file *file, const char *buffer, unsigned long count, void *data) {

        char kbuffer[255];


        if (!count)
                return -EINVAL;
        if (count > 254)
                count = 254;
        if (copy_from_user(kbuffer, buffer, count))
                return -EFAULT;
        if (kbuffer[count - 1] == 0x0A)
                kbuffer[count - 1] = 0;
        kbuffer[count] = 0;

        if ((!strcmp(kbuffer, "off")) || (!strcmp(kbuffer, "OFF"))) {
                mvGppValueSet(GPP_GROUP(14), GPP_BIT(14),
                                        1 << GPP_ID(14));

        } else if ((!strcmp(kbuffer, "on")) || (!strcmp(kbuffer, "ON"))) {
                mvGppValueSet(GPP_GROUP(14), GPP_BIT(14), 0 << GPP_ID(14));
        }

        return count;
}

//add for 6281 HDD present -

static int proc_gpio_read(char *page, char **start, off_t off, int count, int *eof, void *data) {

	int len;
        char *p = page;
	int *ppio_num;
	MV_U32 tmp;

	ppio_num = (int *) data;

	tmp = mvGppValueGet(GPP_GROUP(*ppio_num), GPP_BIT(*ppio_num));
	tmp = tmp >> GPP_ID(*ppio_num); 

	p += sprintf(p, "%1.1X\n", tmp);

	len = p - page;
        if (len <= off+count)
                *eof = 1;
        *start = page + off;
        len -= off;
        if (len > count)
                len = count;
        if (len < 0)
                len = 0;

        return len;

}

//add_shift +
static void led_timer_proc(void)
{
	int i;
	unsigned char ttt;

	if(sync_signal) sync_signal=0; else sync_signal=1;
	ttt=0;
	for ( i=0; i< LED_PINS; i++) {
	  if( (sync_signal) && (shift_led_tbl[i].wait_sync)) 
	  { 
	    shift_led_tbl[i].blinking=1;
	    shift_led_tbl[i].wait_sync=0;
	  }
	  if(shift_led_tbl[i].blinking)
	  	shift_led_tbl[i].status = (shift_led_tbl[i].status) ? 0 : 1;	
	  ttt |=(shift_led_tbl[i].status) << shift_led_tbl[i].bit_mask;
	}
	ttt|=0x81;	//nick
        for ( i=0; i<8; i++) {

	gppRegSet(GPP_GROUP(SHIFT_DATA), GPP_DATA_OUT_REG(GPP_GROUP(SHIFT_DATA)),\
	GPP_BIT(SHIFT_DATA),(ttt&0x01) << GPP_ID(SHIFT_DATA));

	gppRegSet(GPP_GROUP(SHIFT_CLK), GPP_DATA_OUT_REG(GPP_GROUP(SHIFT_CLK)),\
	GPP_BIT(SHIFT_CLK),1 << GPP_ID(SHIFT_CLK));

	gppRegSet(GPP_GROUP(SHIFT_CLK), GPP_DATA_OUT_REG(GPP_GROUP(SHIFT_CLK)),\
	GPP_BIT(SHIFT_CLK),0 << GPP_ID(SHIFT_CLK));

        ttt = ttt >> 1;
        }

        mod_timer(&led_timer, jiffies+ (HZ/2));		//0.5 sec

}

//add_shift -


static int proc_gpio_write(struct file *file, const char *buffer, unsigned long count, void *data) {

	int *ppio_num;
	char kbuffer[255];
	unsigned int valid;
	char *argv1, *argv2, *cfg;
	

	if (!count)
                return -EINVAL;
        if (count > 254)
                count = 254;
        if (copy_from_user(kbuffer, buffer, count))
                return -EFAULT;
        if (kbuffer[count - 1] == 0x0A)
                kbuffer[count - 1] = 0;
        kbuffer[count] = 0;
	ppio_num = (int *) data;

//add_shift +
	if (*ppio_num==50) {
	    unsigned int ttt;
	    int i;	
#ifdef WIX_DEBUG
	    printk(KERN_EMERG "write shift register\n");
#endif
	    if ( (kbuffer[0] == '0')&&(kbuffer[1] == 'x')) {
	    	sscanf(kbuffer, "0x%x", &ttt);
#ifdef WIX_DEBUG
                printk(KERN_EMERG "data=0x%x\n", ttt);
#endif
                //prepare data
        	for ( i=0; i<8; i++) {
#ifdef WIX_DEBUG
                    printk(KERN_NOTICE "GROUP=%d GPP_BIT=0x%x data=0x%x\n",
                    GPP_GROUP(SHIFT_DATA), \
                    GPP_BIT(SHIFT_DATA), (ttt&0x01) << GPP_ID(SHIFT_DATA));
#endif
                    mvGppValueSet(GPP_GROUP(SHIFT_DATA), GPP_BIT(SHIFT_DATA),
                                (ttt&0x01) << GPP_ID(SHIFT_DATA));
                    udelay(1);
                    mvGppValueSet(GPP_GROUP(SHIFT_CLK), GPP_BIT(SHIFT_CLK),
                                1 << GPP_ID(SHIFT_CLK));
                    udelay(10);
                    mvGppValueSet(GPP_GROUP(SHIFT_CLK), GPP_BIT(SHIFT_CLK),
                                0 << GPP_ID(SHIFT_CLK));

                    udelay(10);
                    ttt = ttt >> 1;
          	}
	    }	
	    else {
		int i;
		cfg=kbuffer;
        	for (argv1 = cfg; *cfg && *cfg != ' ' && *cfg != '\t'; cfg++) ;
        	if (*cfg) {
                	*cfg = '\0';
                	cfg++;
        	}
        	for (argv2 = cfg; *cfg && *cfg != ' ' && *cfg != '\t'; cfg++) ;
        	if (*cfg) {
                	*cfg = '\0';
                	cfg++;
        	}

		for(i=0; i < LED_PINS; i++) {
		    if (!strncmp(argv1,shift_led_tbl[i].name,6)) {
			if(!strncmp(argv2, "on", 2)) 
			{
			    shift_led_tbl[i].status= i>1 ? 0:1;	
			    shift_led_tbl[i].blinking=0; 
			    break;
			};
			if(!strncmp(argv2, "off", 3)) 
			{
			    shift_led_tbl[i].status= i>1 ? 1:0 ; 
			    shift_led_tbl[i].blinking=0; 
			    break;
			};	
			if(!strncmp(argv2, "blink", 5)) 
			{
			    if( !((shift_led_tbl[i].blinking)||(shift_led_tbl[i].wait_sync))) {
			        shift_led_tbl[i].status= i>1 ? 1:0; 
			        shift_led_tbl[i].wait_sync=1; 
			    }
			    break;
			};	
			break;
		    }
	
		} //for
	    }
        	return count;
	}

//add_shift -


	if ((!strcmp(kbuffer, "on")) || (!strcmp(kbuffer, "ON"))) {
#ifdef WIX_DEBUG
		printk(KERN_EMERG "gpio%2d:on\n", *ppio_num);
#endif
		mvGppValueSet(GPP_GROUP(*ppio_num), GPP_BIT(*ppio_num), 
					1 << GPP_ID(*ppio_num));		

	} else if ((!strcmp(kbuffer, "off")) || (!strcmp(kbuffer, "OFF"))) {
#ifdef WIX_DEBUG
		printk(KERN_EMERG "gpio%2d:off\n", *ppio_num);
#endif
		mvGppValueSet(GPP_GROUP(*ppio_num), GPP_BIT(*ppio_num),					0 << GPP_ID(*ppio_num));		
	
	} else {
		valid = (unsigned int) simple_strtol(kbuffer, (char **) NULL, 10);
		valid &= 0x01;
		if (valid) {
		} else {
		}		
	}
	
        return count;
	
}

static int __init gpio_init(void) {
	
	int loop;
	struct proc_dir_entry *entry;
	MV_U32	tmp;
#ifdef SUPPORT_LCM
	lcm_device_t *lcm = &STATIC_DEVICE;
    	int major_num;
#endif
//for /proc_sata_pmode +
	proc_pio2 = proc_mkdir("sata", NULL);
	entry = create_proc_read_entry("pmode", 0, proc_pio2, proc_pmode_read, NULL);
//for /proc_sata_pmode -

//add for 6281 HDD present +
//	proc_pio3= proc_mkdir("sata", NULL);
	entry = create_proc_read_entry("present0", 0, proc_pio2, proc_present0_read, NULL);
	entry->write_proc = proc_present0_write;
	tmp = MV_REG_READ(mvCtrlMppRegGet(MPP_GROUP(17)));
	tmp &= 0xffffff0f;
	MV_REG_WRITE(mvCtrlMppRegGet(MPP_GROUP(17)), tmp);
	// set to output mode
        mvGppTypeSet(GPP_GROUP(17), GPP_BIT(17), \
                         (1 << GPP_ID(17)) & MV_GPP_OUT );

	entry = create_proc_read_entry("present1", 0, proc_pio2, proc_present1_read, NULL);
	entry->write_proc = proc_present1_write;
	tmp = MV_REG_READ(mvCtrlMppRegGet(MPP_GROUP(14)));
	tmp &= 0xf0ffffff;
	MV_REG_WRITE(mvCtrlMppRegGet(MPP_GROUP(14)), tmp);
	// set to output mode
        mvGppTypeSet(GPP_GROUP(14), GPP_BIT(14), \
                         (1 << GPP_ID(14)) & MV_GPP_OUT );
//add for 6281 HDD present -
	
	printk(KERN_EMERG "wix gpio_init\n");
	proc_pio = proc_mkdir("wixgpio", NULL);
//BOARD_1A +
extern int shasta_board_sample_1b;
	if(shasta_board_sample_1b == 1)
		LCM_BUS_RS = 17;
	else	
		LCM_BUS_RS = 35;
//BOARD_1A -
	for (loop = 0; loop < MAX_PIO; loop++) {
		sprintf(pio[loop].proc_name, "gpio%02d", loop);
		pio[loop].proc_num = loop;
		entry = create_proc_read_entry(pio[loop].proc_name, 0, proc_pio, proc_gpio_read, &pio[loop].proc_num);
		switch (loop) {
			case LCM_BACKLIGHT:
			case 28:	//HDD Power control, default high
//			case 48:	//CPU power, default high
				if ( !mvGppValueGet(GPP_GROUP(loop), GPP_BIT(loop)) ) {
					printk("++++ wrong initial value for GPP%2d, fixed it by wixgpio\n", loop);
					mvGppValueSet(GPP_GROUP(loop), GPP_BIT(loop), 1);
				}
			case 50:	//add_shift
			case 36:	//LCM D0
			case 37:	//LCM D1
			case 38:	//LCM D2
			case 39:	//LCM D3
			case 40:	//LCM D4
			case 41:	//LCM D5	
			case 42:	//LCM D6
			case 43:	//LCM D7	
			case 44:	//LCM RW
			case 45:	//LCM E
//BOARD_1A			case 17:	//LCM RS	
			case 12:	//Shift register 74AHC164
			case 13:	//Shift register 74AHC164
//BOARD_1A			case 14:	//Shift register 74AHC164
				//check all of these are match what we want to set to GPIO mode?
				tmp = MV_REG_READ(mvCtrlMppRegGet(MPP_GROUP(loop)));
				tmp &= MPP_BITS(loop);
				if (tmp) printk("!!!! wrong MPP%2d setting !!!!\n", loop);

				if (entry)
	                		entry->write_proc = proc_gpio_write;
				break;
			default:
				break;
		}
	}

#ifdef SUPPORT_LCM
	//Init LCM device
	printk(KERN_NOTICE DEVICE_NAME ": WIX LCM driver version %s\n", WIX_LCM_VERSION);
	memset(lcm, 0, sizeof(*lcm));

    	spin_lock_init(&lcm->lock);

    	if ( alloc_chrdev_region(&lcm->devnum, 0, 1, DEVICE_NAME) < 0) {
        	printk(DEVICE_NAME ": unable to alloc char device\n");
        	goto out;
    	}
    	//init cdev struct for adding device to kernel
    	cdev_init(&lcm_cdev, &file_ops);
    	lcm_cdev.owner = THIS_MODULE;
    	lcm_cdev.ops = &file_ops;

    	/* Get registered. */
    	major_num = MKDEV( MAJOR(lcm->devnum), 0);
    	if (cdev_add(&lcm_cdev, major_num, 1)) {
        	printk(DEVICE_NAME ": unable to add char device\n");
        	goto out;
    	}	
//add_shift +
	//set MPP12 and MPP13 to output mode
        mvGppTypeSet(GPP_GROUP(SHIFT_CLK), GPP_BIT(SHIFT_CLK), \
                         (1 << GPP_ID(SHIFT_CLK)) & MV_GPP_OUT );
        mvGppTypeSet(GPP_GROUP(SHIFT_DATA), GPP_BIT(SHIFT_DATA), \
                         (1 << GPP_ID(SHIFT_DATA)) & MV_GPP_OUT );
	mvGppValueSet(GPP_GROUP(SHIFT_CLK), GPP_BIT(SHIFT_CLK), 0);
 
	sync_signal=0;
	//add timer for led 
        led_timer.function = led_timer_proc;
        led_timer.data = &shift_led_tbl;
        init_timer(&led_timer);
        mod_timer(&led_timer, jiffies+ (HZ/2));		//0.5 sec
//add_shift -

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
	lcm->data_bus_mode = -1; //unknow mode
	return 0;
out:
    	return -ENOMEM;
#endif	
	
        return 0;
}

static void __exit gpio_exit(void) {

	int i;

        del_timer_sync(&led_timer);
 

	for (i=0;i<MAX_PIO;i++) {
		remove_proc_entry(pio[i].proc_name, proc_pio);
	}
//for /proc_sata_pmode +
	remove_proc_entry("pmode", proc_pio2);
//add for 6281 HDD present +
	remove_proc_entry("present0", proc_pio2);
	remove_proc_entry("present1", proc_pio2);
//add for 6281 HDD present - 
	remove_proc_entry("sata", NULL);
//for /proc_sata_pmode -
	remove_proc_entry("wixgpio", NULL);
	
#ifdef SUPPORT_LCM

#endif
}



MODULE_DESCRIPTION("GPIO Module for Wistron SXM");
MODULE_LICENSE("GPL");
module_init(gpio_init);
module_exit(gpio_exit);

