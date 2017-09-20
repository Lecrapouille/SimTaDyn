#!/bin/bash

export PATH="/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0"
bf=$(cygpath ${APPVEYOR_BUILD_FOLDER})
cd "$bf" || (echo "Cannot go to directory $bf"; return 1)

# Clone projects that SimTaDyn depends on.
make clone || exit 1

# Build SimTaDyn unit tests.
(cd tests && make -j4 && ./build/UnitTest -a) || exit 1

# Build SimTaDyn executable.
make -j4 || exit 1

# Build standalone Forth.
# FIXME: launch Forth unit tests when they will be done:
# && ./build/SimForth -f ../core/system.fs -f../core/tester.fs
(cd src/forth/standalone && make -j4) || exit 1
