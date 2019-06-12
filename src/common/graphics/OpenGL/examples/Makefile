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
BUILD_TYPE = debug

###################################################
# Location from the project root directory.
P=../../../../..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
include $(M)/Makefile.header
include $(P)/Makefile.common

###################################################
# List of files to compile.
OBJ_UTILS    = Exception.o File.o ILogger.o Logger.o
OBJ_OPENGL   = OpenGL.o GLWindow.o GLImGUI.o
OBJ_EXAMPLES = \
  00_EmptyWindowWithIOEvents.o \
  01_EmptyWindowWithImgui.o \
  02_DynamicTriangle.o \
  03_MultiTexturedTriangle.o \
  05_VAOvsVBO.o \
  06_SphereEBO.o \
  07_SkyBoxTextureCube.o \
  08_TerrainTexture3D.o \
  09_SceneGraph.o \
  10_SimpleFrameBuffer.o \
  11_PostProdFrameBuffer.o \
  12_ComplexShader.o \
  13_Lighting.o

OBJ += $(OBJ_UTILS) $(OBJ_OPENGL) $(OBJ_EXAMPLES) main.o

###################################################
# Get the name of one of the resource to be download.
# This allows to check if the resources have been downloaded
RESOURCES := textures/README.md

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
EXTERNAL_LIBS += $(abspath $(P)/external/SOIL/libSOIL.a)

###################################################
# Compile tutorials
all: $(TARGET) $(RESOURCES)

###################################################
# Download resources before compiling sources
$(OBJS): | $(RESOURCES)
$(RESOURCES): | $(BUILD)
	@$(call print-from,"Downloading","$(PROJECT)","textures")
	@-rm -fr textures 2> /dev/null
	@git clone https://github.com/Lecrapouille/OpenGLCppWrapper-data.git --depth=1 textures > /dev/null
	@cd $(BUILD) && touch resources.done

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.footer
