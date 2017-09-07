#!/bin/bash

# Given by Makefile
if [ "$1" == "" ]; then
  echo "Expected one argument: arhitecture"
  exit 1
fi
ARCHI="$1"

# Clone Backward tool: A beautiful stack trace pretty printer for C++.
# License: MIT
rm -fr backward-cpp 2> /dev/null
git clone https://github.com/bombela/backward-cpp.git --depth=1

# https://github.com/sebastiandev/zipper
# License: MIT
rm -fr zipper 2> /dev/null
git clone --recursive https://github.com/sebastiandev/zipper.git --depth=1
if [ "$?" == "0" ]; then
    mkdir -p zipper/build
    cd zipper/build
    cmake ../ && make -j4 && make install
    cd ..
fi

rm -fr libSOIL 2> /dev/null
if [ "$ARCHI" == "Linux" ]; then

    # https://github.com/kbranigan/Simple-OpenGL-Image-Library
    # License: public domain
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1
    if [ "$?" == "0" ];
    then
	mv Simple-OpenGL-Image-Library libSOIL
        cd libSOIL && make -j4 && make install
    fi

elif [ "$ARCHI" == "Darwin" ]; then

    # https://github.com/childhood/libSOIL
    # License: public domain
    rm -fr zipper 2> /dev/null
    git clone https://github.com/childhood/libSOIL.git --depth=1
    if [ "$?" == "0" ];
    then
	cd libSOIL && make -j4 && make install
    fi

else

    echo "libSOIL for Window: todo"
    exit 1

fi
