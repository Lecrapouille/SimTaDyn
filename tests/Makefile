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
P=..
M=$(P)/.makefile

###################################################
# Project definition
#
TARGET = $(PROJECT)-UnitTest
DESCRIPTION = Unit tests for $(PROJECT)
BUILD_TYPE = debug

include $(P)/Makefile.common

###################################################
# List of files to compile.
#
OBJ_EXTERNAL      +=
OBJ_UTILS          = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_UTILS_UT      += StrongTypeTests.o FileTests.o PathTests.o LoggerTests.o TerminalColorTests.o
OBJ_MATHS          = Maths.o
OBJ_MATHS_UT       = VectorTests.o MatrixTests.o TransformationTests.o
OBJ_MATHS_UT      += BoundingBoxTests.o FilteringTests.o
OBJ_CONTAINERS     =
OBJ_CONTAINERS_UT  = SetTests.o CollectionTests.o
OBJ_MANAGERS       =
OBJ_MANAGERS_UT    = ResourcesTests.o
#LoadersTests.o
OBJ_GRAPHS         = Graph.o GraphAlgorithm.o
#OBJ_GRAPHS_UT     += BasicArcTests.o BasicNodeTests.o BasicGraphTests.o GraphAlgoTests.o
OBJ_OPENGL         = Color.o Camera2D.o OpenGL.o
# Renderer.o
OBJ_OPENGL_UT      = ColorTests.o GLObjectTests.o GLVAOTests.o GLVBOTests.o GLShadersTests.o GLProgramTests.o
OBJ_FORTH          = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_CORE           = ASpreadSheetCell.o ASpreadSheet.o SimTaDynForth.o SimTaDynSheet.o SimTaDynMap.o
OBJ_CORE_UT        = SpreadSheetPrimitives.o ClassicSpreadSheet.o ClassicSpreadSheetTests.o
OBJ_LOADERS        = ManagerException.o ShapeFileLoader.o SimTaDynFileLoader.o
OBJ_LOADERS_UT     = SimTaDynFileLoaderTests.o
#OBJ_GUI            = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
#OBJ_GUI           += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_UNIT_TEST      = main.o
OBJS += $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_UTILS_UT) $(OBJ_MATHS)	\
        $(OBJ_MATHS_UT) $(OBJ_CONTAINERS) $(OBJ_CONTAINERS_UT)		\
        $(OBJ_MANAGERS) $(OBJ_MANAGERS_UT) $(OBJ_GRAPHS)			\
        $(OBJ_GRAPHS_UT) $(OBJ_OPENGL) $(OBJ_OPENGL_UT) $(OBJ_FORTH)	\
        $(OBJ_CORE) $(OBJ_CORE_UT) $(OBJ_LOADERS) $(OBJ_LOADERS_UT)	\
        $(OBJ_GUI) $(OBJ_UNIT_TEST)

###################################################
# Compilation options.
#
LINKER_FLAGS += -lcppunit -pthread

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
#
COMPIL_FLAGS += --coverage
LINKER_FLAGS += --coverage

###################################################
# Inform Makefile where to find header files
#
INCLUDES += -Icommon/containers -Icommon/graphics			\
-Icommon/graph-theory -Icommon/managers -Icommon/maths -Icommon/utils	\
-Icore/loaders -Icore/spreadsheet					\
-I$(P)/src/core/standalone/ClassicSpreadSheet

###################################################
# Inform Makefile where to find *.cpp and *.o files
#
VPATH += common/containers:\
common/graphics:\
common/graph-theory:\
common/managers:\
common/maths:\
common/utils:\
core/loaders:\
core/spreadsheet:\
../src/core/standalone/ClassicSpreadSheet

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
#
THIRDPART_LIBS += \
	$(abspath $(THIRDPART)/SOIL/libSOIL.a) \
	$(abspath $(THIRDPART)/zipper/build/libZipper-static.a)

###################################################
# Compile SimTaDyn unit tests
all: $(TARGET)

###################################################
# Run unit tests.
.PHONY: unit-tests
.PHONY: check
unit-tests check: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./$(BUILD)/$(TARGET) $(TU_OPTIONS) || (cat $(TARGET).log; return 1)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
