cmd_drivers/scsi/thor/lib/common/com_nvram.o := arm-none-linux-gnueabi-gcc -Wp,-MD,drivers/scsi/thor/lib/common/.com_nvram.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -marm -ffunction-sections                         -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f  -msoft-float -Uarm -fno-omit-frame-pointer -fno-optimize-sibling-calls  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/include    -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/core -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/raid       -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/. -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/linux  -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/math -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/cache      -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/include/generic -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/include/icommon -D__MV_LINUX__ -I/include -I/include/scsi -I/drivers/scsi -I/include -I/include/scsi -I/drivers/scsi -D_32_LEGACY_ -include /opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/mv_config.h -I/opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/core/thor -DPRODUCTNAME_THOR -D__LEGACY_OSSW__=1   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(com_nvram)"  -D"KBUILD_MODNAME=KBUILD_STR(mv61xx)" -c -o drivers/scsi/thor/lib/common/com_nvram.o /opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/lib/common/com_nvram.c

deps_drivers/scsi/thor/lib/common/com_nvram.o := \
  /opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/lib/common/com_nvram.c \
  /opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/mv_config.h \
  /opt/devel/proto/marvell/build-eabi/linux-feroceon_4_2_2_KW/drivers/scsi/thor/mv_thor.h \

drivers/scsi/thor/lib/common/com_nvram.o: $(deps_drivers/scsi/thor/lib/common/com_nvram.o)

$(deps_drivers/scsi/thor/lib/common/com_nvram.o):