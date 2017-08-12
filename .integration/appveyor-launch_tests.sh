#!/bin/bash

export PATH="/mingw64/bin:/usr/local/bin:/usr/bin:/bin:/c/WINDOWS/system32:/c/WINDOWS:/c/WINDOWS/System32/Wbem:/c/WINDOWS/System32/WindowsPowerShell/v1.0"
bf=$(cygpath ${APPVEYOR_BUILD_FOLDER})
cd "$bf" || (echo "Cannot go to directory $bf"; return 1)

# Clone projects that SimTaDyn depends on.
cd external
./gitclone.sh

# Export display
# Xvfb :1 -screen 0 1024x768x16 -nolisten tcp &
# export DISPLAY=":1.0"

# Build SimTaDyn unit tests.
cd ../tests && make -j4
./build/UnitTest -a

# Build SimTaDyn executable.
cd .. && make -j4
