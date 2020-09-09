cmd_arch/arm/lib/memcpy.o := arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/lib/.memcpy.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f -msoft-float    -c -o arch/arm/lib/memcpy.o arch/arm/lib/memcpy.S

deps_arch/arm/lib/memcpy.o := \
  arch/arm/lib/memcpy.S \
    $(wildcard include/config/mv/xormemcopy.h) \
    $(wildcard include/config/mv/idma/memcopy.h) \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm/assembler.h \
    $(wildcard include/config/use/dsp.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/smp.h) \
  arch/arm/lib/copy_template.S \

arch/arm/lib/memcpy.o: $(deps_arch/arm/lib/memcpy.o)

$(deps_arch/arm/lib/memcpy.o):
