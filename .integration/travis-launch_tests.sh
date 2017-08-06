#!/bin/sh -e

# Clone projects that SimTaDyn depends on.
cd external
./gitclone.sh

# Build SimTaDyn unit tests and disable
# OpenGL tests (not supported by Travis).
cd ../tests && make -j4
./build/UnitTest -a

# Build SimTaDyn executable.
cd .. && make -j4
