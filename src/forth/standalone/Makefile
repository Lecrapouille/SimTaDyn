##=====================================================================
## SimForth: A Forth for SimTaDyn project.
## Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of SimTaDyn.
##
## SimTaDyn is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

###################################################
# Executable name
PROJECT = SimTaDyn
TARGET = SimForth

###################################################
# Debug mode or Release mode
BUILD_TYPE = debug

###################################################
# Location from the project root directory.
P=../../..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header
include $(P)/Makefile.common

###################################################
# List of files to compile. Splited by directories
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_STANDALONE = main.o
OBJS          += $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_FORTH) $(OBJ_STANDALONE)

###################################################
# Compile Standalone forth project
all: $(TARGET)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
