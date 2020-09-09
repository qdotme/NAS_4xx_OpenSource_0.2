cmd_arch/arm/lib/strnlen_user.o := arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/lib/.strnlen_user.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f -msoft-float    -c -o arch/arm/lib/strnlen_user.o arch/arm/lib/strnlen_user.S

deps_arch/arm/lib/strnlen_user.o := \
  arch/arm/lib/strnlen_user.S \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm/assembler.h \
    $(wildcard include/config/use/dsp.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/smp.h) \
  include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \

arch/arm/lib/strnlen_user.o: $(deps_arch/arm/lib/strnlen_user.o)

$(deps_arch/arm/lib/strnlen_user.o):
