#!/bin/bash

function exists
{
  test -e $1 || (echo "The file $1 does not exist" && exit 1)
}

# Detect number of CPU cores. TODO Mac OS X
CORES=`grep -c '^processor' /proc/cpuinfo` 2> /dev/null
if [ "$CORES" == "" ]; then
    CORES=2
fi
JCORES="-j"$(( CORES * 2 ))
echo "I detected $CORES CPUs I'll use $JCORES"

# Clone and compile projects that SimTaDyn depends on.
make download-external-libs || exit 1
make compile-external-libs || exit 1

# Export display
# Xvfb :1 -screen 0 1024x768x16 -nolisten tcp &
# export DISPLAY=":1.0"

# Disable OpenGL tests.
export CI=true

# Build and install SimTaDyn executable.
make install DESTDIR=/tmp $JCORES || exit 1
VERSION=`cat VERSION`
exists /tmp/usr/share/SimTaDyn/$VERSION/data
exists /tmp/usr/share/SimTaDyn/$VERSION/doc
exists /tmp/usr/share/SimTaDyn/$VERSION/AUTHORS
exists /tmp/usr/share/SimTaDyn/$VERSION/README.md
exists /tmp/usr/share/SimTaDyn/$VERSION/LICENSE
exists /tmp/usr/share/SimTaDyn/$VERSION/ChangeLog
exists /tmp/usr/bin/SimTaDyn-$VERSION

# Build SimTaDyn unit tests.
(cd tests && make unit-tests $JCORES) || exit 1

# Build standalone Forth.
# FIXME: launch Forth unit tests when they will be done:
# && ./build/SimForth -f ../core/system.fs -f../core/tester.fs
(cd src/forth/standalone && make $JCORES) || exit 1

# Build OpenGL examples
(cd src/common/graphics/OpenGL/examples/ && make $JCORES) || exit 1
