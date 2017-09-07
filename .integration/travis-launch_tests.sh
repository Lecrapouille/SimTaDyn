#!/bin/bash

# Clone projects that SimTaDyn depends on.
make clone || exit 1

# Export display
# Xvfb :1 -screen 0 1024x768x16 -nolisten tcp &
# export DISPLAY=":1.0"

# Build SimTaDyn unit tests.
(cd tests && make -j4 && ./build/UnitTest -a) || exit 1

# Build SimTaDyn executable.
(cd .. && make -j4) || exit 1
