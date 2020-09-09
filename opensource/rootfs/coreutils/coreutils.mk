#############################################################
#
#  coreutils
#
#############################################################
COREUTILS_VER:=6.12
COREUTILS_TARGET_DIR:=bin
COREUTILS_SOURCE:=$(PACKAGE_DIR)/coreutils/coreutils-$(COREUTILS_VER)
COREUTILS_BUILD_DIR:=$(BUILD_DIR)/coreutils-$(COREUTILS_VER)


### STEP 1; Copy source code to BUILD directory; and apply patches.
$(COREUTILS_BUILD_DIR)/.copied:
	cp -a $(COREUTILS_SOURCE) $(COREUTILS_BUILD_DIR)
	$(PATCH) $(COREUTILS_BUILD_DIR) $(PACKAGE_DIR)/coreutils coreutils\*-$(COREUTILS_VER).patch
	touch $(COREUTILS_BUILD_DIR)/.copied

### STEP 2; Configure
$(COREUTILS_BUILD_DIR)/.configured: $(COREUTILS_BUILD_DIR)/.copied
	(cd $(COREUTILS_BUILD_DIR); rm -f config.cache; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS=-L$(LIBS_DIR)\
		CPPFLAGS="-I$(INCLUDE_DIR) -I$(COREUTILS_BUILD_DIR)/lib"\
		fu_cv_sys_stat_statfs2_bsize=yes \
		ac_cv_type_size_t=no \
		./configure \
		--target=$(GNU_TARGET_NAME) \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--prefix=/usr \
		--bindir=/usr/bin \
		--sbindir=/usr/sbin \
		--disable-nls \
	);
	touch $(COREUTILS_BUILD_DIR)/.configured

### STEP 3; Make and strip binaries
$(COREUTILS_BUILD_DIR)/src/cp: $(COREUTILS_BUILD_DIR)/.configured
	$(MAKE) CC=$(TARGET_CC) -C $(COREUTILS_BUILD_DIR)

$(TARGET_DIR)/$(COREUTILS_TARGET_DIR)/cp_pro: $(COREUTILS_BUILD_DIR)/src/cp

### STEP 4; Install binaries and related files
coreutils: $(TARGET_DIR)/$(COREUTILS_TARGET_DIR)/cp_pro
	$(INSTALL) -m 755 $(COREUTILS_BUILD_DIR)/src/cp $(TARGET_DIR)/$(COREUTILS_TARGET_DIR)/cp_pro
	$(STRIP) $(TARGET_DIR)/$(COREUTILS_TARGET_DIR)/cp_pro

coreutils-clean:
	$(MAKE) -C $(COREUTILS_BUILD_DIR) clean

coreutils-dirclean:
	rm -rf $(COREUTILS_BUILD_DIR)

#############################################################
# Toplevel Makefile options
#############################################################
ifeq ($(strip $(MAL_PACKAGE_COREUTILS)),y)
TARGETS+=coreutils
endif


