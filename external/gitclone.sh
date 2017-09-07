#!/bin/bash

# Given by Makefile
if [ "$1" == "" ]; then
  echo "Expected one argument: arhitecture"
  exit 1
fi
ARCHI="$1"
TARGET=SimTaDyn

rm -fr backward-cpp zipper SOIL 2> /dev/null

function print-clone
{
    echo -e "\033[35m*** Cloning:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

function print-compile
{
    echo -e "\033[35m*** Compiling:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

# Clone Backward tool: A beautiful stack trace pretty printer for C++.
# License: MIT
print-clone backward-cpp
git clone https://github.com/bombela/backward-cpp.git --depth=1 > /dev/null 2> /dev/null

print-clone SOIL
if [ "$ARCHI" == "Darwin" ]; then

    # https://github.com/childhood/libSOIL
    # License: public domain
    git clone https://github.com/childhood/libSOIL.git --depth=1 > /dev/null 2> /dev/null
    if [ "$?" == "0" ];
    then
	print-compile SOIL
	mv libSOIL SOIL
	cd SOIL
	make -j4
	cd ..
    fi
 
else

    # https://github.com/kbranigan/Simple-OpenGL-Image-Library
    # License: public domain
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1 > /dev/null 2> /dev/null
    if [ "$?" == "0" ];
    then
	print-compile SOIL
	mv Simple-OpenGL-Image-Library SOIL
	cd SOIL
	make -j4
	# Move header and static lib in the same location to be indentical than SOIL for Darwin
	cp src/SOIL.h .
	cp lib/libSOIL.a .
	cd ..
    fi
fi

# https://github.com/sebastiandev/zipper
# License: MIT
print-clone zipper
git clone --recursive https://github.com/sebastiandev/zipper.git --depth=1 > /dev/null 2> /dev/null
if [ "$?" == "0" ]; then
    print-compile zipper
    mkdir -p zipper/build
    cd zipper/build
    if [ "$ARCHI" == "Windows" ]; then
	export ZLIBROOT="/mingw64"
	export LIBZ_LIBRARY="/mingw64/lib"
	cmake -DLIBZ_LIBRARY="/mingw64/lib" ..
    else
	cmake .. 
    fi 
    make
    cd ../..
fi
