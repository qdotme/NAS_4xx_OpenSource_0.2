cmd_arch/arm/mm/copypage-v4wb.o := arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/mm/.copypage-v4wb.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f -msoft-float    -c -o arch/arm/mm/copypage-v4wb.o arch/arm/mm/copypage-v4wb.S

deps_arch/arm/mm/copypage-v4wb.o := \
  arch/arm/mm/copypage-v4wb.S \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
  include/asm/asm-offsets.h \

arch/arm/mm/copypage-v4wb.o: $(deps_arch/arm/mm/copypage-v4wb.o)

$(deps_arch/arm/mm/copypage-v4wb.o):
