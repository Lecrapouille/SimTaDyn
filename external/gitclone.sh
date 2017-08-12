#!/bin/bash

# Clone Backward tool: A beautiful stack trace pretty printer for C++.
# License: MIT
git clone https://github.com/bombela/backward-cpp.git --depth=1


# https://github.com/littlstar/soil
#git clone https://github.com/littlstar/soil.git --depth=1
#cd soil && make -j4 && make install

# https://github.com/kbranigan/Simple-OpenGL-Image-Library
git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1
cd Simple-OpenGL-Image-Library && make -j4 && make install
