# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4) $(INCLUDE_des) -I../com_err -I$(srcdir)/../com_err

bin_PROGRAMS = verify_krb5_conf

noinst_PROGRAMS =				\
	dump_config				\
	test_get_addrs				\
	krbhst-test				\
	test_alname				\
	test_crypto				\
	test_kuserok

TESTS =						\
	aes-test				\
	derived-key-test			\
	n-fold-test				\
	name-45-test				\
	parse-name-test				\
	store-test				\
	string-to-key-test			\
	test_acl				\
	test_addr				\
	test_cc					\
	test_config				\
	test_keytab				\
	test_time				\
	test_mem

check_PROGRAMS = $(TESTS)

LDADD = libkrb5.la \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_roken)

libkrb5_la_LIBADD = \
	$(LIB_com_err) \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_roken) \
	$(LIB_door_create) \
	$(LIB_dlopen)

lib_LTLIBRARIES = libkrb5.la

ERR_FILES = krb5_err.c heim_err.c k524_err.c

libkrb5_la_CPPFLAGS = -DBUILD_KRB5_LIB $(AM_CPPFLAGS)

libkrb5_la_SOURCES =				\
	acache.c				\
	acl.c					\
	add_et_list.c				\
	addr_families.c				\
	aname_to_localname.c			\
	appdefault.c				\
	asn1_glue.c				\
	auth_context.c				\
	build_ap_req.c				\
	build_auth.c				\
	cache.c					\
	changepw.c				\
	codec.c					\
	config_file.c				\
	config_file_netinfo.c			\
	convert_creds.c				\
	constants.c				\
	context.c				\
	copy_host_realm.c			\
	crc.c					\
	creds.c					\
	crypto.c				\
	data.c					\
	eai_to_heim_errno.c			\
	error_string.c				\
	expand_hostname.c			\
	fcache.c				\
	free.c					\
	free_host_realm.c			\
	generate_seq_number.c			\
	generate_subkey.c			\
	get_addrs.c				\
	get_cred.c				\
	get_default_principal.c			\
	get_default_realm.c			\
	get_for_creds.c				\
	get_host_realm.c			\
	get_in_tkt.c				\
	get_in_tkt_pw.c				\
	get_in_tkt_with_keytab.c		\
	get_in_tkt_with_skey.c			\
	get_port.c				\
	init_creds.c				\
	init_creds_pw.c				\
	kcm.c					\
	kcm.h					\
	keyblock.c				\
	keytab.c				\
	keytab_any.c				\
	keytab_file.c				\
	keytab_memory.c				\
	keytab_keyfile.c			\
	keytab_krb4.c				\
	krbhst.c				\
	kuserok.c				\
	log.c					\
	mcache.c				\
	misc.c					\
	mk_error.c				\
	mk_priv.c				\
	mk_rep.c				\
	mk_req.c				\
	mk_req_ext.c				\
	mk_safe.c				\
	mit_glue.c				\
	net_read.c				\
	net_write.c				\
	n-fold.c				\
	padata.c				\
	pkinit.c				\
	principal.c				\
	prog_setup.c				\
	prompter_posix.c			\
	rd_cred.c				\
	rd_error.c				\
	rd_priv.c				\
	rd_rep.c				\
	rd_req.c				\
	rd_safe.c				\
	read_message.c				\
	recvauth.c				\
	replay.c				\
	send_to_kdc.c				\
	sendauth.c				\
	set_default_realm.c			\
	sock_principal.c			\
	store.c					\
	store-int.h				\
	store_emem.c				\
	store_fd.c				\
	store_mem.c				\
	ticket.c				\
	time.c					\
	transited.c				\
	v4_glue.c				\
	verify_init.c				\
	verify_user.c				\
	version.c				\
	warn.c					\
	write_message.c				\
	$(ERR_FILES)

libkrb5_la_LDFLAGS = -version-info 21:0:4

$(libkrb5_la_OBJECTS): $(srcdir)/krb5-protos.h $(srcdir)/krb5-private.h

$(srcdir)/krb5-protos.h:
	cd $(srcdir) && perl ../../cf/make-proto.pl -E KRB5_LIB_FUNCTION -q -P comment -o krb5-protos.h $(libkrb5_la_SOURCES) || rm -f krb5-protos.h

$(srcdir)/krb5-private.h:
	cd $(srcdir) && perl ../../cf/make-proto.pl -q -P comment -p krb5-private.h $(libkrb5_la_SOURCES) || rm -f krb5-private.h

CHECK_SYMBOLS = -lib krb5 -com_err -version \
	krb5_ _krb5_ __heimdal krb524_ krb4_fkt_ops heim_ pkcs7_

#libkrb5_la_LIBADD = ../com_err/error.lo ../com_err/com_err.lo

man_MANS =					\
	kerberos.8				\
	krb5.3					\
	krb5.conf.5				\
	krb524_convert_creds_kdc.3		\
	krb5_425_conv_principal.3		\
	krb5_acl_match_file.3			\
	krb5_address.3				\
	krb5_aname_to_localname.3		\
	krb5_appdefault.3			\
	krb5_auth_context.3			\
	krb5_c_make_checksum.3			\
	krb5_ccache.3				\
	krb5_check_transited.3			\
	krb5_compare_creds.3			\
	krb5_config.3				\
	krb5_context.3				\
	krb5_create_checksum.3			\
	krb5_creds.3				\
	krb5_crypto_init.3			\
	krb5_data.3				\
	krb5_eai_to_heim_errno.3		\
	krb5_encrypt.3				\
	krb5_expand_hostname.3			\
	krb5_find_padata.3			\
	krb5_generate_random_block.3		\
	krb5_get_all_client_addrs.3		\
	krb5_get_credentials.3			\
	krb5_get_forwarded_creds.3		\
	krb5_get_in_cred.3			\
	krb5_get_init_creds.3			\
	krb5_get_krbhst.3			\
	krb5_getportbyname.3			\
	krb5_init_context.3			\
	krb5_is_thread_safe.3			\
	krb5_keyblock.3				\
	krb5_keytab.3				\
	krb5_krbhst_init.3			\
	krb5_kuserok.3				\
	krb5_openlog.3				\
	krb5_parse_name.3			\
	krb5_principal.3			\
	krb5_rcache.3				\
	krb5_rd_error.3				\
	krb5_set_default_realm.3		\
	krb5_set_password.3			\
	krb5_storage.3				\
	krb5_string_to_key.3			\
	krb5_ticket.3				\
	krb5_timeofday.3			\
	krb5_unparse_name.3			\
	krb5_verify_init_creds.3		\
	krb5_verify_user.3			\
	krb5_warn.3				\
	verify_krb5_conf.8

include_HEADERS = krb5.h krb5-protos.h krb5-private.h krb5_err.h heim_err.h k524_err.h krb5_ccapi.h

CLEANFILES = krb5_err.c krb5_err.h heim_err.c heim_err.h k524_err.c k524_err.h

$(libkrb5_la_OBJECTS): krb5_err.h heim_err.h k524_err.h

# to help stupid solaris make

krb5_err.h: krb5_err.et

heim_err.h: heim_err.et

k524_err.h: k524_err.et
