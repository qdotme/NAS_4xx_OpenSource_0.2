#############################################################
#
#  ethtool
#
#############################################################
ETHTOOL_VER:=6
ETHTOOL_TARGET_DIR:=usr/sbin
ETHTOOL_SOURCE:=$(PACKAGE_DIR)/ethtool/ethtool-$(ETHTOOL_VER)
ETHTOOL_BUILD_DIR:=$(BUILD_DIR)/ethtool-$(ETHTOOL_VER)

$(ETHTOOL_BUILD_DIR)/.copied:
	cp -a $(ETHTOOL_SOURCE) $(ETHTOOL_BUILD_DIR)
	#$(PATCH) $(ETHTOOL_BUILD_DIR) $(PACKAGE_DIR)/ethtool ethtool-$(ETHTOOL_VER)\*.patch
	touch $(ETHTOOL_BUILD_DIR)/.copied

$(ETHTOOL_BUILD_DIR)/.configured: $(ETHTOOL_BUILD_DIR)/.copied
	(cd $(ETHTOOL_BUILD_DIR); rm -f config.cache; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
    		LDFLAGS="$(LDFLAGS)"\
		CPPFLAGS="$(CPPFLAGS)"\
		./configure \
		--target=$(GNU_TARGET_NAME) \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--prefix=/usr \
		--bindir=/usr/bin \
		--sbindir=/usr/sbin \
	);
	touch $(ETHTOOL_BUILD_DIR)/.configured

$(ETHTOOL_BUILD_DIR)/ethtool: $(ETHTOOL_BUILD_DIR)/.configured
	$(MAKE) CPPFLAGS="$(CPPFLAGS)" -C $(ETHTOOL_BUILD_DIR)

$(TARGET_DIR)/$(ETHTOOL_TARGET_DIR)/ethtool: $(ETHTOOL_BUILD_DIR)/ethtool
	$(STRIP) --strip-unneeded $(ETHTOOL_BUILD_DIR)/ethtool
	$(INSTALL) -m 755 $(ETHTOOL_BUILD_DIR)/ethtool $(TARGET_DIR)/$(ETHTOOL_TARGET_DIR)/ethtool

ethtool: $(TARGET_DIR)/$(ETHTOOL_TARGET_DIR)/ethtool

ethtool-clean:
	$(MAKE) -C $(ETHTOOL_BUILD_DIR) clean

ethtool-dirclean:
	rm -rf $(ETHTOOL_BUILD_DIR)

#############################################################
# Toplevel Makefile options
#############################################################
ifeq ($(strip $(MAL_PACKAGE_ETHTOOL)),y)
TARGETS+=ethtool
endif


