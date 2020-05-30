#!/bin/bash

### This script will git clone some needed libraries for the project.

### $1 is given by ../Makefile and refers to the current architecture.
if [ "$1" == "" ]; then
  echo "Expected one argument. Select the architecture: Linux, Darwin or Windows"
  exit 1
fi
ARCHI=$1
TARGET=$2

### Delete all previous directories to be sure to have and compile
### fresh code source.
rm -fr SimForth MyLogger Exception TerminalColor backward-cpp zipper SOIL CmdParser 2> /dev/null

function print-clone
{
    echo -e "\033[35m*** Downloading:\033[00m \033[36m$TARGET\033[00m <= \033[33m$1\033[00m"
}

### Clone Backward tool: A beautiful stack trace pretty printer for C++. No installation
### is needed but some libraries are needed (libdw-dev, binutils-dev, ...).
### License: MIT
print-clone backward-cpp
git clone https://github.com/Lecrapouille/backward-cpp.git --depth=1 > /dev/null && touch backward-cpp/.downloaded

### Library CmdParser is a C++ alternative to C getopt for parsing options like --help
print-clone CmdParser
git clone https://github.com/FlorianRappl/CmdParser.git --depth=1 > /dev/null && touch CmdParser/.downloaded

### TerminalColor
print-clone TerminalColor
git clone https://github.com/Lecrapouille/TerminalColor --depth=1 > /dev/null && touch TerminalColor/.downloaded

### MyLogger
print-clone MyLogger
git clone https://github.com/Lecrapouille/MyLogger --depth=1 > /dev/null && touch MyLogger/.downloaded

### Exception
print-clone Exception
git clone https://github.com/Lecrapouille/Exception --depth=1 > /dev/null && touch Exception/.downloaded

### SimForth
print-clone SimForth
git clone https://github.com/Lecrapouille/SimForth --depth=1 > /dev/null && touch SimForth/.downloaded

### OpenGLCppWrapper
print-clone OpenGLCppWrapper
git clone https://github.com/Lecrapouille/OpenGLCppWrapper --depth=1 > /dev/null && touch OpenGLCppWrapper/.downloaded

### Library SOIL for opening pictures files (jpeg, png ...)
print-clone SOIL
if [ "$ARCHI" == "Darwin" ];
then

    # License: public domain
    git clone https://github.com/childhood/libSOIL.git --depth=1 > /dev/null 2> /dev/null
    mv libSOIL SOIL

else

    # License: public domain
    git clone https://github.com/kbranigan/Simple-OpenGL-Image-Library.git --depth=1 > /dev/null 2> /dev/null
    mv Simple-OpenGL-Image-Library SOIL

fi

### Library for manipulating zip files (forked from https://github.com/sebastiandev/zipper)
### License: MIT
print-clone zipper
git clone --recursive https://github.com/Lecrapouille/zipper.git --branch fix_leaks --depth=1 > /dev/null 2> /dev/null

### Library for creating GUI in OpenGL
### License: MIT
print-clone imgui
git clone https://github.com/ocornut/imgui.git --depth=1 > /dev/null 2> /dev/null
cp imgui/examples/imgui_impl_glfw.cpp imgui/imgui_impl_glfw.cpp
cp imgui/examples/imgui_impl_glfw.h imgui/imgui_impl_glfw.h
cp imgui/examples/imgui_impl_opengl3.cpp imgui/imgui_impl_opengl3.cpp
cp imgui/examples/imgui_impl_opengl3.h imgui/imgui_impl_opengl3.h

### Makefile synchronization
touch .downloaded
