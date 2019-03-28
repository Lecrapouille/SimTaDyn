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
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Location from the project root directory.
P=.

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header

###################################################
# Make the list of compiled files
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_PATTERNS   =
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS = PendingData.o
OBJ_MANAGERS   =
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_OPENGL     = Color.o Camera2D.o GLException.o OpenGL.o NodesRenderer.o Renderer.o
# OBJ_RTREE      = RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_CORE       = ASpreadSheetCell.o ASpreadSheet.o SimTaDynForth.o SimTaDynForthPrimitives.o SimTaDynSheet.o SimTaDynMap.o
OBJ_LOADERS    = ManagerException.o SimTaDynLoaders.o ShapeFileLoader.o SimTaDynFileLoader.o
# TextureFileLoader.o
# SimTaDynFile.o
OBJ_GUI        = DialogException.o Redirection.o SimTaDynMapExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI       += ForthInspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_SIMTADYN   = SimTaDyn.o
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_LOADERS) \
                 $(OBJ_GUI) $(OBJ_SIMTADYN)

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`

###################################################
# Inform Makefile where to find header files
INCLUDES += -I$(P)/external -I$(P)/external/SOIL			\
-I$(P)/external/YesEngine -I$(P)/external/zipper			\
-I$(P)/src/common/spreadsheet -I$(P)/src/common/patterns		\
-I$(P)/src/common/managers -I$(P)/src/common/utils			\
-I$(P)/src/common/maths -I$(P)/src/common/containers			\
-I$(P)/src/common/graph-theory -I$(P)/src/common/graphics/OpenGL	\
-I$(P)/src/common/graphics/OpenGL/ -I$(P)/src/common/graphics/RTree	\
-I$(P)/src/common/graphics -I$(P)/src/core -I$(P)/src/core/loaders	\
-I$(P)/src/forth -I$(P)/src/ui -I$(P)/src

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH += $(P)/external/YesEngine:\
$(P)/external/SOIL:$(P)/src/common/spreadsheet:\
$(P)/src/common/patterns:$(P)/src/common/managers:\
$(P)/src/common/utils:$(P)/src/common/maths:\
$(P)/src/common/containers:$(P)/src/common/graph-theory:\
$(P)/src/common/graphics:$(P)/src/common/graphics/OpenGL:\
$(P)/src/common/graphics/RTree:$(P)/src/core:$(P)/src/core/loaders:\
$(P)/src/forth:$(P)/src/ui:$(P)/src

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL
# Disable ugly gtkmm compilation warnings
DEFINES += -DGTK_SOURCE_H_INSIDE -DGTK_SOURCE_COMPILATION
#
DEFINES += -DPROJECT_TEMP_DIR=\"/tmp/$(TARGET)/\"
DEFINES += -DPROJECT_DATA_PATH=\"$(PWD)/data:$(PROJECT_DATA_PATH)\"

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
EXTERNAL_LIBS = \
	$(abspath $(P)/external/SOIL/libSOIL.a) \
	$(abspath $(P)/external/zipper/build/libZipper-static.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

ifeq ($(ARCHI),Darwin)
EXTERNAL_LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw -lz
else ifeq ($(ARCHI),Linux)
EXTERNAL_LIBS += -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0		\
-latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0	\
-lgthread-2.0 -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11 -lGLEW	\
-ldl -lz -lstdc++
else
$(error Unknown architecture)
endif

###################################################
# Compile SimTaDyn project
all: $(TARGET)

###################################################
# Compile and launch unit tests and generate the code coverage html report.
.PHONY: unit-tests
unit-tests:
	@$(call print-simple,"Compiling unit tests")
	@make -C tests coverage

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
include $(M)/Makefile.footer
