cmd_arch/arm/mm/proc-arm926.o := arm-none-linux-gnueabi-gcc -Wp,-MD,arch/arm/mm/.proc-arm926.o.d  -nostdinc -isystem /opt/devel/proto/marvell/build-eabi/kernel-toolchain/bin/../lib/gcc/arm-none-linux-gnueabi/4.2.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5t -mtune=marvell-f -msoft-float    -c -o arch/arm/mm/proc-arm926.o arch/arm/mm/proc-arm926.S

deps_arch/arm/mm/proc-arm926.o := \
  arch/arm/mm/proc-arm926.S \
    $(wildcard include/config/cpu/arm926/cpu/idle.h) \
    $(wildcard include/config/mv/dcache/size.h) \
    $(wildcard include/config/arch/feroceon/orion.h) \
    $(wildcard include/config/arch/feroceon/kw.h) \
    $(wildcard include/config/arch/feroceon/mv78xx0.h) \
    $(wildcard include/config/reg.h) \
    $(wildcard include/config/l2/cache/enable.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/arch/feroceon.h) \
    $(wildcard include/config/cpu/dcache/writethrough.h) \
    $(wildcard include/config/cpu/l2/dcache/writethrough.h) \
    $(wildcard include/config/cpu/cache/round/robin.h) \
    $(wildcard include/config/mv88f6183.h) \
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
  include/asm/assembler.h \
    $(wildcard include/config/use/dsp.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/smp.h) \
  include/asm/elf.h \
  include/asm/pgtable-hwdef.h \
  include/asm/pgtable.h \
  include/asm-generic/4level-fixup.h \
  include/asm/proc-fns.h \
    $(wildcard include/config/cpu/32.h) \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm7tdmi.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm740t.h) \
    $(wildcard include/config/cpu/arm9tdmi.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/arm940t.h) \
    $(wildcard include/config/cpu/arm946e.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/v6.h) \
    $(wildcard include/config/cpu/v7.h) \
  include/asm/memory.h \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/discontigmem.h) \
  include/asm/arch/memory.h \
  include/asm/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/out/of/line/pfn/to/page.h) \
  include/asm/arch/vmalloc.h \
  include/asm/page.h \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/aeabi.h) \
  include/asm-generic/page.h \
  arch/arm/mm/proc-macros.S \
  include/asm/asm-offsets.h \
  include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  include/asm/arch/kw_macro.h \
  arch/arm/mm/../mach-feroceon-kw/kw_family/ctrlEnv/sys/mvCpuIfRegs.h \
    $(wildcard include/config/default/mask.h) \
    $(wildcard include/config/default.h) \
  arch/arm/mm/../mach-feroceon-kw/config/mvSysHwConfig.h \
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

arch/arm/mm/proc-arm926.o: $(deps_arch/arm/mm/proc-arm926.o)

$(deps_arch/arm/mm/proc-arm926.o):
