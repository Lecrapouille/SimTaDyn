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
# Executable name
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
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJ_STANDALONE = main.o
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_FORTH) $(OBJ_STANDALONE)

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -std=c++11
LDFLAGS =

###################################################
# Common defines
SIMTADYN_DEFINES=-DCHECK_OPENGL -DSIMTADYN_TEMP_DIR=\"/tmp/SimTaDyn/\" -DSIMTADYN_DATA_PATH=\"$(PROJECT_DATA_PATH)\"
DEFINES += $(SIMTADYN_DEFINES)

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
OPTIM_FLAGS = -O2 -g
DEFINES += -DBACKWARD_HAS_DW=1
LIBS += -ldw
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
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM_FLAGS) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ ../*~ $(BUILD) 2> /dev/null

###################################################
# Sharable informations between all Makefiles
-include $(M)/Makefile.footer
