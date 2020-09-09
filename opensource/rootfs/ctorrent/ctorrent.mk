#############################################################
#
# ctorrent
#
#############################################################
CTORRENT_VER:=dnh3.2
CTORRENT_SOURCE:=$(PACKAGE_DIR)/ctorrent/ctorrent-$(CTORRENT_VER)
CTORRENT_DIR:=$(BUILD_DIR)/ctorrent-$(CTORRENT_VER)
CTORRENT_TARGET_BINARY:=usr/bin/ctorrent

$(CTORRENT_DIR)/.copied:
	cp -a $(CTORRENT_SOURCE) $(CTORRENT_DIR)
	$(PATCH) $(CTORRENT_DIR) $(PACKAGE_DIR)/ctorrent ctorrent-$(CTORRENT_VER)-\*.patch
	touch $(CTORRENT_DIR)/.copied

$(CTORRENT_DIR)/.configured: $(CTORRENT_DIR)/.copied
	(cd $(CTORRENT_DIR); \
		CPPFLAGS="-I$(STAGING_TARGET_DIR)/usr/include" \
		LD_LIBRARY_PATH="-L$(STAGING_TARGET_DIR)/usr/lib" \
		LDFLAGS="-L$(STAGING_TARGET_DIR)/usr/lib" \
		$(TARGET_CONFIGURE_OPTS) \
		./configure \
		--host=$(GNU_TARGET_NAME) \
		--target=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--prefix=/usr \
		--exec-prefix=/usr \
		--bindir=/usr/bin \
		--sbindir=/usr/sbin \
		--libdir=/usr/lib \
		--includedir=/usr/include \
	);
	touch $(CTORRENT_DIR)/.configured;

$(CTORRENT_DIR)/ctorrent: $(CTORRENT_DIR)/.configured
	$(MAKE) -C $(CTORRENT_DIR)

$(TARGET_DIR)/$(CTORRENT_TARGET_BINARY): $(CTORRENT_DIR)/ctorrent
	$(STRIP) $(CTORRENT_DIR)/ctorrent
	yes | cp -f $(CTORRENT_DIR)/ctorrent $(TARGET_DIR)/$(CTORRENT_TARGET_BINARY)
	# Install shared library denpendencies
	# Shared library: [libssl.so.$(OPENSSL_VER)]
	# Shared library: [librt.so.1]
	# Shared library: [libcrypto.so.$(OPENSSL_VER)]
	# Shared library: [libdl.so.2]
	# Shared library: [libstdc++.so.6]
	# Shared library: [libm.so.6]
	# Shared library: [libgcc_s.so.1]
	(cd $(TARGET_DIR)/lib; \
		yes | cp -f $(LIBS_DIR)/libssl.so.$(OPENSSL_VER) libssl.so.$(OPENSSL_VER); \
		$(STRIP) libssl.so.$(OPENSSL_VER); \
		ln -sf libssl.so.$(OPENSSL_VER) libssl.so.0; \
		ln -sf libssl.so.0 libssl.so; \
		yes | cp -f $(LIBS_DIR)/librt-2.3.4.so librt-2.3.4.so; \
		$(STRIP) librt-2.3.4.so; \
		ln -sf librt-2.3.4.so librt.so.1; \
		ln -sf librt.so.1 librt.so; \
		yes | cp -f $(LIBS_DIR)/libcrypto.so.$(OPENSSL_VER) libcrypto.so.$(OPENSSL_VER); \
		$(STRIP) libcrypto.so.$(OPENSSL_VER); \
		ln -sf libcrypto.so.$(OPENSSL_VER) libcrypto.so.0; \
		ln -sf libcrypto.so.0 libcrypto.so; \
		yes | cp -f $(LIBS_DIR)/libdl-2.3.4.so libdl-2.3.4.so; \
		$(STRIP) libdl-2.3.4.so; \
		ln -sf libdl-2.3.4.so libdl.so.2; \
		ln -sf libdl.so.2 libdl.so; \
		yes | cp -f $(LIBS_DIR)/libstdc++.so.6.0.3 libstdc++.so.6.0.3; \
		$(STRIP) libstdc++.so.6.0.3; \
		ln -sf libstdc++.so.6.0.3 libstdc++.so.6; \
		ln -sf libstdc++.so.6 libstdc++.so; \
		yes | cp -f $(LIBS_DIR)/libm-2.3.4.so libm-2.3.4.so; \
		$(STRIP) libm-2.3.4.so; \
		ln -sf libm-2.3.4.so libm.so.6; \
		ln -sf libm.so.6 libm.so; \
		yes | cp -f $(LIBS_DIR)/libgcc_s.so.1 libgcc_s.so.1; \
		$(STRIP) libgcc_s.so.1; \
		ln -sf libgcc_s.so.1 libgcc_s.so; \
	);

ctorrent: glibc openssl $(TARGET_DIR)/$(CTORRENT_TARGET_BINARY)

ctorrent-clean:
	-$(MAKE) -C $(CTORRENT_DIR) clean

ctorrent-dirclean:
	rm -f $(TARGET_DIR)/$(CTORRENT_TARGET_BINARY)
	rm -rf $(CTORRENT_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(MAL_PACKAGE_CTORRENT)),y)
TARGETS+=ctorrent
endif
