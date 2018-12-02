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
TARGET = $(PROJECT)-UnitTest

###################################################
# Location from the project root directory.
P=..
ABSP=$(abspath $(P))

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header

###################################################
# List of files to compile.
#OBJ_EXTERNAL       = backward.o
OBJ_UTILS          = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_UTILS_UT      += StrongTypeTests.o FileTests.o PathTests.o LoggerTests.o TerminalColorTests.o
OBJ_MATHS          = Maths.o
OBJ_MATHS_UT       = VectorTests.o MatrixTests.o TransformationTests.o
OBJ_MATHS_UT      += BoundingBoxTests.o FilteringTests.o
OBJ_CONTAINERS     = PendingData.o
OBJ_CONTAINERS_UT  = SetTests.o CollectionTests.o
OBJ_MANAGERS       =
#OBJ_MANAGERS_UT    = ResourcesTests.o
OBJ_GRAPHS         = Graph.o GraphAlgorithm.o
#OBJ_GRAPHS_UT     += BasicArcTests.o BasicNodeTests.o BasicGraphTests.o GraphAlgoTests.o
OBJ_OPENGL         = Color.o Camera2D.o GLException.o OpenGL.o
# Renderer.o
OBJ_OPENGL_UT      = ColorTests.o GLObjectTests.o GLVAOTests.o GLVBOTests.o GLShadersTests.o GLProgramTests.o 
OBJ_FORTH          = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_CORE           = ASpreadSheetCell.o ASpreadSheet.o SimTaDynForth.o SimTaDynSheet.o SimTaDynMap.o
OBJ_CORE_UT        = ClassicSpreadSheet.o ClassicSpreadSheetTests.o
OBJ_LOADERS        = LoaderException.o ShapeFileLoader.o SimTaDynFileLoader.o
#ILoader.o
#OBJ_LOADERS_UT     = SimTaDynFileLoaderTests.o
#OBJ_GUI            = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
#OBJ_GUI           += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_UNIT_TEST      = main.o
OBJ = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_UTILS_UT) $(OBJ_MATHS)	   \
      $(OBJ_MATHS_UT) $(OBJ_CONTAINERS) $(OBJ_CONTAINERS_UT)	   \
      $(OBJ_MANAGERS) $(OBJ_MANAGERS_UT) $(OBJ_GRAPHS)		   \
      $(OBJ_GRAPHS_UT) $(OBJ_OPENGL) $(OBJ_OPENGL_UT) $(OBJ_FORTH) \
      $(OBJ_CORE) $(OBJ_CORE_UT) $(OBJ_LOADERS) $(OBJ_LOADERS_UT)  \
      $(OBJ_GUI) $(OBJ_UNIT_TEST)

###################################################
# Compilation options.
OPTIM_FLAGS = -O2 -g
CXXFLAGS = -W -Wall -Wextra -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit

###################################################
# Inform Makefile where to find header files
INCLUDES += -Icommon/containers -Icommon/graphics -Icommon/graph-theory -Icommon/managers -Icommon/maths -Icommon/utils -Icore/loaders -Icore/spreadsheet -I../src/core/standalone/ClassicSpreadSheet

###################################################
# Inform Makefile where to find *.cpp and *.o files
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
# Project defines
DEFINES += -DCHECK_OPENGL -DARCHI=$(ARCHI)
# Disable ugly gtkmm compilation warnings
DEFINES += -DGTK_SOURCE_H_INSIDE -DGTK_SOURCE_COMPILATION

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS = $(abspath $(P)/external/SOIL/libSOIL.a) \
       $(abspath $(P)/external/zipper/build/libZipper-static.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

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
#ifneq ($(CI),true)
#OPTIM_FLAGS = -O1 -g
#CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
#LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
#LIBS += -static-libstdc++ -static-libasan
#SANITIZER := ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer)
#endif

###################################################
# Compile SimTaDyn unit tests
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile unit tests
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM_FLAGS) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Run unit tests.
.PHONY: unit-tests
unit-tests: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	$(SANITIZER) ./build/$(TARGET) $(TU_OPTIONS) || (cat SimTaDyn.log; return 1)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) $(DOC) 2> /dev/null

###################################################
# Sharable informations between all Makefiles
-include $(M)/Makefile.footer
