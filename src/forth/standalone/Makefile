##=====================================================================
## SimForth: A Forth for SimTaDyn project.
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
TARGET = SimForth

###################################################
# Location from the project root directory.
P=../../..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
-include $(M)/Makefile.header

###################################################
# List of files to compile. Splited by directories
ifeq ($(PROJECT_MODE),debug)
OBJ_EXTERNAL   = backward.o
else
OBJ_EXTERNAL   =
endif
OBJ_UTILS      = Exception.o File.o ILogger.o Logger.o Path.o
OBJ_FORTH      = ForthExceptions.o ForthUtils.o ForthStream.o ForthDictionary.o ForthClibrary.o Forth.o
OBJ_STANDALONE = main.o
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_FORTH) $(OBJ_STANDALONE)

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11 `pkg-config --cflags gtkmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0`

###################################################
#
INCLUDES += -I$(P)/src
VPATH += :$(P)/src

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL -DARCHI=$(ARCHI)
# Disable ugly gtkmm compilation warnings
DEFINES += -DGTK_SOURCE_H_INSIDE -DGTK_SOURCE_COMPILATION
DEFINES += -DUSE_COMPUTED_GOTO

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS =

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS += -L/usr/local/lib

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -ldl

## Windows
else

#$(error Unknown architecture)
endif

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
# Code coverage. Uncomment if desired
# CXXFLAGS += --coverage
# LDFLAGS += --coverage

###################################################
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
# Install project. You need to be root user.
.PHONY: install
install: $(TARGET)
	@$(call print-to,"Installing","data","$(PROJECT_DATA_PATH)","")
	@rm -fr $(PROJECT_DATA_PATH)
	@mkdir -p $(PROJECT_DATA_PATH)/forth
	@cp -r ../core/system.fs $(PROJECT_DATA_PATH)/forth
	@cp -r ../core/LibC $(PROJECT_DATA_PATH)/forth
	@$(call print-to,"Installing","doc","$(PROJECT_DOC_PATH)","")
	@mkdir -p $(PROJECT_DOC_PATH)
	@cp -r ../doc/* $(PROJECT_DATA_ROOT)/doc
#	@cp AUTHORS LICENSE README.md ChangeLog $(PROJECT_DATA_ROOT)
	@$(call print-to,"Installing","$(BUILD)/$(TARGET)","$(PROJECT_EXE)","")
	@mkdir -p $(BINDIR)
	@cp $(BUILD)/$(TARGET) $(PROJECT_EXE)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ ../*~ $(BUILD) 2> /dev/null

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.help
include $(M)/Makefile.footer
