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

#include <asm/uaccess.h>
#include <asm/io.h>


#define	MAX_PIO	20

static struct piostruct {
	int proc_num;
	char proc_name[16];
} pio[MAX_PIO];

static struct proc_dir_entry *proc_pio;


static int proc_gpio_read(char *page, char **start, off_t off, int count, int *eof, void *data) {

	int len;
        char *p = page;
	int *ppio_num;

	ppio_num = (int *) data;

	p += sprintf(p, "%X\n", 00);

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

static int proc_gpio_write(struct file *file, const char *buffer, unsigned long count, void *data) {

	int *ppio_num;
	char kbuffer[255];
	unsigned int valid;
	

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

	if ((!strcmp(kbuffer, "on")) || (!strcmp(kbuffer, "ON"))) {
#ifdef WIX_DEBUG
		printk(KERN_EMERG "gpio:on\n");
#endif

	} else if ((!strcmp(kbuffer, "off")) || (!strcmp(kbuffer, "OFF"))) {
	
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

	
#ifdef WIX_DEBUG
	printk(KERN_EMERG "gpio_init\n");
#endif
	proc_pio = proc_mkdir("wixgpio", NULL);

	for (loop = 0; loop < MAX_PIO; loop++) {
		sprintf(pio[loop].proc_name, "gpio%02d", loop);
		pio[loop].proc_num = loop;
		entry = create_proc_read_entry(pio[loop].proc_name, 0, proc_pio, proc_gpio_read, &pio[loop].proc_num);
		switch (loop) {
			case 0:
			case 1:
			case 8:
			case 9:
			case 10:
			case 19:
				if (entry)
	                		entry->write_proc = proc_gpio_write;
				break;
			default:
				break;
		}
	}

	//initial button gpio , set GPIO IN
	//TODO put gpio direction/initial value here

        return 0;
}

static void __exit gpio_exit(void) {

	int loop;


	for (loop=0;loop<MAX_PIO;loop++) {
		remove_proc_entry(pio[loop].proc_name, proc_pio);
	}
	remove_proc_entry("wixgpio", NULL);
	
}



MODULE_DESCRIPTION("GPIO Module for Wistron SXM");
MODULE_LICENSE("GPL");
module_init(gpio_init);
module_exit(gpio_exit);

