#!/bin/bash

### This script will git clone some libraries that SimTaDyn needs and
### compile them. To avoid pollution, they are not installed into your
### environement. Therefore SimTaDyn Makefiles have to know where to
### find their files (includes and static/shared libraries).

### $1 is given by ../Makefile and refers to the current architecture.
if [ "$1" == "" ]; then
  echo "Expected one argument: arhitecture"
  exit 1
fi
ARCHI="$1"
TARGET=SimTaDyn

### Delete all previous directories to be sure to have and compile
### fresh code source.
rm -fr backward-cpp zipper SOIL 2> /dev/null

function print-clone
{
    echo -e "\033[35m*** Cloning:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

function print-compile
{
    echo -e "\033[35m*** Compiling:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

### Clone Backward tool: A beautiful stack trace pretty printer for C++. No installation
### is needed but some libraries are needed (libdw-dev, binutils-dev, ...).
### License: MIT
print-clone backward-cpp
git clone https://github.com/bombela/backward-cpp.git --depth=1 > /dev/null 2> /dev/null

### Library for opening pictures files (jpeg, png ...)
print-clone SOIL
if [ "$ARCHI" == "Darwin" ]; then

    # License: public domain
    git clone https://github.com/childhood/libSOIL.git --depth=1 > /dev/null 2> /dev/null
    if [ "$?" == "0" ];
    then
        print-compile SOIL
        mv libSOIL SOIL
        cd SOIL
        (make -j4) > /dev/null 2> /dev/null
        cd ..
    fi

else

    # License: public domain
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1 > /dev/null 2> /dev/null
    if [ "$?" == "0" ];
    then
        print-compile SOIL
        mv Simple-OpenGL-Image-Library SOIL
        cd SOIL
        (make -j4) > /dev/null 2> /dev/null
        # Move header and static lib in the same location to be indentical than SOIL for Darwin
        cp src/SOIL.h .
        cp lib/libSOIL.a .
        cd ..
    fi
fi

### Library for manipulating zip files (forked from https://github.com/sebastiandev/zipper)
### License: MIT
print-clone zipper
git clone --recursive https://github.com/Lecrapouille/zipper.git --branch fix_leaks --depth=1 > /dev/null 2> /dev/null
if [ "$?" == "0" ]; then
    print-compile zipper
    mkdir -p zipper/build
    cd zipper/build
    (cmake ..) > /dev/null 2> /dev/null
    (make CXX_FLAGS='-O2' -j4) > /dev/null 2> /dev/null
    cd ../..
fi
