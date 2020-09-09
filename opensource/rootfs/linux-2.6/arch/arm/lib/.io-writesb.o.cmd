cmd_arch/arm/lib/io-writesb.o := arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/lib/.io-writesb.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f -msoft-float    -c -o arch/arm/lib/io-writesb.o arch/arm/lib/io-writesb.S

deps_arch/arm/lib/io-writesb.o := \
  arch/arm/lib/io-writesb.S \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm/assembler.h \
    $(wildcard include/config/use/dsp.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/smp.h) \

arch/arm/lib/io-writesb.o: $(deps_arch/arm/lib/io-writesb.o)

$(deps_arch/arm/lib/io-writesb.o):
