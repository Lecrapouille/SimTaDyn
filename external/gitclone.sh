#!/bin/bash

# Given by Makefile
if [ "$1" == "" ];
then
  echo "Expected one argument: arhitecture"
  exit 1
fi
ARCHI="$1"

# Clone Backward tool: A beautiful stack trace pretty printer for C++.
# License: MIT
git clone https://github.com/bombela/backward-cpp.git --depth=1

# https://github.com/sebastiandev/zipper
# License: MIT
git clone --recursive https://github.com/sebastiandev/zipper.git --depth=1
mkdir -p zipper/build
cd zipper/build
cmake ../ && make -j4 && make install
cd ..

if [ "$ARCHI" == "Linux" ]; then

    # https://github.com/kbranigan/Simple-OpenGL-Image-Library
    # License: public domain
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1
    cd Simple-OpenGL-Image-Library && make -j4 && make install

elif [ "$ARCHI" == "Darwin" ]; then

    # https://github.com/smibarber/libSOIL
    # License: public domain
    git clone https://github.com/smibarber/libSOIL.git --depth=1
    cd libSOIL && make -j4 && make install

else

    echo "libSOIL for Window: todo"

fi
