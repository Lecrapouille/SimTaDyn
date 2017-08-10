#!/bin/sh -e

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