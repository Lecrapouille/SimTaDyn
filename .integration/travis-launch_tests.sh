#!/bin/bash

# Clone projects that SimTaDyn depends on.
make clone || exit 1

# Export display
# Xvfb :1 -screen 0 1024x768x16 -nolisten tcp &
# export DISPLAY=":1.0"

# Build SimTaDyn unit tests.
(cd tests && make -j4 && ./build/UnitTest -a) || exit 1

# Build SimTaDyn executable.
make -j4 || exit 1

# Build standalone Forth.
# FIXME: launch Forth unit tests when they will be done:
# && ./build/SimForth -f ../core/system.fs -f../core/tester.fs
(cd src/forth/standalone && make -j4) || exit 1
