cmd_drivers/md/mktables := gcc -Wp,-MD,drivers/md/.mktables.d -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer     -o drivers/md/mktables drivers/md/mktables.c  

deps_drivers/md/mktables := \
  drivers/md/mktables.c \
  /usr/include/stdio.h \
  /usr/include/features.h \
  /usr/include/sys/cdefs.h \
  /usr/include/gnu/stubs.h \
  /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h \
  /usr/include/bits/types.h \
  /usr/include/bits/wordsize.h \
  /usr/include/bits/typesizes.h \
  /usr/include/libio.h \
  /usr/include/_G_config.h \
  /usr/include/wchar.h \
  /usr/include/bits/wchar.h \
  /usr/include/gconv.h \
  /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h \
  /usr/include/bits/stdio_lim.h \
  /usr/include/bits/sys_errlist.h \
  /usr/include/bits/stdio.h \
  /usr/include/string.h \
  /usr/include/bits/string.h \
  /usr/include/bits/string2.h \
  /usr/include/endian.h \
  /usr/include/bits/endian.h \
  /usr/include/stdlib.h \
  /usr/include/inttypes.h \
  /usr/include/stdint.h \
  /usr/include/sys/types.h \
  /usr/include/time.h \
  /usr/include/sys/select.h \
  /usr/include/bits/select.h \
  /usr/include/bits/sigset.h \
  /usr/include/bits/time.h \
  /usr/include/sys/sysmacros.h \
  /usr/include/bits/pthreadtypes.h \
  /usr/include/bits/sched.h \
  /usr/include/alloca.h \

drivers/md/mktables: $(deps_drivers/md/mktables)

$(deps_drivers/md/mktables):
