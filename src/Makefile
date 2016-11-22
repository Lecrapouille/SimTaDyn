.PHONY: clean build

###################################################
# Color
CLR_DEFAULT := $(shell echo '\033[00m')
CLR_RED     := $(shell echo '\033[31m')
CLR_GREEN   := $(shell echo '\033[32m')
CLR_YELLOW  := $(shell echo '\033[33m')
CLR_BLUE    := $(shell echo '\033[34m')
CLR_PURPLE  := $(shell echo '\033[35m')
CLR_CYAN    := $(shell echo '\033[36m')
print-comp = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT) <= $(CLR_YELLOW)$3$(CLR_DEFAULT)"
print-link= \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT) => $(CLR_YELLOW)$3$(CLR_DEFAULT) $4"

###################################################
# Path where to store *.o files. Default: ./build/
ifeq ($(BUILD),)
BUILD = build
endif

###################################################
# Set include paths
INCLUDES = -I. -Igui -Iloader -Icells -Iforth -Irtree -Icommon -I/usr/include/mysql

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):gui:loader:cells:forth:rtree:common

###################################################
# Set objects architecture specific
OBJECTS_COMMON = Color.o Vector3D.o Camera2D.o
OBJECTS_RTREE  = BoundingBox.o RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJECTS_FORTH  = ForthReader.o ForthDico.o ForthPrimitives.o ForthInner.o
OBJECTS_CELLS  = SimTaDynCells.o SimTaDynGraphs.o
OBJECTS_LOADER = ShapeFile.o
OBJECTS_GUI    = OpenGL.o Textures.o Fonts.o Renderer.o DrawingArea.o \
	         TextEditor.o ForthEditor.o SimTaDynWindow.o
OBJECTS = $(OBJECTS_COMMON) $(OBJECTS_RTREE) $(OBJECTS_FORTH) $(OBJECTS_CELLS) $(OBJECTS_LOADER) $(OBJECTS_GUI) main.o
TARGET = SimTaDyn

###################################################
# Compil options
#CC = gcc
#CFLAGS = -W -Wall -Wextra -O2 -g `pkg-config --cflags gtk+-2.0 glib-2.0 gtkglext-1.0 pango`
#LDFLAGS = `pkg-config --libs gtk+-2.0 glib-2.0 gtkglext-1.0 pango`
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O0 -g -std=c++11 `pkg-config --cflags gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2 sigc++-2.0`
LDFLAGS = `pkg-config --libs gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2 sigc++-2.0`
DEFINES = -DCHECK_OPENGL

###################################################
# Set Libraries
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo

###################################################
# Files dependencies
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# Main entry
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(call print-link,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJECTS) -o $(TARGET) $(LIBS) $(LDFLAGS)

%.o: %.cpp
%.o: %.cpp $(BUILD)/%.d
	@$(call print-comp,"Compiling C","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ common/*~ rtree/*~ forth/*~ cells/*~ gui/*~ $(BUILD) 2> /dev/null

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
