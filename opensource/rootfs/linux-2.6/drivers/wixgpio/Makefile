#
# Makefile for Wistron SxM board
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#
ifeq ($(CONFIG_ARCH_FEROCEON_KW),y)
        include $(TOPDIR)/arch/arm/mach-feroceon-kw/config/mvRules.mk
endif

#obj-$(CONFIG_WIX_GPIO)	+= wixgpio-dummy.o
obj-$(CONFIG_WIX_GPIO)	+= wixgpio.o 

