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
## along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

###################################################
# Executable name
PROJECT = SimTaDyn
TARGET = MyExcel

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Location from the project root directory.
P=../../../..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header
include $(P)/Makefile.common

###################################################
# List of files to compile. Splited by directories
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS = PendingData.o
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o
OBJ_FORTH     += ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_FORTH     += SimTaDynForth.o SpreadSheetPrimitives.o
OBJ_CORE       = ASpreadSheetCell.o ASpreadSheet.o
OBJ_STANDALONE = ClassicSpreadSheet.o main.o
OBJ           += $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_GRAPHS) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_STANDALONE)

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
EXTERNAL_LIBS +=

###################################################
all: $(TARGET)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
