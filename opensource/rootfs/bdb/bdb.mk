#############################################################
#
# bdb
#
#############################################################
BDB_VER:=4.2.52
BDB_SOURCE:=$(PACKAGE_DIR)/bdb/db-$(BDB_VER)
BDB_DIR:=$(BUILD_DIR)/db-$(BDB_VER)

$(BDB_DIR)/.copied:
	cp -a $(BDB_SOURCE) $(BDB_DIR)
	#$(PATCH) $(BDB_DIR) $(PACKAGE_DIR)/bdb bdb-\*.patch
	echo 0 > $(BDB_DIR)/.copied

$(BDB_DIR)/.configured: $(BDB_DIR)/.copied
	(cd $(BDB_DIR)/build_unix; rm -f config.cache; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS=-L$(LIBS_DIR) \
		CPPFLAGS=-I$(INCLUDE_DIR) \
		../dist/configure \
		--target=$(GNU_TARGET_NAME) \
		--host=$(GNU_TARGET_NAME) \
		--build=$(GNU_HOST_NAME) \
		--prefix=/usr \
		--exec-prefix=/usr \
	);
	touch  $(BDB_DIR)/.configured

$(BDB_DIR)/build_unix/.libs/libdb-4.2.so: $(BDB_DIR)/.configured
	$(MAKE) -C ${BDB_DIR}/build_unix
	#$(STRIP) --strip-unneeded $(BDBDIR)/build_unix/.libs/libdb-4.2.so
	#$(STRIP) --strip-unneeded $(BDBDIR)/build_unix/.libs/libdb-4.2.a

$(TARGET_DIR)/usr/lib/libdb-4.2.so: $(BDB_DIR)/build_unix/.libs/libdb-4.2.so
	#$(INSTALL) -m 644 $(BDB_DIR)/build_unix/.libs/libdb-4.2.so $(TARGET_DIR)/usr/lib/libdb-4.2.so
	#$(INSTALL) -m 644 $(BDB_DIR)/build_unix/.libs/libdb-4.2.a $(TARGET_DIR)/usr/lib/libdb-4.2.a
	#ln -s $(TARGET_DIR)/usr/lib/libdb-4.2.a $(TARGET_DIR)/usr/lib/libdb.a
	#mkdir -p $(TARGET_DIR)/usr/include/db4
	#$(INSTALL) -m 644 $(BDB_DIR)/build_unix/db.h $(TARGET_DIR)/usr/include/db4/db.h
	#$(INSTALL) -m 644 $(BDB_DIR)/build_unix/db_cxx.h $(TARGET_DIR)/usr/include/db4/db_cxx.h
	$(MAKE) DESTDIR=$(TARGET_DIR) CC=$(TARGET_CC) -C $(BDB_DIR)/build_unix install_include 
	$(MAKE) DESTDIR=$(TARGET_DIR) CC=$(TARGET_CC) -C $(BDB_DIR)/build_unix install_lib 

bdb: glibc $(TARGET_DIR)/usr/lib/libdb-4.2.so

bdb-clean:
	-$(MAKE) -C $(BDB_DIR)/build_unix clean

bdb-dirclean:
	rm -rf $(BDB_DIR)

#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(MAL_PACKAGE_BDB)),y)
TARGETS+=bdb
endif
