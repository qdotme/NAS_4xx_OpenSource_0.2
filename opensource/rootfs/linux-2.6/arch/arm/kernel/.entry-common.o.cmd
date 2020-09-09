cmd_arch/arm/kernel/entry-common.o := arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/kernel/.entry-common.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f -msoft-float    -c -o arch/arm/kernel/entry-common.o arch/arm/kernel/entry-common.S

deps_arch/arm/kernel/entry-common.o := \
  arch/arm/kernel/entry-common.S \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/oabi/compat.h) \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/aeabi.h) \
    $(wildcard include/config/alignment/trap.h) \
  include/asm/unistd.h \
    $(wildcard include/config/arch/wistron.h) \
  include/asm/arch/entry-macro.S \
    $(wildcard include/config/cpu/big/endian.h) \
  include/asm/hardware.h \
  include/asm/arch/hardware.h \
  include/asm/sizes.h \
  include/../arch/arm/mach-feroceon-kw/config/mvSysHwConfig.h \
    $(wildcard include/config/marvell.h) \
    $(wildcard include/config/mv/include/pex.h) \
    $(wildcard include/config/mv/include/twsi.h) \
    $(wildcard include/config/mv/include/cesa.h) \
    $(wildcard include/config/mv/include/gig/eth.h) \
    $(wildcard include/config/mv/include/integ/sata.h) \
    $(wildcard include/config/mv/include/usb.h) \
    $(wildcard include/config/mv/include/nand.h) \
    $(wildcard include/config/mv/include/tdm.h) \
    $(wildcard include/config/mv/include/xor.h) \
    $(wildcard include/config/mv/include/uart.h) \
    $(wildcard include/config/mv/include/spi.h) \
    $(wildcard include/config/mv/include/sflash/mtd.h) \
    $(wildcard include/config/mv/include/audio.h) \
    $(wildcard include/config/mv/include/ts.h) \
    $(wildcard include/config/mv/include/sdio.h) \
    $(wildcard include/config/mv/nand/boot.h) \
    $(wildcard include/config/mv/nand.h) \
    $(wildcard include/config/mv/spi/boot.h) \
    $(wildcard include/config/mv/nfp/stats.h) \
    $(wildcard include/config/str.h) \
    $(wildcard include/config/mv/eth/rx/q/num.h) \
    $(wildcard include/config/mv/eth/tx/q/num.h) \
    $(wildcard include/config/mv/tdm/linear/mode.h) \
    $(wildcard include/config/mv/tdm/ulaw/mode.h) \
    $(wildcard include/config/rw/wa/target.h) \
    $(wildcard include/config/rw/wa/use/original/win/values.h) \
    $(wildcard include/config/rw/wa/base.h) \
    $(wildcard include/config/rw/wa/size.h) \
  include/asm/arch/irqs.h \
  arch/arm/kernel/entry-header.S \
    $(wildcard include/config/frame/pointer.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/acpi/hotplug/memory.h) \
  include/linux/compiler.h \
    $(wildcard include/config/enable/must/check.h) \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm/assembler.h \
    $(wildcard include/config/use/dsp.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/smp.h) \
  include/asm/asm-offsets.h \
  include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  arch/arm/kernel/calls.S \

arch/arm/kernel/entry-common.o: $(deps_arch/arm/kernel/entry-common.o)

$(deps_arch/arm/kernel/entry-common.o):
