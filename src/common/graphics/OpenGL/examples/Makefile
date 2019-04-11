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
## along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

###################################################
# Executable name
PROJECT = OpenGLExamples
TARGET = $(PROJECT)

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# Location from the project root directory.
P=../../../../..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header
include $(P)/Makefile.common

###################################################
# List of files to compile. Splited by directories
OBJ_UTILS    = Exception.o File.o GLException.o PendingData.o ILogger.o Logger.o
OBJ_OPENGL   = OpenGL.o GLWindow.o
IMGUI        = imgui_draw.o imgui_widgets.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o
OBJ         += $(OBJ_EXTERNAL) $(OBJ_TOOL) $(OBJ_UTILS) $(OBJ_OPENGL) $(IMGUI) \
               Example01.o Example02.o Example03.o main.o

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
EXTERNAL_LIBS += $(abspath $(P)/external/SOIL/libSOIL.a)

###################################################
# Compile OpenGL examples
all: $(TARGET)

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
