cmd_drivers/md/raid6sse2.o := arm-none-linux-gnueabi-gcc -Wp,-MD,drivers/md/.raid6sse2.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -marm -ffunction-sections                         -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f  -msoft-float -Uarm -fno-omit-frame-pointer -fno-optimize-sibling-calls  -fno-stack-protector -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(raid6sse2)"  -D"KBUILD_MODNAME=KBUILD_STR(raid456)" -c -o drivers/md/raid6sse2.o drivers/md/raid6sse2.c

deps_drivers/md/raid6sse2.o := \
  drivers/md/raid6sse2.c \

drivers/md/raid6sse2.o: $(deps_drivers/md/raid6sse2.o)

$(deps_drivers/md/raid6sse2.o):
