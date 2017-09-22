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
INCLUDES = -I$(BUILD) -I. -I../external/SOIL -I../external/zipper	\
-I../external/backward-cpp -I../external/YesEngine -I../src		\
-I../src/common/patterns -I../src/common/managers -Icommon/managers	\
-I../src/common/utils -Icommon/utils -I../src/common/maths		\
-Icommon/maths -I../src/common/containers -Icommon/containers		\
-I../src/common/graph-theory -Icommon/graph-theory			\
-I../src/common/graphics/OpenGL -I../src/common/graphics/RTree		\
-I../src/common/graphics -Icommon/graphics/OpenGL			\
-Icommon/graphics/RTree -Icommon/graphics -I../src/core			\
-I../src/core/loaders -Icore/loaders -I../src/forth -Iforth		\
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
../src/core:../src/core/loaders:core/loaders:\
../src/forth:forth:\
../src/ui:\
../external/SOIL:../external/backward-cpp:../external/YesEngine

###################################################
# List of files to compile.
OBJ_EXTERNAL       = backward.o
OBJ_UTILS          = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_UTILS_UT      += LoggerTests.o FileTests.o
OBJ_MATHS          = Maths.o
OBJ_MATHS_UT       = VectorTests.o MatrixTests.o TransformationTests.o
OBJ_MATHS_UT      += BoundingBoxTests.o FilteringTests.o
OBJ_CONTAINERS     = PendingData.o
OBJ_CONTAINERS_UT  = SetTests.o CollectionTests.o
OBJ_MANAGERS       =
OBJ_MANAGERS_UT    = ResourcesTests.o
OBJ_GRAPHS         = Graph.o GraphAlgorithm.o
OBJ_GRAPHS_UT     += BasicArcTests.o BasicNodeTests.o BasicGraphTests.o GraphAlgoTests.o
OBJ_OPENGL         = Color.o Camera2D.o OpenGLException.o OpenGL.o GLObject.o GLShader.o GLVertexArray.o
OBJ_OPENGL        += GLVertexBuffer.o GLAttrib.o GLTextures.o GLWindow.o Renderer.o
OBJ_OPENGL_UT      = ColorTests.o GLBufferTests.o OpenGLTests.o
OBJ_FORTH          = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJ_CORE           = SimForth.o CellForth.o SimTaDynGraph.o SimTaDynMap.o
OBJ_LOADERS        = LoaderException.o ILoader.o ShapeFileLoader.o SimTaDynFileLoader.o
OBJ_LOADERS_UT     = SimTaDynFileLoaderTests.o
OBJ_GUI            = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI           += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_UNIT_TEST      = main.o
OBJ = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_UTILS_UT) $(OBJ_MATHS)		\
      $(OBJ_MATHS_UT) $(OBJ_CONTAINERS) $(OBJ_CONTAINERS_UT)		\
      $(OBJ_MANAGERS) $(OBJ_MANAGERS_UT) $(OBJ_GRAPHS)			\
      $(OBJ_GRAPHS_UT) $(OBJ_OPENGL) $(OBJ_OPENGL_UT) $(OBJ_FORTH)	\
      $(OBJ_CORE) $(OBJ_LOADERS) $(OBJ_LOADERS_UT) $(OBJ_GUI)		\
      $(OBJ_UNIT_TEST)

###################################################
# Compilation options. For knowing which libraries
# is needed please read the doc/Install.md file.
OPTIM = -O2
CXXFLAGS = -W -Wall -Wextra -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
DEFINES = $(SIMTADYN_DEFINES)

###################################################
# Set Libraries compiled in the external-l directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS = $(abspath $(PWD)/../external/SOIL/libSOIL.a) $(abspath $(PWD)/../external/zipper/build/libZipper-static.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the doc/Install.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw -lz

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0 -latk-1.0		\
-lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0		\
-lgthread-2.0 -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11 -lGLEW	\
-lglfw -ldl -ldw -lz

## Windows
else
#$(error Unknown architecture)
endif

# if inside Docker with Travis-CI, OpenGL context cannot be created.
# So disable some tests.
ifeq ($(CI),true)
TU_OPTIONS=-a
endif

###################################################
# Code coverage. Comment these lines if coveraging
# is not desired.
CXXFLAGS += --coverage
LDFLAGS += --coverage

###################################################
# Address sanitizer. Comment these lines if asan is
# not desired.

# FIXME: clang on Travis-CI bugs during linking (infinite loop).
# FIXME: g++ on Tracis-CI I have a segfault and trace are not human
# readable. Temporary disable asan with Travis

# ifeq ($(CXX),g++)
ifneq ($(CI),true)
OPTIM = -O1 -g
CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LIBS += -static-libstdc++ -static-libasan
SANITIZER := ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer)
endif

###################################################
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile ../Makefile ../.makefile/Makefile.header ../.makefile/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Run unit tests.
.PHONY: unit-tests
unit-tests: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./build/$(TARGET) $(TU_OPTIONS) || (cat SimTaDyn.log; return 1)

###################################################
# Generate the rapport of the code coverage.
# Contrary to the unit-tests rule, if unit tests
# failed, clear the $? to let lcov to be launched.
.PHONY: coverage
coverage: unit-tests
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./build/$(TARGET) $(TU_OPTIONS) || cat SimTaDyn.log
	@$(call print-to,"Documentation","$(RAPPORT)","$(DOC)","")
	@lcov --quiet --directory .. -c -o $(RAPPORT)
	@lcov --quiet --remove $(RAPPORT) '/usr*' 'external/*' 'tests/*' -o $(RAPPORT)
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
