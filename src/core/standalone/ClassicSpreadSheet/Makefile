##=====================================================================
## SimTaDyn: A GIS in a spreadsheet.
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
# Location of the project directory and Makefiles
#
P=../../../..
M=$(P)/.makefile

###################################################
# Project definition
#
TARGET = MyExcel
DESCRIPTION = Simple spreadsheet solver
BUILD_TYPE = debug

include $(P)/Makefile.common

###################################################
# List of files to compile.
#
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS =
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o
OBJ_FORTH     += ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_FORTH     += SimTaDynForth.o SpreadSheetPrimitives.o
OBJ_CORE       = ASpreadSheetCell.o ASpreadSheet.o
OBJ_STANDALONE = ClassicSpreadSheet.o main.o
OBJS          += $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_GRAPHS) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_STANDALONE)

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
#
THIRDPART_LIBS +=

###################################################
# Compile the binary
all: $(TARGET)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
