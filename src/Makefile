.PHONY: clean build install-data

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
BUILD = ../build
endif

###################################################
# Where to install project datum
# FIXME: do not change this location (for the moment)
# ifeq ($(PROJECT_DATA_ROOT),)
PROJECT_DATA_ROOT = ~/.SimTaDyn
# endif
PROJECT_DATA_PATH = $(PROJECT_DATA_ROOT)/data

###################################################
# Set include paths
INCLUDES = -I. -I../tools -IUtils -ICommon/SharedLibrary -ICommon  \
	   -IOpenGL -ISimForth -ISimForth/Forth -ISimMap -ISimMap  \
	   -ISimMap/RTree -ISimMap/Graph -ISimMap/Loaders -ISimGui \
	   -ISimTaDyn
# -I/usr/include/mysql

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):../tools:Utils:Common/SharedLibrary:Common:OpenGL:SimForth:SimForth/Forth:SimMap:SimMap/RTree:SimMap/Graph:SimMap/Loaders:SimGui:SimTaDyn

###################################################
# Set objects architecture specific
OBJECTS_TOOL     = Backward.o Logger.o SimTaDynLogger.o
OBJECTS_UTILS    = Singleton.o Resources.o ResourceManager.o Exception.o  SharedLibrary_UNIX.o SharedLibrary.o
OBJECTS_COMMON   = File.o Color.o Vector3D.o Camera2D.o
OBJECTS_FORTH    = TextColor.o ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJECTS_RTREE    = BoundingBox.o RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJECTS_MAP      = BasicGraph.o SimTaDynGraph.o SimTaDynMap.o
OBJECTS_LOADER   = MapLoader.o ShapeFile.o
OBJECTS_OPENGL   = OpenGL.o Shader.o Textures.o Fonts.o Renderer.o #Renderable.o
OBJECTS_GUI      = Redirection.o TextEditor.o ForthEditor.o SimForth.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJECTS_SIMTADYN =

OBJECTS         = $(OBJECTS_TOOL) $(OBJECTS_UTILS) $(OBJECTS_COMMON) $(OBJECTS_FORTH) $(OBJECTS_RTREE) \
                  $(OBJECTS_MAP) $(OBJECTS_LOADER) $(OBJECTS_OPENGL) $(OBJECTS_GUI) $(OBJECTS_SIMTADYN) \
                  main.o

TARGET = SimTaDyn

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O0 -g -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -ldw
DEFINES = -DCHECK_OPENGL

###################################################
# Set Libraries
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11 -lGLEW -ldl

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
	@$(call print-comp,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ common/*~ rtree/*~ forth/*~ cells/*~ gui/*~ $(BUILD) 2> /dev/null

###################################################
# Install project datum
install-data:
	@$(call print-link,"Copying","../data","$(PROJECT_DATA_PATH)","")
	@rm -fr $(PROJECT_DATA_PATH)
	@mkdir -p $(PROJECT_DATA_PATH)/forth
	@cp -r ../data/* $(PROJECT_DATA_PATH)
	@cp -r SimForth/Forth/core/system.fs $(PROJECT_DATA_PATH)/forth

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
