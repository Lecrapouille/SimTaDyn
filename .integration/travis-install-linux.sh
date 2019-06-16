#!/bin/bash

apt-get update

# Global packages
apt-get install -y git cmake g++ clang

# SimTaDyn dependencies
apt-get install -y libgtksourceviewmm-3.0-dev freeglut3 freeglut3-dev mesa-common-dev libglu1-mesa-dev libglew-dev libglfw3-dev libdw-dev liblzma-dev bc

# Unit tests dependencies
apt-get install -y libcppunit-dev libglm-dev llvm-4.0-tools
ln -s /usr/bin/llvm-symbolizer-4.0 /usr/bin/llvm-symbolizer

# coveralls dependencies
apt-get install -y gcovr lcov ruby-dev
gem install coveralls-lcov
