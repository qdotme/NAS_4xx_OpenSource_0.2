# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $

include $(top_srcdir)/Makefile.am.common

if KRB4
dir_45 = 45
endif
if OTP
dir_otp = otp
endif
if DCE
dir_dce = kdfs
endif

SUBDIRS = @DIR_roken@ vers editline @DIR_com_err@ sl asn1 @DIR_des@ krb5 \
	kafs hdb kadm5 gssapi auth $(dir_45) $(dir_otp) $(dir_dce)
