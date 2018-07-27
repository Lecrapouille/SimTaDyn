#!/bin/sh -e

# SimTaDyn unit tests
cd tests && make -j4
./build/UnitTest -a

# SimTaDyn executable
cd ../src && make -j4
