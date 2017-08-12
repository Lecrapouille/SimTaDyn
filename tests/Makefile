##=====================================================================
## SimTaDyn: A GIS in a spreadsheet.
## Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
# Sharable informations between all Makefiles
-include ../.makefile/Makefile.header

###################################################
# Executable name
TARGET = UnitTest

###################################################
# Path where to store the generated coverage docs
DOC = ../doc/coverage
# Path where to store lcov rapport
RAPPORT = build/rapport.info

###################################################
# Inform Makefile where to find header files
INCLUDES = -I$(BUILD) -I. -I../external/backward-cpp -I../external/YesEngine\
-I../src -I../src/common/patterns -I../src/common/managers		\
-Icommon/managers -I../src/common/utils -Icommon/utils			\
-I../src/common/maths -Icommon/maths -I../src/common/containers		\
-Icommon/containers -I../src/common/graph-theory			\
-Icommon/graph-theory -I../src/common/graphics/OpenGL			\
-I../src/common/graphics/RTree -I../src/common/graphics			\
-Icommon/graphics/OpenGL -Icommon/graphics/RTree -Icommon/graphics	\
-I../src/core -I../src/core/loaders -I../src/forth -Iforth		\
-I../src/ui

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=.:$(BUILD):../src:../src/common/patterns:\
../src/common/managers:common/managers:\
../src/common/utils:common/utils:\
../src/common/maths:common/maths:\
../src/common/containers:common/containers:\
../src/common/graph-theory:common/graph-theory:\
../src/common/graphics/OpenGL:common/graphics/OpenGL:\
../src/common/graphics/RTree:common/graphics/RTree:\
../src/common/graphics:common/graphics:\
../src/core:../src/core/loaders:\
../src/forth:forth:\
../src/ui:\
../external/backward-cpp:../external/YesEngine

###################################################
# List of files to compile.
OBJ_EXTERNAL   = backward.o
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_UTILS     += FileTests.o
OBJ_PATTERNS   =
OBJ_MATHS      = Maths.o VectorTests.o MatrixTests.o TransformationTests.o
OBJ_MATHS     += BoundingBoxTests.o FilteringTests.o
OBJ_CONTAINERS = PendingData.o SetTests.o CollectionTests.o
OBJ_MANAGERS   = ResourcesTests.o
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_GRAPHS    += BasicArcTests.o BasicNodeTests.o BasicGraphTests.o GraphAlgoTests.o
OBJ_OPENGL     = Color.o Camera2D.o OpenGL.o GLObject.o GLShader.o GLVertexArray.o
OBJ_OPENGL    += GLVertexBuffer.o GLAttrib.o GLTextures.o GLWindow.o Renderer.o
OBJ_OPENGL    += ColorTests.o GLBufferTests.o OpenGLTests.o
# OBJ_RTREE      = RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJ_CORE       = SimForth.o CellForth.o SimTaDynGraph.o SimTaDynMap.o
OBJ_LOADERS    = ILoader.o ShapeFile.o
# SimTaDynFile.o
OBJ_GUI        = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI       += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_UT_SIMTADYN= main.o
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_LOADERS) \
                 $(OBJ_GUI) $(OBJ_UT_SIMTADYN)

###################################################
# Compilation options. For knowing which libraries
# is needed please read the doc/Install.md file.
ifneq ($(MY_CXX),)
CXX = $(MY_CXX)
endif
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
DEFINES = $(SIMTADYN_DEFINES)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the doc/Install.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS = -I/usr/local/include -I/opt/X11/include -L/usr/local/lib	\
-I/opt/X11/lib -framework OpenGL -lglew -lglfw -lSOIL

## Linux
else ifeq ($(ARCHI),Linux)
CXXFLAGS += --coverage
LDFLAGS += --coverage
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread      \
       -lX11 -lGLEW  -lglfw -lSOIL -ldl -ldw

## TODO: Windows
else
$(error Unknown architecture)
endif

###################################################
all: $(TARGET)

$(TARGET): version.h $(OBJ) Makefile ../Makefile ../.makefile/Makefile.header ../.makefile/Makefile.footer
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp version.h
%.o: %.cpp $(BUILD)/%.d version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Generate the rapport of the code coverage
.PHONY: coverage
coverage: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	./build/$(TARGET)
	@$(call print-to,"Documentation","$(RAPPORT)","$(DOC)","")
	@lcov --quiet --directory .. -c -o $(RAPPORT)
	@lcov --quiet --remove $(RAPPORT) '/usr*' -o $(RAPPORT)
	@genhtml --quiet -o $(DOC) -t "SimTaDyn Unit Tests" $(RAPPORT)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) $(DOC) 2> /dev/null

###################################################
# Generate a header file with the project version
version.h: ../VERSION
	@$(call print-from,"Version","$(TARGET)","VERSION","")
	@../.makefile/version.sh ../VERSION $(BUILD)/version.h

###################################################
# Sharable informations between all Makefiles
-include ../.makefile/Makefile.footer
