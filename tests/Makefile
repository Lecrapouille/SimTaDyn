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

.PHONY: clean build coverage gcov
-include ../.makefile/Makefile.helper

###################################################
# Path where to store *.o files. Default: ./build/
ifeq ($(BUILD),)
BUILD = build
endif

###################################################
# Path where to store the generated coverage docs
DOC = ../doc/coverage
# Path where to store lcov rapport
RAPPORT = build/rapport.info

###################################################
# Operating system detection
ifeq ($(OS),Windows_NT)
ARCHI := Windows
else
ARCHI := $(shell uname -s)
endif

###################################################
# Set include paths
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
# Path for Makefile to find *.o
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
# Set objects architecture specific
TARGET = UnitTest
OBJECTS = version.h backward.o ILogger.o Logger.o ILoader.o PendingData.o File.o Path.o   \
	FileTests.o ResourcesTests.o Maths.o VectorTests.o MatrixTests.o        \
	TransformationTests.o BoundingBoxTests.o FilteringTests.o	        \
	Color.o ColorTests.o SetTests.o CollectionTests.o Exception.o           \
	OpenGL.o GLObject.o GLShader.o GLVertexArray.o GLAttrib.o               \
	GLTextures.o GLWindow.o Graph.o BasicArcTests.o                         \
	BasicNodeTests.o BasicGraphTests.o GraphAlgorithm.o                     \
	GraphAlgoTests.o GLBufferTests.o OpenGLTests.o main.o

###################################################
# Compil options
# CXX = g++
DEFINES = -DBACKWARD_HAS_DW=1 -DSIMTADYN_DATA_PATH=\"$(PROJECT_DATA_PATH)\"

###################################################
# Set Libraries
ifeq ($(ARCHI),Darwin)
CXXFLAGS = -W -Wall -Wextra -O0 -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
LIBS = -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -framework OpenGL -lglew -lglfw -lSOIL
else ifeq ($(ARCHI),Linux)
CXXFLAGS = -W -Wall -Wextra -O0 -g --coverage -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = --coverage `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread      \
       -lX11 -lGLEW  -lglfw -lSOIL -ldl -ldw
else
$(error Unknown architecture)
endif

###################################################
# Files dependencies
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# Main entry
all: $(TARGET)

$(TARGET): version.h $(OBJECTS)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJECTS) -o $(TARGET) $(LIBS) $(LDFLAGS)

%.o: %.cpp Makefile
%.o: %.cpp $(BUILD)/%.d Makefile
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) --version
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
#
version.h: ../VERSION
	@$(call print-from,"Check version","$(TARGET)","VERSION","")
	@../.makefile/version.sh ../VERSION $(BUILD)/version.h

###################################################
# Clean Target
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ $(DOC) gui/*~ cells/*~ common/*~ $(BUILD) 2> /dev/null

###################################################
# Code coverage
coverage: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	./build/$(TARGET)
	@$(call print-to,"Documentation","$(RAPPORT)","$(DOC)","")
	@lcov --quiet --directory .. -c -o $(RAPPORT)
	@lcov --quiet --remove $(RAPPORT) '/usr*' -o $(RAPPORT)
	@genhtml --quiet -o $(DOC) -t "SimTaDyn Unit Tests" $(RAPPORT)

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
