#!/bin/bash

# Detect number of CPU cores. TODO Mac OS X
CORES=`grep -c '^processor' /proc/cpuinfo` 2> /dev/null
if [ "$CORES" == "" ]; then
    CORES=2
fi
JCORES="-j"$(( CORES * 2 ))
echo "I detected $CORES CPUs I'll use $JCORES"

# Clone projects that SimTaDyn depends on.
make clone || exit 1

# Export display
# Xvfb :1 -screen 0 1024x768x16 -nolisten tcp &
# export DISPLAY=":1.0"

# Disable OpenGL tests.
export CI=true

# Build and install SimTaDyn executable.
make install $JCORES || exit 1

# Build SimTaDyn unit tests.
(cd tests && make unit-tests $JCORES) || exit 1

# Build standalone Forth.
# FIXME: launch Forth unit tests when they will be done:
# && ./build/SimForth -f ../core/system.fs -f../core/tester.fs
(cd src/forth/standalone && make $JCORES) || exit 1
