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
TARGET = OpenGLExamples

###################################################
# Target version
VERSION := $(shell cat ../../../../VERSION)

###################################################
# Sharable informations between all Makefiles
M=../../../../../.makefile/
-include $(M)/Makefile.header

###################################################
# Inform Makefile where to find header files
INCLUDES = -I$(BUILD) -I.. -I../geometry -I../.. -I../../../utils	\
-I../../../maths -I../../../patterns -I../../../containers		\
-I../../../../../src -I../../../../../external/backward-cpp		\
-I../../../../../external/SOIL -I../../../../../

###################################################
#  Inform Makefile where to find *.cpp and *.o files
VPATH=.:$(BUILD):..:../geometry:../..:../../../maths:../../../utils:../../../patterns:../../../containers:../../../../../external/SOIL:../../../../../external/backward-cpp:../../../../../

###################################################
#  List of files to compile. Splited by directories
OBJ_TOOL     = backward.o
OBJ_UTILS    = Exception.o PendingData.o ILogger.o Logger.o
OBJ_OPENGL   = OpenGLException.o OpenGL.o GLObject.o GLShader.o         \
               GLVertexArray.o GLVertexBuffer.o GLAttrib.o GLTextures.o \
               GLWindow.o
OBJ          = $(OBJ_TOOL) $(OBJ_UTILS) $(OBJ_OPENGL) \
               Example01.o Example02.o Example03.o Example04.o Example05.o main.o

###################################################
# Compilation options. For knowing which libraries
# is needed please read the doc/Install.md file.
ifneq ($(MY_CXX),)
CXX = $(MY_CXX)
endif
CXXFLAGS = -W -Wall -Wextra -O2 -g -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`

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
LIBS = $(abspath ../../../../../external/SOIL/libSOIL.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the doc/Install.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS += -L/usr/local/lib -framework OpenGL -lglew -lglfw

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0 -latk-1.0		\
-lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0		\
-lgthread-2.0 -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11 -lGLEW	\
-lglfw -ldl -ldw

## Window
else
$(error Unknown architecture)
endif

###################################################
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-link,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ $(BUILD) 2> /dev/null

###################################################
# Generate a header file with the project version
version.h: VERSION
	@$(call print-from,"Version","$(TARGET)","VERSION","")
	@./$(M)/version.sh $(M)/../VERSION $(BUILD)/version.h

###################################################
# Sharable informations between all Makefiles
-include $(M)/Makefile.footer
