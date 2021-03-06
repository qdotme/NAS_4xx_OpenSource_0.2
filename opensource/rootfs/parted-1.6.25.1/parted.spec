%define	name	parted
%define	ver	1.6.25.1
%define	rel	1
%define	prefix	/usr
%define	sbindir	/sbin
%define mandir	/usr/share/man
%define aclocaldir	/usr/share/aclocal


Summary		: flexible partitioning tool
Name		: %{name}
Version		: %{ver}
Release		: %{rel}
Source		: ftp://ftp.gnu.org/gnu/%{name}/%{name}-%{ver}.tar.gz
Buildroot	: %{_tmppath}/%{name}-root
Packager	: Fabian Emmes <fab@orlen.de>
Copyright	: GPL
Group		: Applications/System
Requires	: e2fsprogs, readline
BuildPrereq	: e2fsprogs-devel, readline-devel
%description
GNU Parted is a program that allows you to create, destroy,
resize, move and copy hard disk partitions. This is useful for
creating space for new operating systems, reorganising disk
usage, and copying data to new hard disks.


%package devel
Summary		: files required to compile software that uses libparted
Group		: Development/System
Requires	: e2fsprogs
BuildPrereq	: e2fsprogs-devel, readline-devel
%description devel
This package includes the header files and libraries needed to
statically link software with libparted.


%prep
%setup

%build
if [ -n "$LINGUAS" ]; then unset LINGUAS; fi
%configure --prefix=%{prefix} --sbindir=%{sbindir}
make


%install
rm -rf "$RPM_BUILD_ROOT"
make DESTDIR="$RPM_BUILD_ROOT" install
strip "${RPM_BUILD_ROOT}%{sbindir}"/parted


%clean
rm -rf "$RPM_BUILD_ROOT"


%files
%defattr(-,root,root)
%doc AUTHORS BUGS COPYING ChangeLog NEWS README THANKS TODO doc/COPYING.DOC doc/API doc/USER doc/FAT
%{prefix}/share/locale/*/*/*
%{sbindir}/*
%{mandir}/*/*
%{prefix}/lib/*.so*


%files devel
%defattr(-,root,root)
%{prefix}/include/*
%{aclocaldir}/*
%{prefix}/lib/*.a*
%{prefix}/lib/*.la*

%changelog
* Mon Mar 13 2000 Fabian Emmes <fab@orlen.de>
- changed "unset LINGUAS" line
- reintroduced %build section ;)
- started changelog

