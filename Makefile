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
-include .makefile/Makefile.header

###################################################
# Executable name
TARGET = SimTaDyn

###################################################
# Inform Makefile where to find header files
INCLUDES = -I$(BUILD) -Iexternal/backward-cpp -Iexternal/libSOIL	\
-Iexternal/YesEngine -Isrc -Isrc/common/patterns			\
-Isrc/common/managers -Isrc/common/utils -Isrc/common/maths		\
-Isrc/common/containers -Isrc/common/graph-theory			\
-Isrc/common/graphics/OpenGL -Isrc/common/graphics/RTree		\
-Isrc/common/graphics -Isrc/core -Isrc/core/loaders -Isrc/forth		\
-Isrc/ui

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=$(BUILD):external/backward-cpp:external/YesEngine:external/libSOIL\
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
# List of files to compile. Splited by directories
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
OBJ_LOADERS    = ILoader.o ShapeFile.o SimTaDynFile.o
# SimTaDynFile.o
OBJ_GUI        = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI       += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_SIMTADYN   = main.o
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_LOADERS) \
                 $(OBJ_GUI) $(OBJ_SIMTADYN)

###################################################
# Compilation options. For knowing which libraries
# is needed please read the doc/Install.md file.
ifneq ($(MY_CXX),)
CXX = $(MY_CXX)
endif
CXXFLAGS = -W -Wall -Wextra -O2 -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`
DEFINES = $(SIMTADYN_DEFINES)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the doc/Install.md file.

## OS X: FIXME update this to avoid installing libSOIL
ifeq ($(ARCHI),Darwin)
INCLUDES += -I/opt/local/include -I/usr/local/include -I/opt/X11/include -I/opt/X11/lib
LIBS = -L/opt/local/lib -L/usr/local/lib -framework OpenGL -lglew -lglfw -lSOIL
## Linux
else ifeq ($(ARCHI),Linux)
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11\
       -lGLEW -ldl -ldw -lSOIL
# -lZipper-static -lz

## Window
else

#$(error Unknown architecture)
endif

###################################################
all: $(TARGET)

$(TARGET): Makefile .makefile/Makefile.header .makefile/Makefile.footer version.h $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp version.h
%.o: %.cpp $(BUILD)/%.d version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
ifeq ($(ARCHI),Darwin)
# FIXME OSX
	@mkdir -p $(BUILD)
endif
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# git clone external projects that SimTaDyn needs
.PHONY: clone
clone:
	@cd external && ./gitclone.sh $(ARCHI); cd -

###################################################
# https://scan.coverity.com/
# Coverity Scan: static analysis of code (web service)
# For working, this service needs you download a runnable
# and to compile your code with it. Once done, you have to
# create a tarball of generated files and to upload the
# tarball to the website.
#
# This rule clean and launch the compilation again and
# create the tarball. TODO: upload the tarball to Coverity scan
.PHONY: coverity-scan
coverity-scan: clean $(TARGET)
	@cov-build --dir cov-int make && tar czvf SimTaDyn.tgz cov-int

###################################################
# Call the unit tests makefile (in tests/ directory),
# compile tests, launch them and generate code coverage.
.PHONY: unit-tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@make -C tests coverage

###################################################
# Generate the code source documentation with doxygen.
.PHONY: doc
doc:
	@doxygen Doxyfile

###################################################
# Create Debian/Ubuntu package. Need to be root user
.PHONY: package
package: $(TARGET)
	$(call print-simple,"Creating a Debian/Ubuntu package")
	@./.makefile/package.sh

###################################################
# Compress SimTaDyn sources without its .git, build
# folders and doc generated files. If a tarball
# already exists, it'll not be smashed.
.PHONY: targz
targz:
	@./.makefile/targz.sh . $(PWD)

###################################################
# Install project. You need to be root user.
.PHONY: install
install: $(TARGET)
	@$(call print-to,"Installing","data/","$(PROJECT_DATA_PATH)","")
	@rm -fr $(PROJECT_DATA_PATH)
	@mkdir -p $(PROJECT_DATA_PATH)/forth
	@cp -r data/* $(PROJECT_DATA_PATH)
	@cp -r src/forth/core/system.fs $(PROJECT_DATA_PATH)/forth
	@$(call print-to,"Installing",$(TARGET),"$(BINDIR)","")
	@mkdir -p $(BINDIR)
	@cp $(BUILD)/$(TARGET) $(BINDIR)/$(TARGET)

###################################################
# Uninstall project. You need to be root user.
.PHONY: uninstall
uninstall:
	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
	@rm -f $(PREFIX)/$(TARGET)
	@rm -fr $(PROJECT_DATA_ROOT)
#	@echo "$(CLR_GREEN)*** You can remove manually your personal SimTaDyn folder located at $(PROJECT_DATA_ROOT)$(CLR_DEFAULT)"

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) 2> /dev/null

###################################################
# Cleaning
.PHONY: distclean
distclean: clean
	@rm -fr cov-int SimTaDyn.tgz *.log foo 2> /dev/null
	@cd tests && make -s clean; cd - > /dev/null
	@cd src/common/graphics/OpenGL/examples/ && make -s clean; cd - > /dev/null
	@cd src/forth/standalone && make -s clean; cd - > /dev/null

###################################################
# Generate a header file with the project version
version.h: VERSION
	@$(call print-from,"Version","$(TARGET)","VERSION","")
	@./.makefile/version.sh VERSION $(BUILD)/version.h

###################################################
# Sharable informations between all Makefiles
-include .makefile/Makefile.footer
