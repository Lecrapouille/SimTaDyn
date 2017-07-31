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
# Operating system detection
ifeq ($(OS),Windows_NT)
ARCHI := Windows
else
ARCHI := $(shell uname -s)
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
INCLUDES = -I. -I../external/backward -I../external/YesEngine -Icommon/patterns			\
-Icommon/managers -Icommon/utils -Icommon/maths -Icommon/containers	\
-Icommon/graph-theory -Icommon/graphics/OpenGL				\
-Icommon/graphics/RTree -Icommon/graphics -Icore -Icore/loaders		\
-Iforth -Iui

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):../external/backward:../external/YesEngine:common/patterns:common/managers:common/utils:common/maths:common/containers:common/graph-theory:common/graphics:common/graphics/OpenGL:common/graphics/RTree:core:core/loaders:forth:ui

###################################################
OBJ_EXTERNAL   = Backward.o
OBJ_UTILS      = Exception.o ILogger.o Logger.o File.o Path.o
OBJ_PATTERNS   = Singleton.o
OBJ_MATHS      = Maths.o
OBJ_CONTAINERS = PendingData.o
OBJ_MANAGERS   =
OBJ_GRAPHS     = Graph.o GraphAlgorithm.o
OBJ_OPENGL     = Color.o Camera2D.o OpenGL.o GLObject.o GLShader.o GLVertexArray.o
OBJ_OPENGL    += GLVertexBuffer.o GLAttrib.o GLTextures.o Renderer.o
# OBJ_RTREE      = RTreeNode.o RTreeIndex.o RTreeSplit.o
OBJ_FORTH      = ForthExceptions.o ForthStream.o ForthDictionary.o ForthPrimitives.o Forth.o
OBJ_CORE       = SimForth.o CellForth.o SimTaDynGraph.o SimTaDynMap.o
OBJ_LOADERS    = ILoader.o ShapeFile.o
# SimTaDynFile.o
OBJ_GUI        = Redirection.o PackageExplorer.o TextEditor.o ForthEditor.o
OBJ_GUI       += Inspector.o MapEditor.o DrawingArea.o SimTaDynWindow.o
OBJ_SIMTADYN   = main.o

###################################################
TARGET         = SimTaDyn
OBJ            = $(OBJ_EXTERNAL) $(OBJ_UTILS) $(OBJ_PATTERNS) $(OBJ_MATHS) $(OBJ_CONTAINERS) \
                 $(OBJ_MANAGERS) $(OBJ_GRAPHS) $(OBJ_OPENGL) $(OBJ_FORTH) $(OBJ_CORE) $(OBJ_LOADERS) \
                 $(OBJ_GUI) $(OBJ_SIMTADYN)

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O0 -g -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`
DEFINES = -DCHECK_OPENGL

###################################################
# Set Libraries

ifeq ($(ARCHI),Darwin)
LIBS = -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -framework OpenGL -lglew
else ifeq ($(ARCHI),Linux)
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11\
       -lGLEW -ldl -ldw -lSOIL -lZipper-static -lz
else
$(error Unknown architecture)
endif

###################################################
# Files dependencies
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# Main entry
all: $(TARGET)

$(TARGET): $(OBJ)
	@$(call print-link,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

%.o: %.cpp
%.o: %.cpp $(BUILD)/%.d
	@$(call print-comp,"Compiling C++","$(TARGET)","$<")
ifeq ($(ARCHI),Darwin)
	@mkdir -p $(BUILD)
endif
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Check yaml syntax
check-travis:
	@travis lint ../.travis.yml

###################################################
# Coverity Scan: static analysis of code
# Prepare the tarball ready to be uploaded to Coverity Scan servers.
coverity:
	@cov-build --dir cov-int make && tar czvf SimTaDyn.tgz cov-int

###################################################
# Compress SimTaDyn sources
targz:
	@tar czvf /tmp/SimTaDyn.tgz .. && mv /tmp/SimTaDyn.tgz .

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ common/*~ rtree/*~ forth/*~ cells/*~ gui/*~ $(BUILD) cov-int SimTaDyn.tgz 2> /dev/null

###################################################
# Install project datum
install-data:
	@$(call print-link,"Copying","../data","$(PROJECT_DATA_PATH)","")
	@rm -fr $(PROJECT_DATA_PATH)
	@mkdir -p $(PROJECT_DATA_PATH)/forth
	@cp -r ../data/* $(PROJECT_DATA_PATH)
	@cp -r forth/core/system.fs $(PROJECT_DATA_PATH)/forth

###################################################
# Create a temporary folder to store *.o and *.d files
$(DEPFILES): | $(BUILD)
$(OBJ): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)

###################################################
# Auto-Dependency Generation
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJ)))
