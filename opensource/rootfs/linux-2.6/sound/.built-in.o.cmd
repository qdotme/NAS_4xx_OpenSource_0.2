cmd_sound/built-in.o :=  arm-none-linux-gnueabi-ld -EL   -r -o sound/built-in.o sound/soundcore.o sound/core/built-in.o sound/i2c/built-in.o sound/drivers/built-in.o sound/isa/built-in.o sound/pci/built-in.o sound/ppc/built-in.o sound/arm/built-in.o sound/synth/built-in.o sound/usb/built-in.o sound/sparc/built-in.o sound/parisc/built-in.o sound/pcmcia/built-in.o sound/mips/built-in.o sound/soc/built-in.o sound/last.o
