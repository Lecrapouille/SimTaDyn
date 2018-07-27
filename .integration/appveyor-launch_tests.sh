#!/bin/bash

export PATH="/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0"
bf=$(cygpath ${APPVEYOR_BUILD_FOLDER})
cd "$bf" || (echo "Cannot go to directory $bf"; return 1)

function exists
{
  test -e $1 || (echo "The file $1 does not exist" && exit 1)
}

# FIXME: get the number of cores
JCORES="-j4"

# Clone and compile projects that SimTaDyn depends on.
make download-external-libs || exit 1
make compile-external-libs || exit 1

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
