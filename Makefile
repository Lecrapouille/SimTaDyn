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
TARGET = $(PROJECT)

###################################################
# Location from the project root directory.
P=.

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
-include $(M)/Makefile.header

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# List of files to compile. Splited by directories
ifeq ($(PROJECT_MODE),debug)
OBJ_EXTERNAL   = backward.o
else
OBJ_EXTERNAL   =
endif
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_PATTERNS   =
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS = PendingData.o
OBJ_MANAGERS   =
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_OPENGL     = Color.o Camera2D.o GLException.o OpenGL.o Renderer.o
# OBJ_RTREE      = RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_CORE       = ASpreadSheetCell.o ASpreadSheet.o SimTaDynForth.o SimTaDynSheet.o SimTaDynMap.o
OBJ_LOADERS    = LoaderException.o SimTaDynLoaders.o ShapeFileLoader.o SimTaDynFileLoader.o
# TextureFileLoader.o
# SimTaDynFile.o
OBJ_GUI        = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI       += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_SIMTADYN   = SimTaDyn.o
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_LOADERS) \
                 $(OBJ_GUI) $(OBJ_SIMTADYN)

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`

###################################################
#
INCLUDES += -I$(P)/src
VPATH += $(P)/src:

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL
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
-ldl -lz -lstdc++

## Windows
else

#$(error Unknown architecture)
endif

###################################################
# Address sanitizer. Uncomment these lines if asan
# is desired.
##OPTIM = -O1 -g
##CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
##LDFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
##LIBS += -static-libstdc++ -static-libasan
##SANITIZER := ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=$(shell which llvm-symbolizer)

###################################################
# Backward allows tracing stack when segfault happens
ifeq ($(PROJECT_MODE),debug)
ifneq ($(ARCHI),Darwin)
OPTIM_FLAGS = -O2 -g
DEFINES += -DBACKWARD_HAS_DW=1
LIBS += -ldw
endif
else
OPTIM_FLAGS = -O3
endif

###################################################
#
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(OPTIM_FLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Download external projects that SimTaDyn needs.
.PHONY: download-external-libs
download-external-libs:
	@cd external && ./download-external-libs.sh $(ARCHI); cd .. > /dev/null 2> /dev/null

###################################################
# Compile external projects that SimTaDyn needs.
.PHONY: compile-external-libs
compile-external-libs:
	@cd external && ./compile-external-libs.sh $(ARCHI); cd .. > /dev/null 2> /dev/null

###################################################
# https://scan.coverity.com/
# Coverity Scan: static analysis of code (web service)
# For working, this service needs you download a runnable
# and to compile your code with it. Once done, you have to
# create a tarball of generated files and to upload the
# tarball to the website.
#
# Compile again the project for Coverity Scan. An uploadable tarball is created.
.PHONY: coverity-scan
coverity-scan: clean
	@rm -fr SimTaDyn.tgz cov-int 2> /dev/null
	@cov-build --dir cov-int make -j8 && tar czvf SimTaDyn.tgz cov-int

###################################################
# Compile and launch unit tests. Then generate the html code coverage.
.PHONY: unit-tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@make -C tests coverage

###################################################
# Launch the executable with address sanitizer (if enabled).
.PHONY: run
run: $(TARGET)
	$(SANITIZER) ./build/$(TARGET) 2>&1 | ./external/asan_symbolize.py

###################################################
# Generate the code source documentation with doxygen.
.PHONY: doc
doc:
	@doxygen Doxyfile
	@xdg-open doc/html/index.html >/dev/null

###################################################
# Compress SimTaDyn sources without its .git, build
# folders and doc generated files. If a tarball
# already exists, the older will stay intact and a
# new one is created. Tarball confict names is managed.
#
# Compress the code source into a unique tarball for backup.
.PHONY: targz
targz:
	@./.makefile/targz.sh $(PWD)

###################################################
# Create an uploadable tarball for the OpenSuse Build Service (OBS).
.PHONY: obs
obs:
	@./.integration/opensuse-build-service.sh

###################################################
# Install project. You need to be root user.
.PHONY: install
install: $(TARGET)
	@$(call print-to,"Installing","data","$(PROJECT_DATA_PATH)","")
	@rm -fr $(PROJECT_DATA_PATH)
	@mkdir -p $(PROJECT_DATA_PATH)/forth
	@cp -r src/forth/core/system.fs $(PROJECT_DATA_PATH)/forth
	@cp -r src/forth/core/LibC $(PROJECT_DATA_PATH)/forth
	@cp -r data/* $(PROJECT_DATA_PATH)
	@$(call print-to,"Installing","doc","$(PROJECT_DOC_PATH)","")
	@mkdir -p $(PROJECT_DOC_PATH)
	@cp -r doc/* $(PROJECT_DATA_ROOT)/doc
	@cp AUTHORS LICENSE README.md ChangeLog $(PROJECT_DATA_ROOT)
	@$(call print-to,"Installing","$(BUILD)/$(TARGET)","$(PROJECT_EXE)","")
	@mkdir -p $(BINDIR)
	@cp $(BUILD)/$(TARGET) $(PROJECT_EXE)

###################################################
# Uninstall the project. You need to be root.
.PHONY: uninstall
uninstall:
	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
	@rm $(PROJECT_EXE)
	@rm -r $(PROJECT_DATA_ROOT)

###################################################
# Clean the build/ directory.
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr $(BUILD) 2> /dev/null

###################################################
# Clean the whole project.
.PHONY: veryclean
veryclean: clean
	@rm -fr cov-int SimTaDyn.tgz *.log foo 2> /dev/null
	@cd tests && make -s clean; cd - > /dev/null
	@cd src/common/graphics/OpenGL/examples/ && make -s clean; cd - > /dev/null
	@cd src/forth/standalone && make -s clean; cd - > /dev/null
	@cd src/core/standalone/ClassicSpreadSheet && make -s clean; cd - > /dev/null
	@$(call print-simple,"Cleaning","$(PWD)/doc/html")
	@cd doc/ && rm -fr html

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.help
include $(M)/Makefile.footer
