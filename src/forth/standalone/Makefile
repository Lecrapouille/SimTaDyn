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
# Target version
VERSION := $(shell cat ../../VERSION)

###################################################
# Sharable informations between all Makefiles
-include ../../../.makefile/Makefile.header

###################################################
# Path where to store the generated coverage docs
DOC = ../doc/coverage
# Path where to store lcov rapport
RAPPORT = build/rapport.info

###################################################
# Inform Makefile where to find header files
INCLUDES = -I$(BUILD) -I. -I.. -I../../../src/common/utils	\
-I../../../src/common/patterns -I../../../external/backward-cpp

###################################################
# Inform Makefile where to find *.cpp and *.o files
VPATH=$(BUILD):.:..:../../../src/common/utils:../../../src/common/patterns:../../../external/backward-cpp

###################################################
# List of files to compile. Splited by directories
OBJ_EXTERNAL   = backward.o
OBJ_UTILS      = Exception.o ILogger.o Logger.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJ_STANDALONE =
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_FORTH) $(OBJ_STANDALONE) main.o

###################################################
# Compilation options. For knowing which libraries
# is needed please read the doc/Install.md file.
OPTIM = -O2
CXXFLAGS = -W -Wall -Wextra -g -std=c++11

###################################################
# Common defines
PROJECT_DATA_PATH=$(DESTDIR)$(PREFIX)/share
PROJECT_DATA_ROOT = $(DATADIR)/SimTaDyn/$(VERSION)
PROJECT_DATA_PATH = $(PROJECT_DATA_ROOT)/data
SIMTADYN_DEFINES=-DBACKWARD_HAS_DW=1 -DCHECK_OPENGL -DSIMTADYN_DATA_PATH=\"$(PROJECT_DATA_PATH)\"
DEFINES = $(SIMTADYN_DEFINES)

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS =

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the doc/Install.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS +=

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -ldl -ldw

## Windows
else
#$(error Unknown architecture)
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
%.o: %.cpp $(BUILD)/%.d Makefile ../../../.makefile/Makefile.header ../../../.makefile/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(OPTIM) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Generate the rapport of the code coverage
.PHONY: coverage
coverage: $(TARGET)
	@$(call print-to,"Running","$(TARGET)","$(RAPPORT)","")
	@./build/$(TARGET) || cat SimTaDyn.log
	@$(call print-to,"Documentation","$(RAPPORT)","$(DOC)","")
	@lcov --quiet --directory .. -c -o $(RAPPORT)
	@lcov --quiet --remove $(RAPPORT) '/usr*' 'external/*' 'tests/*' -o $(RAPPORT)
	@genhtml --quiet -o $(DOC) -t "SimForth" $(RAPPORT)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ ../*~ $(BUILD) coverage_report 2> /dev/null

###################################################
# Generate a header file with the project version
version.h: VERSION
	@$(call print-from,"Version","$(TARGET)","VERSION","")
	@../../../.makefile/version.sh VERSION $(BUILD)/version.h

###################################################
# Sharable informations between all Makefiles
-include ../../../.makefile/Makefile.footer
