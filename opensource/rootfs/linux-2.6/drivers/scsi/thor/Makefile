#
#
# Note: This Makefile is for 2.6 kernel, non-raid only, at present.
#
# V0.0.0.2 Ver.Make for kbuild

# default to build for the running kernel
KERNEL_SRC    :=   $(srctree)/$(src)


src=$(KERNEL_SRC)
include $(src)/mv_conf.mk



thor_objs_$(SUPPORT_THOR)   := core/thor/core_exp.o core/thor/core_init.o\
		   core/thor/core_api.o core/thor/core_xor.o\
		   core/thor/scsi2sata.o core/thor/core_swxor.o core/thor/core_sat.o

CORE_OBJS       :=  $(odin_objs_y) $(thor_objs_y) 

COMM_OBJS       := lib/common/com_util.o lib/common/com_u64.o             \
		   lib/common/com_scsi.o lib/common/com_tag.o             \
		   lib/common/com_sgd.o  lib/common/com_nvram.o

OSDEP_OBJS      := linux/linux_main.o    linux/hba_exp.o          \
		   linux/hba_mod.o       linux/hba_timer.o        \
		   linux/oss_wrapper.o   linux/linux_iface.o      \
	           linux/res_mgmt.o 	

LIBMV_OBJS	:= $(CORE_OBJS) $(COMM_OBJS) 

HBA_OBJS        := $(OSDEP_OBJS) $(LIBMV_OBJS)

INCLUDE_DIR     = -I$(KERNEL_BLD_DIR)/include                          \
		  -I$(KERNEL_BLD_DIR)/include/scsi                     \
		  -I$(KERNEL_BLD_DIR)/drivers/scsi                     \
	 	  -I$(KERNEL_SRC_DIR)/include                          \
	 	  -I$(KERNEL_SRC_DIR)/include/scsi                     \
	 	  -I$(KERNEL_SRC_DIR)/drivers/scsi



ifeq ($(SUPPORT_THOR), y)
obj-$(CONFIG_SCSI_MV_THOR)      :=   mv61xx.o
mv61xx-objs   :=   $(HBA_OBJS) 
endif

clean-files   +=   Modules.symvers
clean-files   +=   Module.symvers
EXTRA_CFLAGS  :=   -I$(src)/include    -I$(src)/core            \
		   -I$(src)/raid       -I$(src)/.               \
		   -I$(src)/linux  -I$(src)/math            \
                   -I$(src)/cache      -I$(src)/include/generic \
		   -I$(src)/include/icommon

EXTRA_CFLAGS  +=   -D__MV_LINUX__ $(INCLUDE_DIR)

EXTRA_CFLAGS  +=   -D_32_LEGACY_

EXTRA_CFLAGS  +=   -include $(src)/mv_config.h


ifeq ($(SUPPORT_THOR), y)
EXTRA_CFLAGS  +=   -I$(src)/core/thor -DPRODUCTNAME_THOR
endif


EXTRA_CFLAGS  +=   -D__LEGACY_OSSW__=1

