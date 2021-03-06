/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.12.6-arm1
 * Mon Jun 16 21:15:03 2008
 */
#define AUTOCONF_INCLUDED
#define CONFIG_ARM 1
#define CONFIG_MMU 1
#define CONFIG_UID16 1
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#define CONFIG_GENERIC_CALIBRATE_DELAY 1
#define CONFIG_GENERIC_IOMAP 1

/*
 * Code maturity level options
 */
#define CONFIG_EXPERIMENTAL 1
#define CONFIG_CLEAN_COMPILE 1
#define CONFIG_BROKEN_ON_SMP 1
#define CONFIG_INIT_ENV_ARG_LIMIT 32

/*
 * General setup
 */
#define CONFIG_LOCALVERSION ""
#define CONFIG_SWAP 1
#define CONFIG_SYSVIPC 1
#undef CONFIG_POSIX_MQUEUE
#undef CONFIG_BSD_PROCESS_ACCT
#define CONFIG_SYSCTL 1
#undef CONFIG_AUDIT
#define CONFIG_HOTPLUG 1
#define CONFIG_KOBJECT_UEVENT 1
#undef CONFIG_IKCONFIG
#undef CONFIG_EMBEDDED
#define CONFIG_KALLSYMS 1
#undef CONFIG_KALLSYMS_EXTRA_PASS
#define CONFIG_PRINTK 1
#define CONFIG_BUG 1
#define CONFIG_BASE_FULL 1
#define CONFIG_FUTEX 1
#define CONFIG_EPOLL 1
#define CONFIG_CC_OPTIMIZE_FOR_SIZE 1
#define CONFIG_SHMEM 1
#define CONFIG_CC_ALIGN_FUNCTIONS 0
#define CONFIG_CC_ALIGN_LABELS 0
#define CONFIG_CC_ALIGN_LOOPS 0
#define CONFIG_CC_ALIGN_JUMPS 0
#undef CONFIG_TINY_SHMEM
#define CONFIG_BASE_SMALL 0

/*
 * Loadable module support
 */
#define CONFIG_MODULES 1
#define CONFIG_MODULE_UNLOAD 1
#undef CONFIG_MODULE_FORCE_UNLOAD
#define CONFIG_OBSOLETE_MODPARM 1
#undef CONFIG_MODVERSIONS
#undef CONFIG_MODULE_SRCVERSION_ALL
#undef CONFIG_KMOD

/*
 * System Type
 */
#undef CONFIG_ARCH_CLPS7500
#undef CONFIG_ARCH_CLPS711X
#undef CONFIG_ARCH_CO285
#undef CONFIG_ARCH_EBSA110
#undef CONFIG_ARCH_CAMELOT
#undef CONFIG_ARCH_FOOTBRIDGE
#undef CONFIG_ARCH_INTEGRATOR
#define CONFIG_ARCH_FEROCEON 1
#undef CONFIG_ARCH_IOP3XX
#undef CONFIG_ARCH_IXP4XX
#undef CONFIG_ARCH_IXP2000
#undef CONFIG_ARCH_L7200
#undef CONFIG_ARCH_PXA
#undef CONFIG_ARCH_RPC
#undef CONFIG_ARCH_SA1100
#undef CONFIG_ARCH_S3C2410
#undef CONFIG_ARCH_SHARK
#undef CONFIG_ARCH_LH7A40X
#undef CONFIG_ARCH_OMAP
#undef CONFIG_ARCH_VERSATILE
#undef CONFIG_ARCH_IMX
#undef CONFIG_ARCH_H720X

/*
 * Feroceon SoC options
 */
#undef CONFIG_MV88F1181
#undef CONFIG_MV88F1281
#define CONFIG_MV88F5181 1
#undef CONFIG_MV88F5181L
#undef CONFIG_MV88F5182
#undef CONFIG_MV88F5082
#undef CONFIG_MV88F5180N
#undef CONFIG_MV88F6082
#undef CONFIG_MV88W8660
#undef CONFIG_JTAG_DEBUG

/*
 * Feroceon SoC Included Features
 */
#undef CONFIG_MV_INCLUDE_PCI
#define CONFIG_MV_INCLUDE_PEX 1
#define CONFIG_MV_INCLUDE_IDMA 1
#define CONFIG_MV_INCLUDE_USB 1
#define CONFIG_MV_INCLUDE_NAND 1
#define CONFIG_MV_INCLUDE_GIG_ETH 1
#define CONFIG_MV_GPP_MAX_PINS 32
#define CONFIG_MV_DCACHE_SIZE 0x8000
#define CONFIG_MV_ICACHE_SIZE 0x8000

/*
 * Feroceon SoC MTD support
 */
#undef CONFIG_MV_NAND
#undef CONFIG_ARCH_SUPPORTS_BIG_ENDIAN
#define CONFIG_USE_DSP 1
#define CONFIG_MV_USE_IDMA_ENGINE 1
#define CONFIG_MV_IDMA_COPYUSER 1
#define CONFIG_MV_IDMA_COPYUSER_THRESHOLD 1260
#define CONFIG_MV_IDMA_MEMZERO 1
#define CONFIG_MV_IDMA_MEMZERO_THRESHOLD 192
#define CONFIG_MV_IDMA_MEMCOPY 1
#define CONFIG_MV_IDMA_MEMCOPY_THRESHOLD 128
#define CONFIG_FEROCEON_PROC 1
#undef CONFIG_MV_GENERIC_NAS_FS
#define CONFIG_UBOOT_STRUCT 1
#undef CONFIG_MV_DBG_TRACE

/*
 * SoC Networking support
 */
#define CONFIG_MV_ETHERNET 1
#undef CONFIG_MV_GATEWAY
#define CONFIG_MV_ETH_NAME "egiga"
#undef CONFIG_ETH_MULTI_Q
#undef CONFIG_EGIGA_STATIS
#define CONFIG_ETH_0_MACADDR "000000000051"
#undef CONFIG_EGIGA_PROC
#define CONFIG_SCSI_MVSATA 1

/*
 * Sata options
 */
#define CONFIG_MV_SATA_SUPPORT_ATAPI 1
#define CONFIG_MV_SATA_ENABLE_1MB_IOS 1
#define CONFIG_SATA_NO_DEBUG 1
#undef CONFIG_SATA_DEBUG_ON_ERROR
#undef CONFIG_SATA_FULL_DEBUG

/*
 * Processor Type
 */
#define CONFIG_CPU_32 1
#define CONFIG_CPU_ARM926T 1
#define CONFIG_CPU_32v5 1
#define CONFIG_CPU_ABRT_EV5TJ 1
#define CONFIG_CPU_CACHE_VIVT 1
#define CONFIG_CPU_COPY_V4WB 1
#define CONFIG_CPU_TLB_V4WBI 1

/*
 * Processor Features
 */
#define CONFIG_ARM_THUMB 1
#undef CONFIG_CPU_ICACHE_DISABLE
#undef CONFIG_CPU_DCACHE_DISABLE
#undef CONFIG_CPU_DCACHE_WRITETHROUGH
#undef CONFIG_CPU_CACHE_ROUND_ROBIN

/*
 * Bus support
 */
#define CONFIG_ISA_DMA_API 1
#define CONFIG_PCI 1
#define CONFIG_PCI_LEGACY_PROC 1
#undef CONFIG_PCI_NAMES

/*
 * PCCARD (PCMCIA/CardBus) support
 */
#undef CONFIG_PCCARD

/*
 * Kernel Features
 */
#undef CONFIG_SMP
#undef CONFIG_PREEMPT
#define CONFIG_AEABI 1
#define CONFIG_OABI_COMPAT 1
#define CONFIG_REORDER 1
#undef CONFIG_DISCONTIGMEM
#undef CONFIG_LEDS
#define CONFIG_ALIGNMENT_TRAP 1

/*
 * Boot options
 */
#define CONFIG_ZBOOT_ROM_TEXT 0x0
#define CONFIG_ZBOOT_ROM_BSS 0x0
#define CONFIG_CMDLINE "console=ttyAM0 root=/dev/nfs rw nfsroot=10.4.50.31:/home/rshitrit/cramfs-1.1/cramfs-1.1/shoko2 mem=32M ip=10.4.50.99:10.4.50.31:::ARM:eth0:none"
#undef CONFIG_XIP_KERNEL

/*
 * Floating point emulation
 */

/*
 * At least one emulation must be selected
 */
#undef CONFIG_FPE_NWFPE
#undef CONFIG_FPE_FASTFPE
#define CONFIG_VFP 1

/*
 * Userspace binary formats
 */
#define CONFIG_BINFMT_ELF 1
#undef CONFIG_BINFMT_AOUT
#undef CONFIG_BINFMT_MISC
#undef CONFIG_ARTHUR

/*
 * Power management options
 */
#undef CONFIG_PM

/*
 * Device Drivers
 */

/*
 * Generic Driver Options
 */
#undef CONFIG_STANDALONE
#undef CONFIG_PREVENT_FIRMWARE_BUILD
#undef CONFIG_FW_LOADER

/*
 * Memory Technology Devices (MTD)
 */
#define CONFIG_MTD 1
#undef CONFIG_MTD_DEBUG
#undef CONFIG_MTD_CONCAT
#define CONFIG_MTD_PARTITIONS 1
#undef CONFIG_MTD_REDBOOT_PARTS
#undef CONFIG_MTD_CMDLINE_PARTS
#undef CONFIG_MTD_AFS_PARTS

/*
 * User Modules And Translation Layers
 */
#define CONFIG_MTD_CHAR 1
#define CONFIG_MTD_BLOCK 1
#undef CONFIG_FTL
#undef CONFIG_NFTL
#undef CONFIG_INFTL

/*
 * RAM/ROM/Flash chip drivers
 */
#define CONFIG_MTD_CFI 1
#undef CONFIG_MTD_JEDECPROBE
#define CONFIG_MTD_GEN_PROBE 1
#define CONFIG_MTD_CFI_ADV_OPTIONS 1
#define CONFIG_MTD_CFI_NOSWAP 1
#undef CONFIG_MTD_CFI_BE_BYTE_SWAP
#undef CONFIG_MTD_CFI_LE_BYTE_SWAP
#define CONFIG_MTD_CFI_GEOMETRY 1
#define CONFIG_MTD_MAP_BANK_WIDTH_1 1
#undef CONFIG_MTD_MAP_BANK_WIDTH_2
#undef CONFIG_MTD_MAP_BANK_WIDTH_4
#undef CONFIG_MTD_MAP_BANK_WIDTH_8
#undef CONFIG_MTD_MAP_BANK_WIDTH_16
#undef CONFIG_MTD_MAP_BANK_WIDTH_32
#define CONFIG_MTD_CFI_I1 1
#undef CONFIG_MTD_CFI_I2
#undef CONFIG_MTD_CFI_I4
#undef CONFIG_MTD_CFI_I8
#undef CONFIG_MTD_CFI_INTELEXT
#define CONFIG_MTD_CFI_AMDSTD 1
#define CONFIG_MTD_CFI_AMDSTD_RETRY 0
#undef CONFIG_MTD_CFI_STAA
#define CONFIG_MTD_CFI_UTIL 1
#undef CONFIG_MTD_RAM
#undef CONFIG_MTD_ROM
#undef CONFIG_MTD_ABSENT

/*
 * Mapping drivers for chip access
 */
#undef CONFIG_MTD_COMPLEX_MAPPINGS
#undef CONFIG_MTD_PHYSMAP
#define CONFIG_WIXNAS_SXM 1
#undef CONFIG_MTD_ARM_INTEGRATOR
#undef CONFIG_MTD_EDB7312

/*
 * Self-contained MTD device drivers
 */
#undef CONFIG_MTD_PMC551
#undef CONFIG_MTD_SLRAM
#undef CONFIG_MTD_PHRAM
#undef CONFIG_MTD_MTDRAM
#undef CONFIG_MTD_BLKMTD
#undef CONFIG_MTD_BLOCK2MTD

/*
 * Disk-On-Chip Device Drivers
 */
#undef CONFIG_MTD_DOC2000
#undef CONFIG_MTD_DOC2001
#undef CONFIG_MTD_DOC2001PLUS

/*
 * NAND Flash Device Drivers
 */
#undef CONFIG_MTD_NAND

/*
 * Parallel port support
 */
#undef CONFIG_PARPORT

/*
 * Plug and Play support
 */

/*
 * Block devices
 */
#undef CONFIG_BLK_CPQ_DA
#undef CONFIG_BLK_CPQ_CISS_DA
#undef CONFIG_BLK_DEV_DAC960
#undef CONFIG_BLK_DEV_UMEM
#undef CONFIG_BLK_DEV_COW_COMMON
#undef CONFIG_BLK_DEV_LOOP
#undef CONFIG_BLK_DEV_NBD
#undef CONFIG_BLK_DEV_SX8
#undef CONFIG_BLK_DEV_UB
#undef CONFIG_BLK_DEV_RAM
#define CONFIG_BLK_DEV_RAM_COUNT 16
#define CONFIG_INITRAMFS_SOURCE ""
#define CONFIG_LBD 1
#undef CONFIG_CDROM_PKTCDVD

/*
 * IO Schedulers
 */
#define CONFIG_IOSCHED_NOOP 1
#define CONFIG_IOSCHED_AS 1
#define CONFIG_IOSCHED_DEADLINE 1
#define CONFIG_IOSCHED_CFQ 1
#undef CONFIG_ATA_OVER_ETH

/*
 * SCSI device support
 */
#define CONFIG_SCSI 1
#define CONFIG_SCSI_PROC_FS 1

/*
 * SCSI support type (disk, tape, CD-ROM)
 */
#define CONFIG_BLK_DEV_SD 1
#undef CONFIG_CHR_DEV_ST
#undef CONFIG_CHR_DEV_OSST
#undef CONFIG_BLK_DEV_SR
#define CONFIG_CHR_DEV_SG 1

/*
 * Some SCSI devices (e.g. CD jukebox) support multiple LUNs
 */
#define CONFIG_SCSI_MULTI_LUN 1
#undef CONFIG_SCSI_CONSTANTS
#undef CONFIG_SCSI_LOGGING

/*
 * SCSI Transport Attributes
 */
#undef CONFIG_SCSI_SPI_ATTRS
#undef CONFIG_SCSI_FC_ATTRS
#undef CONFIG_SCSI_ISCSI_ATTRS

/*
 * SCSI low-level drivers
 */
#undef CONFIG_BLK_DEV_3W_XXXX_RAID
#undef CONFIG_SCSI_3W_9XXX
#undef CONFIG_SCSI_ACARD
#undef CONFIG_SCSI_AACRAID
#undef CONFIG_SCSI_AIC7XXX
#undef CONFIG_SCSI_AIC7XXX_OLD
#undef CONFIG_SCSI_AIC79XX
#undef CONFIG_SCSI_DPT_I2O
#undef CONFIG_MEGARAID_NEWGEN
#undef CONFIG_MEGARAID_LEGACY
#undef CONFIG_SCSI_SATA
#undef CONFIG_SCSI_BUSLOGIC
#undef CONFIG_SCSI_DMX3191D
#undef CONFIG_SCSI_EATA
#undef CONFIG_SCSI_FUTURE_DOMAIN
#undef CONFIG_SCSI_GDTH
#undef CONFIG_SCSI_IPS
#undef CONFIG_SCSI_INITIO
#undef CONFIG_SCSI_INIA100
#undef CONFIG_SCSI_SYM53C8XX_2
#undef CONFIG_SCSI_IPR
#undef CONFIG_SCSI_QLOGIC_FC
#undef CONFIG_SCSI_QLOGIC_1280
#define CONFIG_SCSI_QLA2XXX 1
#undef CONFIG_SCSI_QLA21XX
#undef CONFIG_SCSI_QLA22XX
#undef CONFIG_SCSI_QLA2300
#undef CONFIG_SCSI_QLA2322
#undef CONFIG_SCSI_QLA6312
#undef CONFIG_SCSI_LPFC
#undef CONFIG_SCSI_DC395x
#undef CONFIG_SCSI_DC390T
#undef CONFIG_SCSI_NSP32
#undef CONFIG_SCSI_DEBUG

/*
 * Multi-device support (RAID and LVM)
 */
#define CONFIG_MD 1
#define CONFIG_BLK_DEV_MD 1
#define CONFIG_MD_LINEAR 1
#define CONFIG_MD_RAID0 1
#define CONFIG_MD_RAID1 1
#undef CONFIG_MD_RAID10
#define CONFIG_MD_RAID5 1
#undef CONFIG_MD_RAID6
#undef CONFIG_MD_MULTIPATH
#undef CONFIG_MD_FAULTY
#define CONFIG_BLK_DEV_DM 1
#undef CONFIG_DM_CRYPT
#undef CONFIG_DM_SNAPSHOT
#undef CONFIG_DM_MIRROR
#undef CONFIG_DM_ZERO
#undef CONFIG_DM_MULTIPATH

/*
 * Fusion MPT device support
 */
#undef CONFIG_FUSION

/*
 * IEEE 1394 (FireWire) support
 */
#undef CONFIG_IEEE1394

/*
 * I2O device support
 */
#undef CONFIG_I2O

/*
 * Networking support
 */
#define CONFIG_NET 1

/*
 * Networking options
 */
#define CONFIG_PACKET 1
#define CONFIG_PACKET_MMAP 1
#define CONFIG_UNIX 1
#undef CONFIG_NET_KEY
#define CONFIG_INET 1
#define CONFIG_IP_MULTICAST 1
#undef CONFIG_IP_ADVANCED_ROUTER
#define CONFIG_IP_PNP 1
#define CONFIG_IP_PNP_DHCP 1
#undef CONFIG_IP_PNP_BOOTP
#undef CONFIG_IP_PNP_RARP
#undef CONFIG_NET_IPIP
#undef CONFIG_NET_IPGRE
#undef CONFIG_IP_MROUTE
#undef CONFIG_ARPD
#undef CONFIG_SYN_COOKIES
#undef CONFIG_INET_AH
#undef CONFIG_INET_ESP
#undef CONFIG_INET_IPCOMP
#undef CONFIG_INET_TUNNEL
#define CONFIG_IP_TCPDIAG 1
#undef CONFIG_IP_TCPDIAG_IPV6
#undef CONFIG_IPV6
#undef CONFIG_NETFILTER

/*
 * SCTP Configuration (EXPERIMENTAL)
 */
#undef CONFIG_IP_SCTP
#undef CONFIG_ATM
#undef CONFIG_BRIDGE
#undef CONFIG_VLAN_8021Q
#undef CONFIG_DECNET
#undef CONFIG_LLC2
#undef CONFIG_IPX
#undef CONFIG_ATALK
#undef CONFIG_X25
#undef CONFIG_LAPB
#undef CONFIG_NET_DIVERT
#undef CONFIG_ECONET
#undef CONFIG_WAN_ROUTER

/*
 * QoS and/or fair queueing
 */
#undef CONFIG_NET_SCHED
#undef CONFIG_NET_CLS_ROUTE

/*
 * Network testing
 */
#undef CONFIG_NET_PKTGEN
#undef CONFIG_NETPOLL
#undef CONFIG_NET_POLL_CONTROLLER
#undef CONFIG_HAMRADIO
#undef CONFIG_IRDA
#undef CONFIG_BT
#define CONFIG_NETDEVICES 1
#undef CONFIG_DUMMY
#undef CONFIG_BONDING
#undef CONFIG_EQUALIZER
#undef CONFIG_TUN

/*
 * ARCnet devices
 */
#undef CONFIG_ARCNET

/*
 * Ethernet (10 or 100Mbit)
 */
#undef CONFIG_NET_ETHERNET

/*
 * Ethernet (1000 Mbit)
 */
#undef CONFIG_ACENIC
#undef CONFIG_DL2K
#undef CONFIG_E1000
#undef CONFIG_NS83820
#undef CONFIG_HAMACHI
#undef CONFIG_YELLOWFIN
#undef CONFIG_R8169
#undef CONFIG_SK98LIN
#undef CONFIG_TIGON3
#undef CONFIG_BNX2

/*
 * Ethernet (10000 Mbit)
 */
#undef CONFIG_IXGB
#undef CONFIG_S2IO

/*
 * Token Ring devices
 */
#undef CONFIG_TR

/*
 * Wireless LAN (non-hamradio)
 */
#define CONFIG_NET_RADIO 1

/*
 * Obsolete Wireless cards support (pre-802.11)
 */
#define CONFIG_STRIP 1

/*
 * Wireless 802.11b ISA/PCI cards support
 */
#undef CONFIG_HERMES
#undef CONFIG_ATMEL

/*
 * Prism GT/Duette 802.11(a/b/g) PCI/Cardbus support
 */
#undef CONFIG_PRISM54
#define CONFIG_NET_WIRELESS 1

/*
 * Wan interfaces
 */
#undef CONFIG_WAN
#undef CONFIG_FDDI
#undef CONFIG_HIPPI
#undef CONFIG_PPP
#undef CONFIG_SLIP
#undef CONFIG_NET_FC
#undef CONFIG_SHAPER
#undef CONFIG_NETCONSOLE

/*
 * ISDN subsystem
 */
#undef CONFIG_ISDN

/*
 * Telephony Support
 */
#undef CONFIG_PHONE

/*
 * Input device support
 */
#define CONFIG_INPUT 1

/*
 * Userland interfaces
 */
#define CONFIG_INPUT_MOUSEDEV 1
#define CONFIG_INPUT_MOUSEDEV_PSAUX 1
#define CONFIG_INPUT_MOUSEDEV_SCREEN_X 1024
#define CONFIG_INPUT_MOUSEDEV_SCREEN_Y 768
#undef CONFIG_INPUT_JOYDEV
#undef CONFIG_INPUT_TSDEV
#undef CONFIG_INPUT_EVDEV
#undef CONFIG_INPUT_EVBUG

/*
 * Input Device Drivers
 */
#undef CONFIG_INPUT_KEYBOARD
#undef CONFIG_INPUT_MOUSE
#undef CONFIG_INPUT_JOYSTICK
#undef CONFIG_INPUT_TOUCHSCREEN
#undef CONFIG_INPUT_MISC

/*
 * Hardware I/O ports
 */
#undef CONFIG_SERIO
#undef CONFIG_GAMEPORT

/*
 * Character devices
 */
#define CONFIG_VT 1
#define CONFIG_VT_CONSOLE 1
#define CONFIG_HW_CONSOLE 1
#undef CONFIG_SERIAL_NONSTANDARD

/*
 * Serial drivers
 */
#define CONFIG_SERIAL_8250 1
#define CONFIG_SERIAL_8250_CONSOLE 1
#define CONFIG_SERIAL_8250_NR_UARTS 4
#undef CONFIG_SERIAL_8250_EXTENDED

/*
 * Non-8250 serial port support
 */
#define CONFIG_SERIAL_CORE 1
#define CONFIG_SERIAL_CORE_CONSOLE 1
#undef CONFIG_SERIAL_JSM
#define CONFIG_UNIX98_PTYS 1
#define CONFIG_LEGACY_PTYS 1
#define CONFIG_LEGACY_PTY_COUNT 16

/*
 * IPMI
 */
#undef CONFIG_IPMI_HANDLER

/*
 * Watchdog Cards
 */
#undef CONFIG_WATCHDOG
#undef CONFIG_NVRAM
#undef CONFIG_RTC
#undef CONFIG_DTLK
#undef CONFIG_R3964
#undef CONFIG_APPLICOM

/*
 * Ftape, the floppy tape device driver
 */
#undef CONFIG_DRM
#undef CONFIG_RAW_DRIVER

/*
 * TPM devices
 */
#undef CONFIG_TCG_TPM

/*
 * I2C support
 */
#define CONFIG_I2C 1
#undef CONFIG_I2C_CHARDEV

/*
 * I2C Algorithms
 */
#undef CONFIG_I2C_ALGOBIT
#undef CONFIG_I2C_ALGOPCF
#undef CONFIG_I2C_ALGOPCA

/*
 * I2C Hardware Bus support
 */
#undef CONFIG_I2C_ALI1535
#undef CONFIG_I2C_ALI1563
#undef CONFIG_I2C_ALI15X3
#undef CONFIG_I2C_AMD756
#undef CONFIG_I2C_AMD8111
#undef CONFIG_I2C_I801
#undef CONFIG_I2C_I810
#undef CONFIG_I2C_PIIX4
#undef CONFIG_I2C_ISA
#undef CONFIG_I2C_NFORCE2
#undef CONFIG_I2C_PARPORT_LIGHT
#undef CONFIG_I2C_PROSAVAGE
#undef CONFIG_I2C_SAVAGE4
#undef CONFIG_SCx200_ACB
#undef CONFIG_I2C_SIS5595
#undef CONFIG_I2C_SIS630
#undef CONFIG_I2C_SIS96X
#undef CONFIG_I2C_STUB
#undef CONFIG_I2C_VIA
#undef CONFIG_I2C_VIAPRO
#undef CONFIG_I2C_VOODOO3
#undef CONFIG_I2C_PCA_ISA

/*
 * Hardware Sensors Chip support
 */
#undef CONFIG_I2C_SENSOR
#undef CONFIG_SENSORS_ADM1021
#undef CONFIG_SENSORS_ADM1025
#undef CONFIG_SENSORS_ADM1026
#undef CONFIG_SENSORS_ADM1031
#undef CONFIG_SENSORS_ASB100
#undef CONFIG_SENSORS_DS1621
#undef CONFIG_SENSORS_FSCHER
#undef CONFIG_SENSORS_FSCPOS
#undef CONFIG_SENSORS_GL518SM
#undef CONFIG_SENSORS_GL520SM
#undef CONFIG_SENSORS_IT87
#undef CONFIG_SENSORS_LM63
#undef CONFIG_SENSORS_LM75
#undef CONFIG_SENSORS_LM77
#undef CONFIG_SENSORS_LM78
#undef CONFIG_SENSORS_LM80
#undef CONFIG_SENSORS_LM83
#undef CONFIG_SENSORS_LM85
#undef CONFIG_SENSORS_LM87
#undef CONFIG_SENSORS_LM90
#undef CONFIG_SENSORS_LM92
#undef CONFIG_SENSORS_MAX1619
#undef CONFIG_SENSORS_PC87360
#undef CONFIG_SENSORS_SMSC47B397
#undef CONFIG_SENSORS_SIS5595
#undef CONFIG_SENSORS_SMSC47M1
#undef CONFIG_SENSORS_VIA686A
#undef CONFIG_SENSORS_W83781D
#undef CONFIG_SENSORS_W83L785TS
#undef CONFIG_SENSORS_W83627HF

/*
 * Other I2C Chip support
 */
#undef CONFIG_SENSORS_DS1337
#undef CONFIG_SENSORS_EEPROM
#undef CONFIG_SENSORS_PCF8574
#undef CONFIG_SENSORS_PCF8591
#undef CONFIG_SENSORS_RTC8564
#undef CONFIG_I2C_DEBUG_CORE
#undef CONFIG_I2C_DEBUG_ALGO
#undef CONFIG_I2C_DEBUG_BUS
#undef CONFIG_I2C_DEBUG_CHIP

/*
 * Misc devices
 */

/*
 * Multimedia devices
 */
#undef CONFIG_VIDEO_DEV

/*
 * Digital Video Broadcasting Devices
 */
#undef CONFIG_DVB

/*
 * Graphics support
 */
#undef CONFIG_FB

/*
 * Console display driver support
 */
#undef CONFIG_VGA_CONSOLE
#define CONFIG_DUMMY_CONSOLE 1

/*
 * Sound
 */
#undef CONFIG_SOUND

/*
 * USB support
 */
#define CONFIG_USB_ARCH_HAS_HCD 1
#define CONFIG_USB_ARCH_HAS_OHCI 1
#define CONFIG_USB 1
#undef CONFIG_USB_DEBUG

/*
 * Miscellaneous USB options
 */
#define CONFIG_USB_DEVICEFS 1
#undef CONFIG_USB_BANDWIDTH
#undef CONFIG_USB_DYNAMIC_MINORS
#undef CONFIG_USB_OTG

/*
 * USB Host Controller Drivers
 */
#define CONFIG_USB_EHCI_HCD 1
#define CONFIG_USB_EHCI_SPLIT_ISO 1
#define CONFIG_USB_EHCI_ROOT_HUB_TT 1
#define CONFIG_USB_OHCI_HCD 1
#undef CONFIG_USB_OHCI_BIG_ENDIAN
#define CONFIG_USB_OHCI_LITTLE_ENDIAN 1
#undef CONFIG_USB_UHCI_HCD
#undef CONFIG_USB_SL811_HCD

/*
 * USB Device Class drivers
 */
#undef CONFIG_USB_BLUETOOTH_TTY
#undef CONFIG_USB_ACM
#define CONFIG_USB_PRINTER_MODULE 1

/*
 * NOTE: USB_STORAGE enables SCSI, and 'SCSI disk support' may also be needed; see USB_STORAGE Help for more information
 */
#define CONFIG_USB_STORAGE 1
#undef CONFIG_USB_STORAGE_DEBUG
#define CONFIG_USB_STORAGE_DATAFAB 1
#define CONFIG_USB_STORAGE_FREECOM 1
#define CONFIG_USB_STORAGE_DPCM 1
#undef CONFIG_USB_STORAGE_USBAT
#define CONFIG_USB_STORAGE_SDDR09 1
#define CONFIG_USB_STORAGE_SDDR55 1
#define CONFIG_USB_STORAGE_JUMPSHOT 1

/*
 * USB Input Devices
 */
#define CONFIG_USB_HID 1
#undef CONFIG_USB_HIDINPUT
#define CONFIG_USB_HIDDEV 1
#undef CONFIG_USB_AIPTEK
#undef CONFIG_USB_WACOM
#undef CONFIG_USB_KBTAB
#undef CONFIG_USB_POWERMATE
#undef CONFIG_USB_MTOUCH
#undef CONFIG_USB_EGALAX
#undef CONFIG_USB_XPAD
#undef CONFIG_USB_ATI_REMOTE

/*
 * USB Imaging devices
 */
#undef CONFIG_USB_MDC800
#undef CONFIG_USB_MICROTEK

/*
 * USB Multimedia devices
 */
#undef CONFIG_USB_DABUSB

/*
 * Video4Linux support is needed for USB Multimedia device support
 */

/*
 * USB Network Adapters
 */
#undef CONFIG_USB_CATC
#undef CONFIG_USB_KAWETH
#undef CONFIG_USB_PEGASUS
#undef CONFIG_USB_RTL8150
#undef CONFIG_USB_USBNET
#undef CONFIG_USB_ZD1201
#undef CONFIG_USB_MON

/*
 * USB port drivers
 */

/*
 * USB Serial Converter support
 */
#undef CONFIG_USB_SERIAL

/*
 * USB Miscellaneous drivers
 */
#undef CONFIG_USB_EMI62
#undef CONFIG_USB_EMI26
#undef CONFIG_USB_AUERSWALD
#undef CONFIG_USB_RIO500
#undef CONFIG_USB_LEGOTOWER
#undef CONFIG_USB_LCD
#undef CONFIG_USB_LED
#undef CONFIG_USB_CYTHERM
#undef CONFIG_USB_PHIDGETKIT
#undef CONFIG_USB_PHIDGETSERVO
#undef CONFIG_USB_IDMOUSE
#undef CONFIG_USB_SISUSBVGA
#undef CONFIG_USB_TEST

/*
 * USB ATM/DSL drivers
 */

/*
 * USB Gadget Support
 */
#undef CONFIG_USB_GADGET

/*
 * MMC/SD Card support
 */
#undef CONFIG_MMC
#define CONFIG_WIX_EVENT 1
#define CONFIG_WIX_DEV 1

/*
 * File systems
 */
#define CONFIG_EXT2_FS 1
#undef CONFIG_EXT2_FS_XATTR
#define CONFIG_EXT3_FS 1
#undef CONFIG_EXT3_FS_XATTR
#define CONFIG_JBD 1
#undef CONFIG_JBD_DEBUG
#undef CONFIG_REISERFS_FS
#undef CONFIG_JFS_FS

/*
 * XFS support
 */
#define CONFIG_XFS_FS 1
#define CONFIG_XFS_EXPORT 1
#undef CONFIG_XFS_RT
#define CONFIG_XFS_QUOTA 1
#undef CONFIG_XFS_SECURITY
#undef CONFIG_XFS_POSIX_ACL
#undef CONFIG_MINIX_FS
#undef CONFIG_ROMFS_FS
#define CONFIG_INOTIFY 1
#define CONFIG_QUOTA 1
#undef CONFIG_QFMT_V1
#define CONFIG_QFMT_V2 1
#define CONFIG_QUOTACTL 1
#define CONFIG_DNOTIFY 1
#undef CONFIG_AUTOFS_FS
#undef CONFIG_AUTOFS4_FS

/*
 * CD-ROM/DVD Filesystems
 */
#undef CONFIG_ISO9660_FS
#undef CONFIG_UDF_FS

/*
 * DOS/FAT/NT Filesystems
 */
#define CONFIG_FAT_FS 1
#define CONFIG_MSDOS_FS 1
#define CONFIG_VFAT_FS 1
#define CONFIG_FAT_DEFAULT_CODEPAGE 437
#define CONFIG_FAT_DEFAULT_IOCHARSET "iso8859-1"
#undef CONFIG_NTFS_FS

/*
 * Pseudo filesystems
 */
#define CONFIG_PROC_FS 1
#define CONFIG_SYSFS 1
#undef CONFIG_DEVFS_FS
#undef CONFIG_DEVPTS_FS_XATTR
#define CONFIG_TMPFS 1
#undef CONFIG_TMPFS_XATTR
#undef CONFIG_HUGETLB_PAGE
#define CONFIG_RAMFS 1

/*
 * Miscellaneous filesystems
 */
#undef CONFIG_ADFS_FS
#undef CONFIG_AFFS_FS
#undef CONFIG_HFS_FS
#undef CONFIG_HFSPLUS_FS
#undef CONFIG_BEFS_FS
#undef CONFIG_BFS_FS
#undef CONFIG_EFS_FS
#undef CONFIG_JFFS_FS
#define CONFIG_JFFS2_FS 1
#define CONFIG_JFFS2_FS_DEBUG 0
#undef CONFIG_JFFS2_FS_NAND
#undef CONFIG_JFFS2_FS_NOR_ECC
#undef CONFIG_JFFS2_COMPRESSION_OPTIONS
#define CONFIG_JFFS2_ZLIB 1
#define CONFIG_JFFS2_RTIME 1
#undef CONFIG_JFFS2_RUBIN
#define CONFIG_CRAMFS 1
#undef CONFIG_VXFS_FS
#undef CONFIG_HPFS_FS
#undef CONFIG_QNX4FS_FS
#undef CONFIG_SYSV_FS
#undef CONFIG_UFS_FS

/*
 * Network File Systems
 */
#define CONFIG_NFS_FS 1
#define CONFIG_NFS_V3 1
#undef CONFIG_NFS_V4
#undef CONFIG_NFS_DIRECTIO
#define CONFIG_NFSD 1
#define CONFIG_NFSD_V3 1
#undef CONFIG_NFSD_V4
#define CONFIG_NFSD_TCP 1
#define CONFIG_ROOT_NFS 1
#define CONFIG_LOCKD 1
#define CONFIG_LOCKD_V4 1
#define CONFIG_EXPORTFS 1
#define CONFIG_SUNRPC 1
#undef CONFIG_RPCSEC_GSS_KRB5
#undef CONFIG_RPCSEC_GSS_SPKM3
#undef CONFIG_SMB_FS
#undef CONFIG_CIFS
#undef CONFIG_NCP_FS
#undef CONFIG_CODA_FS
#undef CONFIG_AFS_FS

/*
 * Partition Types
 */
#define CONFIG_PARTITION_ADVANCED 1
#undef CONFIG_ACORN_PARTITION
#undef CONFIG_OSF_PARTITION
#undef CONFIG_AMIGA_PARTITION
#undef CONFIG_ATARI_PARTITION
#define CONFIG_MAC_PARTITION 1
#define CONFIG_MSDOS_PARTITION 1
#undef CONFIG_BSD_DISKLABEL
#undef CONFIG_MINIX_SUBPARTITION
#undef CONFIG_SOLARIS_X86_PARTITION
#undef CONFIG_UNIXWARE_DISKLABEL
#define CONFIG_LDM_PARTITION 1
#undef CONFIG_LDM_DEBUG
#undef CONFIG_SGI_PARTITION
#undef CONFIG_ULTRIX_PARTITION
#undef CONFIG_SUN_PARTITION
#undef CONFIG_EFI_PARTITION

/*
 * Native Language Support
 */
#define CONFIG_NLS 1
#define CONFIG_NLS_DEFAULT "iso8859-1"
#define CONFIG_NLS_CODEPAGE_437 1
#undef CONFIG_NLS_CODEPAGE_737
#undef CONFIG_NLS_CODEPAGE_775
#undef CONFIG_NLS_CODEPAGE_850
#undef CONFIG_NLS_CODEPAGE_852
#undef CONFIG_NLS_CODEPAGE_855
#undef CONFIG_NLS_CODEPAGE_857
#undef CONFIG_NLS_CODEPAGE_860
#undef CONFIG_NLS_CODEPAGE_861
#undef CONFIG_NLS_CODEPAGE_862
#undef CONFIG_NLS_CODEPAGE_863
#undef CONFIG_NLS_CODEPAGE_864
#undef CONFIG_NLS_CODEPAGE_865
#undef CONFIG_NLS_CODEPAGE_866
#undef CONFIG_NLS_CODEPAGE_869
#undef CONFIG_NLS_CODEPAGE_936
#undef CONFIG_NLS_CODEPAGE_950
#undef CONFIG_NLS_CODEPAGE_932
#undef CONFIG_NLS_CODEPAGE_949
#undef CONFIG_NLS_CODEPAGE_874
#undef CONFIG_NLS_ISO8859_8
#undef CONFIG_NLS_CODEPAGE_1250
#undef CONFIG_NLS_CODEPAGE_1251
#undef CONFIG_NLS_ASCII
#define CONFIG_NLS_ISO8859_1 1
#undef CONFIG_NLS_ISO8859_2
#undef CONFIG_NLS_ISO8859_3
#undef CONFIG_NLS_ISO8859_4
#undef CONFIG_NLS_ISO8859_5
#undef CONFIG_NLS_ISO8859_6
#undef CONFIG_NLS_ISO8859_7
#undef CONFIG_NLS_ISO8859_9
#undef CONFIG_NLS_ISO8859_13
#undef CONFIG_NLS_ISO8859_14
#undef CONFIG_NLS_ISO8859_15
#undef CONFIG_NLS_KOI8_R
#undef CONFIG_NLS_KOI8_U
#undef CONFIG_NLS_UTF8

/*
 * Profiling support
 */
#undef CONFIG_PROFILING

/*
 * Kernel hacking
 */
#undef CONFIG_PRINTK_TIME
#undef CONFIG_DEBUG_KERNEL
#define CONFIG_LOG_BUF_SHIFT 14
#define CONFIG_DEBUG_BUGVERBOSE 1
#define CONFIG_FRAME_POINTER 1
#undef CONFIG_WANT_EXTRA_DEBUG_INFORMATION
#define CONFIG_DEBUG_USER 1

/*
 * Security options
 */
#undef CONFIG_KEYS
#define CONFIG_SECURITY 1
#undef CONFIG_SECURITY_NETWORK
#undef CONFIG_SECURITY_CAPABILITIES
#undef CONFIG_SECURITY_ROOTPLUG
#undef CONFIG_SECURITY_SECLVL
#define CONFIG_SECURITY_TRUSTEES 1
#undef CONFIG_SECURITY_SELINUX

/*
 * Cryptographic options
 */
#define CONFIG_CRYPTO 1
#undef CONFIG_CRYPTO_HMAC
#undef CONFIG_CRYPTO_NULL
#undef CONFIG_CRYPTO_MD4
#undef CONFIG_CRYPTO_MD5
#undef CONFIG_CRYPTO_SHA1
#undef CONFIG_CRYPTO_SHA256
#undef CONFIG_CRYPTO_SHA512
#undef CONFIG_CRYPTO_WP512
#undef CONFIG_CRYPTO_TGR192
#undef CONFIG_CRYPTO_DES
#undef CONFIG_CRYPTO_BLOWFISH
#undef CONFIG_CRYPTO_TWOFISH
#undef CONFIG_CRYPTO_SERPENT
#undef CONFIG_CRYPTO_AES
#undef CONFIG_CRYPTO_CAST5
#undef CONFIG_CRYPTO_CAST6
#undef CONFIG_CRYPTO_TEA
#undef CONFIG_CRYPTO_ARC4
#undef CONFIG_CRYPTO_KHAZAD
#undef CONFIG_CRYPTO_ANUBIS
#undef CONFIG_CRYPTO_DEFLATE
#undef CONFIG_CRYPTO_MICHAEL_MIC
#undef CONFIG_CRYPTO_CRC32C
#undef CONFIG_CRYPTO_TEST

/*
 * OCF Configuration
 */
#undef CONFIG_OCF_OCF

/*
 * Hardware crypto devices
 */

/*
 * Library routines
 */
#undef CONFIG_CRC_CCITT
#define CONFIG_CRC32 1
#define CONFIG_LIBCRC32C 1
#define CONFIG_ZLIB_INFLATE 1
#define CONFIG_ZLIB_DEFLATE 1
