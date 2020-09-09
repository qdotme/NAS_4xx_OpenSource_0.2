/*
 *
 *  Copyright (C) 2006 Marvell Technology Group Ltd. All Rights Reserved.
 *  linux_main.c
 *
 *
 */

#include "mv_include.h"
#include "hba_header.h"
#include "linux_main.h"

#include "hba_exp.h"

#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 7)
unsigned int mv_dbg_opts = 0;
module_param(mv_dbg_opts, uint, S_IRWXU | S_IRWXG);
#endif /* LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 7) */

static const struct pci_device_id mv_pci_ids[] = {
#ifdef ODIN_DRIVER
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_6320)},
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_6340)},
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_6440)},
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_6480)},
	{0}
#endif

#ifdef THOR_DRIVER
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_THORLITE_0S1P)},
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_THORLITE_2S1P)},
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_THOR_4S1P)},
	{PCI_DEVICE(VENDOR_ID, DEVICE_ID_THOR_4S1P_NEW)},
	{PCI_DEVICE(VENDOR_ID,
	           DEVICE_ID_THORLITE_2S1P_WITH_FLASH)},
	{0}
#endif

};

#ifdef SHASTA_BOARD
static struct file_operations mv_fops = {
        .owner   =    THIS_MODULE,
        .open    =    NULL,
        .ioctl   =    NULL,
        .release =    NULL
};
int mv_major = -1 ;
#endif

/* notifier block to get notified on system shutdown/halt/reboot/down */
static int mv_linux_halt(struct notifier_block *nb, unsigned long event,
			 void *buf)
{
	switch (event) {
	case SYS_RESTART:
	case SYS_HALT:
	case SYS_POWER_OFF:
		mv_hba_stop(NULL);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block mv_linux_notifier = {
	mv_linux_halt, NULL, 0
};

static int mv_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	unsigned int ret = PCIBIOS_SUCCESSFUL;
	int err = 0;
	
	ret = pci_enable_device(dev);
	if (ret) {
		MV_PRINT(" enable device failed.\n");
		return ret;
	}
	
	ret = pci_request_regions(dev, mv_driver_name);
	if (ret)
		goto err_req_region;
	
	
	if ( !pci_set_dma_mask(dev, DMA_64BIT_MASK) ) {
		ret = pci_set_consistent_dma_mask(dev, DMA_64BIT_MASK);
		if (ret) {
			ret = pci_set_consistent_dma_mask(dev, 
							  DMA_32BIT_MASK);
			if (ret)
				goto err_dma_mask;
		}
	} else {
		ret = pci_set_dma_mask(dev, DMA_32BIT_MASK);
		if (ret)
			goto err_dma_mask;
		
		ret = pci_set_consistent_dma_mask(dev, DMA_32BIT_MASK);
		if (ret) 
			goto err_dma_mask;
		
	}
		
	pci_set_master(dev);
#ifdef SHASTA_BOARD
	//set Transmitter Pre-Emphasis to 4
{
        unsigned int pci_bar5, tmp;
       	void __iomem * virt_pci_bar5;

	printk("set 6121 Transmitter Pre-Emphasis to 4\n");
       	pci_bar5 = pci_resource_start(dev, 5);
       	virt_pci_bar5 = ioremap(pci_bar5, 0x128);
       	if (!virt_pci_bar5) {
               ret = -ENODEV;
               return ret;
       	}
#define PHY_INDEX 0x178
#define PHY_DATA 0x17c
#define PHY1_INDEX 0x1f8
#define PHY1_DATA 0x1fc

       	tmp = 0xc;
       	writel(tmp, virt_pci_bar5+PHY_INDEX);
//       	printk("write %x to INDEX:%8.8x\n", tmp, virt_pci_bar5+PHY_INDEX);
       	tmp = readl(virt_pci_bar5+PHY_DATA);
//       	printk("default = %x from DATA:%8.8x\n", tmp, virt_pci_bar5+PHY_DATA);
       	tmp &= 0xffffff8f;
       	tmp |= 0x00000040;
       	writel(tmp, virt_pci_bar5+PHY_DATA);
//       	tmp = readl(virt_pci_bar5+PHY_DATA);
//       	printk("readback again = %x from DATA:%8.8x\n", tmp, virt_pci_bar5+PHY_DATA);

       	tmp = 0xc;
       	writel(tmp, virt_pci_bar5+PHY1_INDEX);
//       	printk("write %x to INDEX:%8.8x\n", tmp, virt_pci_bar5+PHY1_INDEX);
       	tmp = readl(virt_pci_bar5+PHY1_DATA);
//       	printk("default = %x from DATA:%8.8x\n", tmp, virt_pci_bar5+PHY1_DATA);
       	tmp &= 0xffffff8f;
       	tmp |= 0x00000040;
       	writel(tmp, virt_pci_bar5+PHY1_DATA);
//       	tmp = readl(virt_pci_bar5+PHY1_DATA);
//       	printk("readback again= %x from DATA:%8.8x\n", tmp, virt_pci_bar5+PHY1_DATA);
       	iounmap(virt_pci_bar5);
}
#endif
	printk("Marvell Storage Controller is found, using IRQ %d.\n",
	       dev->irq);
	
	ret = mv_hba_init(dev, MV_MAX_IO);
	if (ret) {
		MV_DBG(DMSG_HBA, "Error no %d.\n", ret);
		ret = -ENOMEM;
		goto err_dma_mask;
	}

	MV_DPRINT(("Start mv_hba_start.\n"));
	
	if (mv_hba_start(dev)) {
		ret = -ENODEV;
		goto err_mod_start;
	}
	
	if (__mv_get_adapter_count() == 1) {
		register_reboot_notifier(&mv_linux_notifier); 
	}
#ifdef SHASTA_BOARD
// I had to add an register char device, or major number will shift
        if (-1 == mv_major) {
                if ((mv_major = register_chrdev(0,
                                                mv_driver_name,
                                                &mv_fops)) < 0) {
                        printk("THOR : Failed to register character device");
                        ret = -ENODEV;
                        goto err_register_chrdev;
                }
        }
{       //engage activity led
        u32 ttt;
        pci_read_config_dword(dev, 0x44, &ttt);
        ttt |= 0x40;
        ttt &= 0xffff7fff;      //act
        pci_write_config_dword(dev, 0x44, ttt);
}
#endif
	MV_DPRINT(("Finished mv_probe.\n"));

	return 0;

#ifdef SHASTA_BOARD
err_register_chrdev:
        unregister_reboot_notifier(&mv_linux_notifier);
#endif

err_mod_start:
	err++;
	mv_hba_stop(dev);
	mv_hba_release(dev);
err_dma_mask:
	err++;
	pci_release_regions(dev);
err_req_region:
	err++;
	pci_disable_device(dev);

	MV_PRINT(" error counter %d.\n", err);
	return ret;
}

static void __devexit mv_remove(struct pci_dev *dev)
{
#ifdef SHASTA_BOARD
        if (mv_major >= 0) {
                unregister_chrdev(mv_major, mv_driver_name);
        }
#endif
	mv_hba_stop(dev);
	mv_hba_release(dev);
	
	pci_release_regions(dev);
	pci_disable_device(dev);

	if (__mv_get_adapter_count() == 0) {
		unregister_reboot_notifier(&mv_linux_notifier);
	}
	MV_DPRINT(("Marvell linux driver removed !\n"));
}

static struct pci_driver mv_pci_driver = {
	.name     = "mv_"mv_driver_name,
	.id_table = mv_pci_ids,
	.probe    = mv_probe,
	.remove   = __devexit_p(mv_remove),
};

static int __init mv_linux_driver_init(void)
{
        /* default to show no msg */
#ifdef __MV_DEBUG__
	//mv_dbg_opts |= (DMSG_CORE|DMSG_HBA|DMSG_KERN|DMSG_SCSI|DMSG_FREQ);
	mv_dbg_opts |= (DMSG_CORE|DMSG_HBA|DMSG_KERN|DMSG_SCSI);
	mv_dbg_opts |= (DMSG_CORE_EH|DMSG_RAID|DMSG_SAS|DMSG_RES);

	//mv_dbg_opts |= DMSG_PROF_FREQ; 
	//mv_dbg_opts |= DMSG_SCSI_FREQ;

#endif /* __MV_DEBUG__ */
	hba_house_keeper_init();
	
	return pci_register_driver(&mv_pci_driver);
}

static void __exit mv_linux_driver_exit(void)
{
	pci_unregister_driver(&mv_pci_driver);
	hba_house_keeper_exit();
}

MODULE_AUTHOR ("Marvell Technolog Group Ltd.");
#ifdef ODIN_DRIVER
MODULE_DESCRIPTION ("ODIN SAS hba driver");

#ifdef RAID_DRIVER
MODULE_LICENSE("Proprietary");
#else /* RAID_DRIVER */
MODULE_LICENSE("GPL"); 
#endif /* RAID_DRIVER */
#endif

#ifdef THOR_DRIVER
MODULE_DESCRIPTION ("Thor ATA HBA Driver");

MODULE_LICENSE("Proprietary");

#endif

MODULE_DEVICE_TABLE(pci, mv_pci_ids);

module_init(mv_linux_driver_init);
module_exit(mv_linux_driver_exit);

