#!/bin/sh

# Execute this script from the main netatalk source directory.

set -e

debiandir=distrib/debian

if test ! -d libatalk; then
  echo "Not in netatalk directory"
  exit 1
fi

VERSION=`cat VERSION`
DEBVERSION="${VERSION}cvs"
DISTDIR="netatalk-$VERSION"
DISTTGZ="netatalk_$DEBVERSION.orig.tar.gz"

if test ! -f INSTALL; then
  if test -e INSTALL; then
    echo "INSTALL is in the way, please move it away"
    exit 1
  fi
  touch INSTALL
fi

if test ! -x configure; then
  ./autogen.sh
fi

if test ! -e Makefile; then
  if test -x config.status; then
    ./config.status
  else
    ./configure
  fi
fi

make dist

mv "netatalk-$VERSION.tar.gz" "$DISTTGZ"
rm -rf "$DISTDIR" || true
tar xzf "$DISTTGZ"

for FILE in `find $debiandir/patches/*.patch`; do
  patch --dir="$DISTDIR" --strip=1 <$FILE
done

cp -a "$debiandir" "$DISTDIR"
rm -r "$DISTDIR/debian/CVS"
rm -r "$DISTDIR/debian/patches"
rm -r "$DISTDIR/debian/split-init"
rm    "$DISTDIR/debian/cvs2deb.sh"

cd $DISTDIR

touch INSTALL

automake

dpkg-buildpackage -rfakeroot

cd ..
rm -r "$DISTDIR"
rm INSTALL
