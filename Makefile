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

.PHONY: clean very-clean build install uninstall coverity-scan unit-tests package coverage
-include .makefile/Makefile.helper

###################################################
# Path where to store *.o files. Default: ./build/
ifeq ($(BUILD),)
BUILD = ./build
endif

###################################################
# Operating system detection
ifeq ($(OS),Windows_NT)
ARCHI := Windows
else
ARCHI := $(shell uname -s)
endif

###################################################
# Set include paths
INCLUDES = -I$(BUILD) -Iexternal/backward-cpp -Iexternal/YesEngine -Isrc	\
-Isrc/common/patterns -Isrc/common/managers -Isrc/common/utils		\
-Isrc/common/maths -Isrc/common/containers -Isrc/common/graph-theory	\
-Isrc/common/graphics/OpenGL -Isrc/common/graphics/RTree		\
-Isrc/common/graphics -Isrc/core -Isrc/core/loaders -Isrc/forth		\
-Isrc/ui

###################################################
# Path for Makefile to find *.o
VPATH=$(BUILD):external/backward-cpp:external/YesEngine:\
src:\
src/common/patterns:\
src/common/managers:\
src/common/utils:\
src/common/maths:\
src/common/containers:\
src/common/graph-theory:\
src/common/graphics:\
src/common/graphics/OpenGL:\
src/common/graphics/RTree:\
src/core:\
src/core/loaders:\
src/forth:\
src/ui

###################################################
OBJ_EXTERNAL   = backward.o
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_PATTERNS   =
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS = PendingData.o
OBJ_MANAGERS   =
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_OPENGL     = Color.o Camera2D.o OpenGL.o GLObject.o GLShader.o GLVertexArray.o
OBJ_OPENGL    += GLVertexBuffer.o GLAttrib.o GLTextures.o Renderer.o
# OBJ_RTREE      = RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJ_CORE       = SimForth.o CellForth.o SimTaDynGraph.o SimTaDynMap.o
OBJ_LOADERS    = ILoader.o ShapeFile.o
# SimTaDynFile.o
OBJ_GUI        = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI       += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_SIMTADYN   = main.o

###################################################
TARGET         = SimTaDyn
TARGET_TGZ     = SimTaDyn-$(DATE).tar.gz
OBJ            = version.h $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_LOADERS) \
                 $(OBJ_GUI) $(OBJ_SIMTADYN)

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O0 -g -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`
DEFINES = -DBACKWARD_HAS_DW=1 -DCHECK_OPENGL -DSIMTADYN_DATA_PATH=\"$(PROJECT_DATA_PATH)\"

###################################################
# Set Libraries

ifeq ($(ARCHI),Darwin)
LIBS = -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -framework OpenGL -lglew
else ifeq ($(ARCHI),Linux)
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11\
       -lGLEW -ldl -ldw -lSOIL
# -lZipper-static -lz
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

$(TARGET): version.h $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

%.o: %.cpp
%.o: %.cpp $(BUILD)/%.d
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
ifeq ($(ARCHI),Darwin)
	@mkdir -p $(BUILD)
endif
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Coverity Scan: static analysis of code
# Prepare the tarball ready to be uploaded to Coverity Scan servers.
coverity-scan: $(TARGET)
	@cov-build --dir cov-int make && tar czvf SimTaDyn.tgz cov-int

###################################################
# Compile unit tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@make -C tests all

###################################################
#
coverage:
	@$(call print-simple,"Coveraging unit tests")
	@make -C tests coverage

###################################################
# Create Debian/Ubuntu package
package: $(TARGET)
	$(call print-simple,"Creating a Debian/Ubuntu package")
	./.makefile/package.sh

###################################################
# Compress SimTaDyn sources
targz: very-clean
	@$(call print-from,"Tarball","$(TARGET_TGZ)","$(PWD)","")
	@tar czvf /tmp/$(TARGET_TGZ) . > /dev/null && mv /tmp/$(TARGET_TGZ) .

###################################################
# Clean Target
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) 2> /dev/null

###################################################
# Clean Target
very-clean: clean
	@rm -fr cov-int SimTaDyn.tgz 2> /dev/null
	@cd tests && make -s clean; cd - > /dev/null

###################################################
# Install project
install: $(TARGET)
	@$(call print-to,"Installing","data/","$(PROJECT_DATA_PATH)","")
	@rm -fr $(PROJECT_DATA_PATH)
	@mkdir -p $(PROJECT_DATA_PATH)/forth
	@cp -r data/* $(PROJECT_DATA_PATH)
	@cp -r src/forth/core/system.fs $(PROJECT_DATA_PATH)/forth
	@$(call print-to,"Installing",$(TARGET),"$(PREFIX)","")
	@cp $(BUILD)/$(TARGET) $(PREFIX)

###################################################
# Uninstall project
uninstall:
	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
	@rm -f $(PREFIX)/$(TARGET)
	@rm -fr $(PROJECT_DATA_ROOT)
#	@echo "$(CLR_GREEN)*** You can remove manually your personal SimTaDyn folder located at $(PROJECT_DATA_ROOT)$(CLR_DEFAULT)"

###################################################
# Generate a header file with the project version
version.h: VERSION
	@$(call print-from,"Check version","$(TARGET)","VERSION","")
	@./.makefile/version.sh VERSION $(BUILD)/version.h

###################################################
# Create a temporary folder to store *.o and *.d files
$(DEPFILES): | $(BUILD)
$(OBJ): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)

###################################################
# Auto-Dependency Generation
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJ)))
