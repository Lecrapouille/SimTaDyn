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
DESCRIPTION = A GIS in a spreadsheet

###################################################
# Debug mode or Release mode
BUILD_TYPE = debug

###################################################
# Location from the project root directory.
P=.

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header
include $(P)/Makefile.common

###################################################
# Make the list of compiled files
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_PATTERNS   =
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS =
OBJ_MANAGERS   =
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_OPENGL     = Color.o Camera2D.o OpenGL.o NodesRenderer.o Renderer.o
# OBJ_RTREE      = RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o      \
                 ForthPrimitives.o ForthClibrary.o Forth.o
OBJ_CORE       = ASpreadSheetCell.o ASpreadSheet.o SimTaDynForth.o      \
                 SimTaDynForthPrimitives.o SimTaDynSheet.o SimTaDynMap.o
OBJ_LOADERS    = ManagerException.o SimTaDynLoaders.o ShapeFileLoader.o \
                 SimTaDynFileLoader.o
# TextureFileLoader.o SimTaDynFile.o
OBJ_GUI        = Redirection.o Windows.o MapExplorer.o TextEditor.o     \
                 ForthEditor.o ForthInspector.o DrawingArea.o MVP.o     \
                 ForthEditorWindow.o MapEditorCommands.o MapEditor.o    \
                 MapEditorWindow.o MapExplorerWindow.o
OBJ_SIMTADYN   = SimTaDyn.o main.o

OBJS += $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS)	\
        $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH)	\
        $(OBJ_CORE) $(OBJ_LOADERS) $(OBJ_GUI) $(OBJ_SIMTADYN)

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
THIRDPART_LIBS += \
	$(abspath $(P)/external/SOIL/libSOIL.a) \
	$(abspath $(P)/external/zipper/build/libZipper-static.a)

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
# Install project. You need to be root.
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
# Uninstall the project. You need to be root. FIXME: to be updated
#.PHONY: uninstall
#uninstall:
#	@$(call print-simple,"Uninstalling",$(PREFIX)/$(TARGET))
#	@rm $(PROJECT_EXE)
#	@rm -r $(PROJECT_DATA_ROOT)

###################################################
# Clean the whole project.
.PHONY: veryclean
veryclean: clean
	@rm -fr cov-int $(TARGET).tgz *.log foo 2> /dev/null
	@cd tests && make -s clean; cd - > /dev/null
	@cd src/common/graphics/OpenGL/examples/ && $(MAKE) -s clean; cd - > /dev/null
	@cd src/forth/standalone && v -s clean; cd - > /dev/null
	@cd src/core/standalone/ClassicSpreadSheet && $(MAKE) -s clean; cd - > /dev/null
	@$(call print-simple,"Cleaning","$(PWD)/doc/html")
	@cd doc/ && rm -fr html

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
