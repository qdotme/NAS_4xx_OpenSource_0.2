AC_DEFUN([AC_NETATALK_CNID], [

    dnl Don't use BDB unless it's needed
    bdb_required=no
    compiled_backends=""

    dnl Determine whether or not to use BDB Concurrent Data Store
    AC_MSG_CHECKING([whether or not to use BDB Concurrent Data Store])
    AC_ARG_WITH(cnid-cdb-backend,
	[  --with-cnid-cdb-backend	build CNID with Concurrent BDB Data Store],[
	    if test x"$withval" = x"no"; then
	        use_cdb_backend=no
        else
            use_cdb_backend=yes
        fi
    ],[use_cdb_backend=yes]
    )

    if test $use_cdb_backend = yes; then
        AC_MSG_RESULT([yes])
        AC_DEFINE(CNID_BACKEND_CDB, 1, [Define if CNID Concurrent BDB backend should be compiled.])	    
        DEFAULT_CNID_SCHEME=cdb
        bdb_required=yes
        compiled_backends="$compiled_backends cdb"
    else
        AC_MSG_RESULT([no])
    fi

    dnl Determine whether or not to use Database Daemon CNID backend
    AC_MSG_CHECKING([whether or not to use Database Daemon CNID backend])
    AC_ARG_WITH(cnid-dbd-backend,
    [  --with-cnid-dbd-backend       build CNID with Database Daemon Data Store],
    [   if test x"$withval" = x"no"; then
            AC_MSG_RESULT([no])
            use_dbd_backend=no
        else
            use_dbd_backend=yes
            AC_MSG_RESULT([yes])
        fi
    ],[
        use_dbd_backend=yes
        AC_MSG_RESULT([yes])
    ])

    dnl Determine whether or not to use with transaction support in Database Daemon
    AC_MSG_CHECKING([whether or not to use Database Daemon with transaction support])
    AC_ARG_WITH(cnid-dbd-txn,
    [  --with-cnid-dbd-txn           build transaction support for dbd backend],
    [   if test x"$withval" = x"no"; then
            AC_MSG_RESULT([no])
            use_dbd_txn=no
        else
            use_dbd_txn=yes
            AC_MSG_RESULT([yes])
        fi
	],[
        use_dbd_txn=no
        AC_MSG_RESULT([no])
    ])

    if test $use_dbd_txn = yes; then
        use_dbd_backend=yes
        AC_DEFINE(CNID_BACKEND_DBD_TXN, 1, [Define if CNID Database Daemon backend has transaction support])
    else
        if test x"$use_dbd_backend" = x; then    
            use_dbd_backend=no
        fi
    fi

    if test $use_dbd_backend = yes; then
        compiled_backends="$compiled_backends dbd"
        AC_DEFINE(CNID_BACKEND_DBD, 1, [Define if CNID Database Daemon backend should be compiled.])
        if test x"$DEFAULT_CNID_SCHEME" = x; then
            DEFAULT_CNID_SCHEME=dbd
        fi
        bdb_required=yes
    fi
    AM_CONDITIONAL(BUILD_DBD_DAEMON, test x"$use_dbd_backend" = x"yes")

    dnl Determine whether or not to use BDB transactional data store
    AC_MSG_CHECKING([whether or not to use BDB transactional DB store])
    AC_ARG_WITH(cnid-db3-backend,
    [  --with-cnid-db3-backend	build CNID with transactional BDB Data Store],
	[
	    if test x"$withval" = x"no"; then
            use_db3_backend=no
        else
            use_db3_backend=yes
        fi
    ],[
        use_db3_backend=no
    ])

    if test x"$use_db3_backend" = x"yes"; then
        AC_MSG_RESULT([yes])
        AC_DEFINE(CNID_BACKEND_DB3, 1, [Define if CNID transactional BDB backend should be compiled.])
        if test x"$DEFAULT_CNID_SCHEME" = x; then
            DEFAULT_CNID_SCHEME=db3
        fi
        compiled_backends="$compiled_backends db3"
        bdb_required=yes
    else
        AC_MSG_RESULT([no])
    fi

    dnl Determine whether or not to use LAST DID scheme
    AC_MSG_CHECKING([whether or not to use LAST DID scheme])
    AC_ARG_WITH(cnid-last-backend,
	[  --with-cnid-last-backend	build LAST CNID scheme],
	[
        if test x"$withval" = x"no"; then
            use_last_backend=no
        else
            use_last_backend=yes
        fi
    ],[
        use_last_backend=yes
    ])

    if test $use_last_backend = yes; then
        AC_MSG_RESULT([yes])
        AC_DEFINE(CNID_BACKEND_LAST, 1, [Define if CNID LAST scheme backend should be compiled.])
        if test x"$DEFAULT_CNID_SCHEME" = x; then
            DEFAULT_CNID_SCHEME=last
        fi
        compiled_backends="$compiled_backends last"
    else
        AC_MSG_RESULT([no])
    fi

    dnl Determine whether or not to use HASH DID scheme
    AC_MSG_CHECKING([whether or not to use HASH DID scheme])
    AC_ARG_WITH(cnid-hash-backend,
	[  --with-cnid-hash-backend	build HASH CNID scheme],
	[
        if test x"$withval" = x"no"; then
            use_hash_backend=no
        else
            use_hash_backend=yes
        fi
    ],[
        use_hash_backend=no
    ])

    if test $use_hash_backend = yes; then
        AC_MSG_RESULT([yes])
        AC_DEFINE(CNID_BACKEND_HASH, 1, [Define if CNID HASH scheme backend should be compiled.])
        if test x"$DEFAULT_CNID_SCHEME" = x; then
            DEFAULT_CNID_SCHEME=hash
        fi
        compiled_backends="$compiled_backends hash"
    else
        AC_MSG_RESULT([no])
    fi

    dnl Determine whether or not to use TDB DID scheme
    AC_MSG_CHECKING([whether or not to use TDB DID scheme])
    AC_ARG_WITH(cnid-tdb-backend,
	[  --with-cnid-tdb-backend	build DID CNID scheme],
    [
        if test x"$withval" = x"no"; then
            use_tdb_backend=no
        else
            use_tdb_backend=yes
        fi
    ],[
        use_tdb_backend=no
    ])

    if test $use_tdb_backend = yes; then
        AC_MSG_RESULT([yes])
        AC_DEFINE(CNID_BACKEND_TDB, 1, [Define if CNID TDB scheme backend should be compiled.])
        if test x"$DEFAULT_TDB_SCHEME" = x; then
            DEFAULT_CNID_SCHEME=tdb
        fi
        compiled_backends="$compiled_backends tdb"
    else
        AC_MSG_RESULT([no])
    fi

    dnl Determine whether or not to use MTAB DID scheme
    AC_MSG_CHECKING([whether or not to use MTAB DID scheme])
    AC_ARG_WITH(cnid-mtab-backend,
	[  --with-cnid-mtab-backend	build MTAB CNID scheme],
    [
        if test x"$withval" = x"no"; then
            use_mtab_backend=no
        else
            use_mtab_backend=yes
        fi
    ],[
        use_mtab_backend=no
    ])

    if test $use_mtab_backend = yes; then
        AC_MSG_RESULT([yes])
        AC_DEFINE(CNID_BACKEND_MTAB, 1, [Define if CNID MTAB scheme backend should be compiled.])
        if test x"$DEFAULT_CNID_SCHEME" = x; then
            DEFAULT_CNID_SCHEME=mtab
        fi
        compiled_backends="$compiled_backends mtab"
    else		
        AC_MSG_RESULT([no])
    fi

    dnl Set default DID scheme
    AC_MSG_CHECKING([default DID scheme])
    AC_ARG_WITH(cnid-default-backend,
	[  --with-cnid-default-backend=val	set default DID scheme],
    [
        if test x"$withval" = x; then
            AC_MSG_RESULT([ignored])
        else
            DEFAULT_CNID_SCHEME=$withval
            AC_MSG_RESULT($DEFAULT_CNID_SCHEME)
        fi
    ],[
        AC_MSG_RESULT($DEFAULT_CNID_SCHEME)
    ])

    if test x"$DEFAULT_CNID_SCHEME" = x; then
        AC_MSG_ERROR([No DID schemes compiled in ])
    fi

    AC_MSG_CHECKING([whether default CNID scheme has been activated])
    found_scheme=no
    for scheme in $compiled_backends ; do
        if test x"$scheme" = x"$DEFAULT_CNID_SCHEME"; then	
            found_scheme=yes
        fi
    done
    if test x"$found_scheme" = x"no"; then
        AC_MSG_RESULT([no])
        AC_MSG_ERROR([Specified default CNID scheme $DEFAULT_CNID_SCHEME was not selected for compilation])
    else
        AC_MSG_RESULT([yes])
    fi

    AC_DEFINE_UNQUOTED(DEFAULT_CNID_SCHEME, "$DEFAULT_CNID_SCHEME", [Default CNID scheme to be used])
    AC_SUBST(DEFAULT_CNID_SCHEME)
    AC_SUBST(compiled_backends)

    if test "x$bdb_required" = "xyes"; then
	ifelse([$1], , :, [$1])
    else
	ifelse([$2], , :, [$2])     
    fi
])
