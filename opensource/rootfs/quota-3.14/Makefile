# Generated automatically from Makefile.in by configure.
PROGS         = quotacheck quotaon quota quot repquota warnquota quotastats xqmstats edquota setquota convertquota rpc.rquotad 
SOURCES       = bylabel.c common.c convertquota.c edquota.c pot.c quot.c quota.c quotacheck.c quotacheck_v1.c quotacheck_v2.c quotaio.c quotaio_rpc.c quotaio_v1.c quotaio_v2.c quotaio_xfs.c quotaio_generic.c quotaon.c quotaon_xfs.c quotaops.c quotastats.c quotasys.c repquota.c rquota_client.c rquota_server.c rquota_svc.c setquota.c warnquota.c xqmstats.c svc_socket.c
VERSIONDEF    = -DQUOTA_VERSION=\"3.14\"
CFLAGS        = -D__GETTEXT__ -DBSD_BEHAVIOUR -DRPC -DALT_FORMAT -Os -pipe  -D_GNU_SOURCE -Wall -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 $(VERSIONDEF)
EXT2LIBS      = 
RPCSRC        = rquota.h rquota_xdr.c rquota_clnt.c
LIBS          = 
LDFLAGS       = 
LDAPLIBS      = 

# Add lines to /etc/hosts.allow and /etc/hosts.deny
# like: "rquotad: ALL@ALL except my.host.i.want.com" in hosts.deny means
#        only the host designated can get info from rquotad
CFLAGS       += 

INSTALL       = /usr/bin/install -c
LN            = ln -sf
ROOTDIR       =
SUPER_OWNER   = root
BIN_OWNER     = bin
BIN_GROUP     = bin
DEF_SUID_MODE = 4511
DEF_BIN_MODE = 555
DEF_SBIN_MODE = 555
DEF_MAN_MODE  = 444
DEF_CONF_MODE = 644
RPCGEN        = rpcgen

prefix        = /usr
bindir        = $(prefix)/bin
sbindir       = $(prefix)/sbin
root_sbindir  = $(prefix)/sbin
mandir        = ${prefix}/man
includedir    = $(prefix)/include
locale_dir    = $(prefix)/share/locale
sysconfdir    = ${prefix}/etc

RPCCLNTOBJS = rquota_xdr.o rquota_client.o rquota_clnt.o
IOOBJS = quotaio.o quotaio_v1.o quotaio_v2.o quotaio_rpc.o quotaio_xfs.o quotaio_generic.o
IOOBJS += $(RPCCLNTOBJS)
LIBOBJS = bylabel.o common.o quotasys.o pot.o $(IOOBJS)
LIBOBJS += 

.PHONY: all clean clobber realclean pot mo inst_mo

.%.d: %.c
	set -e; $(CC) -MM -MG $(CPPFLAGS) $< | \
		sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@
 
all: $(PROGS)

clean:
	-rm -f core *.o .*.d

clobber: clean
	-rm -f $(PROGS) Makefile config.status config.cache config.log

realclean: clobber
	-rm -f $(RPCSRC) po/*.mo

pot:
	xgettext -k_ -d pot *.c

mo: po
	for n in $(shell ls po/*.po); do \
	msgfmt -o po/`basename $$n .po`.mo $$n; \
	done

inst_mo: mo
	mkdir -p $(ROOTDIR)$(locale_dir)
	for n in $(shell ls po/*.po | sed 's/\.po/\.mo/'); do \
	l=`basename $$n .mo`; \
	$(INSTALL) -m 755 -d $(ROOTDIR)$(locale_dir)/$$l; \
	$(INSTALL) -m 755 -d $(ROOTDIR)$(locale_dir)/$$l/LC_MESSAGES; \
	$(INSTALL) -m 644 $$n $(ROOTDIR)$(locale_dir)/$$l/LC_MESSAGES/quota.mo; \
	done

install: all inst_mo
	-mkdir -p $(ROOTDIR)$(sbindir)
	-mkdir -p $(ROOTDIR)$(bindir)
	-mkdir -p $(ROOTDIR)$(root_sbindir)
	-$(INSTALL) -m $(DEF_SBIN_MODE) \
	quotacheck quotaon $(ROOTDIR)$(root_sbindir)
	-$(LN) quotaon $(ROOTDIR)$(root_sbindir)/quotaoff
	-$(INSTALL) -m $(DEF_SBIN_MODE) \
	edquota repquota warnquota quotastats setquota quot xqmstats \
	convertquota $(ROOTDIR)$(sbindir)
	-mkdir -p $(ROOTDIR)$(sysconfdir)
	-$(INSTALL) -m $(DEF_CONF_MODE) warnquota.conf $(ROOTDIR)$(sysconfdir)
	-$(INSTALL) -m $(DEF_CONF_MODE) quotatab $(ROOTDIR)$(sysconfdir)
	-$(INSTALL) -m $(DEF_CONF_MODE) quotagrpadmins $(ROOTDIR)$(sysconfdir)
	-mkdir -p $(ROOTDIR)$(mandir)/man1
	-mkdir -p $(ROOTDIR)$(mandir)/man2
	-mkdir -p $(ROOTDIR)$(mandir)/man3
	-mkdir -p $(ROOTDIR)$(mandir)/man8
	-$(INSTALL) -m 755 -d $(ROOTDIR)$(includedir)/rpcsvc
	-$(INSTALL) -m 644 rquota.h rquota.x $(ROOTDIR)$(includedir)/rpcsvc
	-$(INSTALL) -s -m $(DEF_SBIN_MODE) quota $(ROOTDIR)$(bindir)
	-$(INSTALL) -s -m $(DEF_SBIN_MODE) rpc.rquotad $(ROOTDIR)$(sbindir)
	-$(INSTALL) -m $(DEF_MAN_MODE) *.1 $(ROOTDIR)$(mandir)/man1
	-$(INSTALL) -m $(DEF_MAN_MODE) *.2 $(ROOTDIR)$(mandir)/man2
	-$(INSTALL) -m $(DEF_MAN_MODE) *.3 $(ROOTDIR)$(mandir)/man3
	-$(INSTALL) -m $(DEF_MAN_MODE) *.8 $(ROOTDIR)$(mandir)/man8

quotaon: quotaon.o quotaon_xfs.o $(LIBOBJS)

quotacheck: quotacheck.o quotacheck_v1.o quotacheck_v2.o quotaops.o $(LIBOBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(EXT2LIBS)

quota: quota.o quotaops.o $(LIBOBJS)

quot: quot.o $(LIBOBJS)

repquota: repquota.o $(LIBOBJS)

warnquota: warnquota.o $(LIBOBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDAPLIBS)

quotastats: quotastats.o common.o pot.o

xqmstats: xqmstats.o common.o pot.o

edquota: edquota.o quotaops.o $(LIBOBJS)

setquota: setquota.o quotaops.o $(LIBOBJS)

convertquota: convertquota.o $(LIBOBJS)

rpc.rquotad: rquota_server.o rquota_svc.o svc_socket.o $(LIBOBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

pot.o: pot.c pot.h
	$(CC) $(CFLAGS) -c $<

rquota.h: rquota.x
	$(RPCGEN) -h -o $@ $<

rquota_xdr.c: rquota.x
	$(RPCGEN) -c -o $@ $<

rquota_xdr.o: rquota_xdr.c rquota.h
	$(CC) $(CFLAGS) -Wno-unused -c $<

rquota_clnt.c: rquota.x
	$(RPCGEN) -l -o $@ $<

-include $(SOURCES:%.c=.%.d)
