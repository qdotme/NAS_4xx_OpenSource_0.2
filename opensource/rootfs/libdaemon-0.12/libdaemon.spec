%define debug_package %{nil}

Summary: lightweight C library which eases the writing of UNIX daemons.
Name: libdaemon
Version: 0.12
Release: 1
URL: http://www.stud.uni-hamburg.de/users/lennart/projects/libdaemon
Source: %{name}-%{version}.tar.gz
License: LGPL 
Group: System Environment/Libraries
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
libdaemon is a lightweight C library which eases the writing of UNIX daemons.
It consists of the following parts:

    * A wrapper around fork() which does the correct daemonization
      procedure of a process
    * A wrapper around syslog() for simpler and compatible log output to
      Syslog or STDERR
    * An API for writing PID files
    * An API for serializing UNIX signals into a pipe for usage with
      select() or poll()

Routines like these are included in most of the daemon software available. It
is not that simple to get it done right and code duplication cannot be a goal.

%package devel
Summary: Static libraries and header files for libdaemon development.
Group: Development/Libraries
Requires: libdaemon = %{version}

%description devel

The libdaemon-devel package contains the header files and static libraries
necessary for developing programs using libdaemon.

%prep
%setup -q

%build
%configure
make

%install
%makeinstall

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc LICENSE README
%{_libdir}/*so.*
%{_libdir}/*.la

%files devel
%defattr(-,root,root)
%doc LICENSE README
%doc doc/*
%{_includedir}/*
%{_libdir}/*.a
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc

%changelog
* Mon Jul 21 2003 Lennart Poettering 0.3
- fixes
* Wed Jul 16 2003 Diego Santa Cruz <Diego.SantaCruz@epfl.ch> 0.2
- initial RPM
