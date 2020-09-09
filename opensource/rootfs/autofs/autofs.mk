#############################################################
#
# autofs
#
#############################################################
AUTOFS_VER:=4.1.4
AUTOFS_SOURCE:=$(PACKAGE_DIR)/autofs/autofs-$(AUTOFS_VER)
AUTOFS_DIR:=$(BUILD_DIR)/autofs-$(AUTOFS_VER)
AUTOFS_SRCDIR:=$(PACKAGE_DIR)/autofs

AUTOFS_BINARY:=automount
AUTOFS_TARGET_BINARY:=usr/sbin/automount


$(AUTOFS_DIR)/.copied:
	cp -a $(AUTOFS_SOURCE) $(AUTOFS_DIR)
	$(PATCH) $(AUTOFS_DIR) $(AUTOFS_SRCDIR) autofs\*.patch
	touch $(AUTOFS_DIR)/.copied

$(AUTOFS_DIR)/.configured: $(AUTOFS_DIR)/.copied
	(cd $(AUTOFS_DIR); rm -f config.cache; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS=-L$(LIBS_DIR) \
		CPPFLAGS="-I$(INCLUDE_DIR) -I$(AUTOFS_DIR)/include"\
		gcc_supports_pie=yes \
		./configure \
		--target=$(GNU_TARGET_NAME) \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--prefix=/usr \
		--bindir=/usr/bin \
		--sbindir=/usr/sbin \
		--libexecdir=/usr/lib \
		--sysconfdir=/etc \
		--datadir=/usr/share \
		--localstatedir=/var \
		--mandir=/usr/man \
		--infodir=/usr/info \
		--disable-ext-env \
		--without-openldap \
		--without-hesiod \
	);
	touch $(AUTOFS_DIR)/.conifgured
		

$(AUTOFS_DIR)/$(AUTOFS_BINARY): $(AUTOFS_DIR)/.configured
	$(TARGET_CONFIGURE_OPTS) $(MAKE) STRIP="$(STRIP)" CPPFLAGS="-I$(INCLUDE_DIR) -I$(AUTOFS_DIR)/include" CC=$(TARGET_CC) -C $(AUTOFS_DIR)

$(TARGET_DIR)/$(AUTOFS_TARGET_BINARY): $(AUTOFS_DIR)/$(AUTOFS_BINARY)
	$(MAKE) INSTALLROOT=$(TARGET_DIR) -C $(AUTOFS_DIR) install
	rm -Rf $(TARGET_DIR)/usr/man
#	$(SED) "s,STRIP     = strip --strip-debug,STRIP = $(STRIP),g" $(AUTOFS_DIR)/Makefile.rules
	# Install shared library denpendencies
	# Shared library: [libdl.so.2]
	(cd $(TARGET_DIR)/lib; \
		yes | cp -f $(LIBS_DIR)/libdl-2.3.4.so libdl-2.3.4.so; \
		$(STRIP) libdl-2.3.4.so; \
		ln -sf libdl-2.3.4.so libdl.so.2; \
		ln -sf libdl.so.2 libdl.so; \
		);

autofs: glibc $(TARGET_DIR)/$(AUTOFS_TARGET_BINARY)

autofs-clean:
#	$(MAKE) prefix=$(TARGET_DIR)/usr -C $(AUTOFS_DIR) uninstall
	-$(MAKE) -C $(AUTOFS_DIR) clean

autofs-dirclean:
	rm -rf $(AUTOFS_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(MAL_PACKAGE_AUTOFS)),y)
TARGETS+=autofs
endif
