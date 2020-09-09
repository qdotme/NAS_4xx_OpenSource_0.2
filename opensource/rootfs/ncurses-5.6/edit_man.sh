#! /bin/sh
# this script is generated by the configure-script CF_MAN_PAGES macro.

prefix="/usr"
datadir="${prefix}/share"

NCURSES_MAJOR="5"
NCURSES_MINOR="6"
NCURSES_PATCH="20061217"

NCURSES_OSPEED="short"
TERMINFO="/usr/share/terminfo"

MKDIRS="sh /opt/devel/proto/marvell/build-eabi/ncurses-5.6/mkinstalldirs"

INSTALL="/usr/bin/install -c"
INSTALL_DATA="${INSTALL} -m 644"

transform="s,^,arm-linux-,"

TMP=${TMPDIR-/tmp}/man$$
trap "rm -f $TMP" 0 1 2 5 15

form=$1
shift || exit 1

verb=$1
shift || exit 1

mandir=$1
shift || exit 1

srcdir=$1
top_srcdir=$srcdir/..
shift || exit 1

if test "$form" = normal ; then
	if test "no" = yes ; then
	if test "no" = no ; then
		sh $0 format $verb $mandir $srcdir $*
		exit 0
	fi
	fi
	cf_subdir=$mandir/man
	cf_tables=no
else
	cf_subdir=$mandir/cat
	cf_tables=yes
fi

# process the list of source-files
for i in $* ; do
case $i in #(vi
*.orig|*.rej) ;; #(vi
*.[0-9]*)
	section=`expr "$i" : '.*\.\([0-9]\)[xm]*'`;
	if test $verb = installing ; then
	if test ! -d $cf_subdir${section} ; then
		$MKDIRS $cf_subdir$section
	fi
	fi
	aliases=
	cf_source=`basename $i`
	inalias=$cf_source
	test ! -f $inalias && inalias="$srcdir/$inalias"
	if test ! -f $inalias ; then
		echo .. skipped $cf_source
		continue
	fi
	aliases=`sed -f $top_srcdir/man/manlinks.sed $inalias | sort -u`
	# perform program transformations for section 1 man pages
	if test $section = 1 ; then
		cf_target=$cf_subdir${section}/`echo $cf_source|sed "${transform}"`
	else
		cf_target=$cf_subdir${section}/$cf_source
	fi
	prog_captoinfo=`echo captoinfo|sed "${transform}"`
	prog_clear=`echo clear|sed "${transform}"`
	prog_infocmp=`echo infocmp|sed "${transform}"`
	prog_infotocap=`echo infotocap|sed "${transform}"`
	prog_tic=`echo tic|sed "${transform}"`
	prog_toe=`echo toe|sed "${transform}"`
	prog_tput=`echo tput|sed "${transform}"`
	sed	-e "s,@DATADIR@,$datadir," \
		-e "s,@TERMINFO@,$TERMINFO," \
		-e "s,@NCURSES_MAJOR@,$NCURSES_MAJOR," \
		-e "s,@NCURSES_MINOR@,$NCURSES_MINOR," \
		-e "s,@NCURSES_PATCH@,$NCURSES_PATCH," \
		-e "s,@NCURSES_OSPEED@,$NCURSES_OSPEED," \
		-e "s,@CAPTOINFO@,$prog_captoinfo," \
		-e "s,@CLEAR@,$prog_clear," \
		-e "s,@INFOCMP@,$prog_infocmp," \
		-e "s,@INFOTOCAP@,$prog_infotocap," \
		-e "s,@TIC@,$prog_tic," \
		-e "s,@TOE@,$prog_toe," \
		-e "s,@TPUT@,$prog_tput," \
		< $i >$TMP
if test $cf_tables = yes ; then
	tbl $TMP >$TMP.out
	mv $TMP.out $TMP
fi
	if test $form = format ; then
		nroff -man $TMP >$TMP.out
		mv $TMP.out $TMP
	fi
	if test $verb = installing ; then
	if ( gzip -f $TMP )
	then
		mv $TMP.gz $TMP
	fi
	fi
	cf_target="$cf_target.gz"
	suffix=`basename $cf_target | sed -e 's%^[^.]*%%'`
	if test $verb = installing ; then
		echo $verb $cf_target
		$INSTALL_DATA $TMP $cf_target
		test -n "$aliases" && (
			cd $cf_subdir${section} && (
				cf_source=`echo $cf_target |sed -e 's%^.*/\([^/][^/]*/[^/][^/]*$\)%\1%'`
				test -n "gz" && cf_source=`echo $cf_source |sed -e 's%\.gz$%%'`
				cf_target=`basename $cf_target`
				for cf_alias in $aliases
				do
					if test $section = 1 ; then
						cf_alias=`echo $cf_alias|sed "${transform}"`
					fi

					if test "yes" = yes ; then
						if test -f $cf_alias${suffix} ; then
							if ( cmp -s $cf_target $cf_alias${suffix} )
							then
								continue
							fi
						fi
						echo .. $verb alias $cf_alias${suffix}
						rm -f $cf_alias${suffix}
						ln -s $cf_target $cf_alias${suffix}
					elif test "$cf_target" != "$cf_alias${suffix}" ; then
						echo ".so $cf_source" >$TMP
						if test -n "gz" ; then
							gzip -f $TMP
							mv $TMP.gz $TMP
						fi
						echo .. $verb alias $cf_alias${suffix}
						rm -f $cf_alias${suffix}
						$INSTALL_DATA $TMP $cf_alias${suffix}
					fi
				done
			)
		)
	elif test $verb = removing ; then
		echo $verb $cf_target
		rm -f $cf_target
		test -n "$aliases" && (
			cd $cf_subdir${section} && (
				for cf_alias in $aliases
				do
					if test $section = 1 ; then
						cf_alias=`echo $cf_alias|sed "${transform}"`
					fi

					echo .. $verb alias $cf_alias${suffix}
					rm -f $cf_alias${suffix}
				done
			)
		)
	else
#		echo ".hy 0"
		cat $TMP
	fi
	;;
esac
done

if test no = yes ; then
if test $form != format ; then
	sh $0 format $verb $mandir $srcdir $*
fi
fi

exit 0
