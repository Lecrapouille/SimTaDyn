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
BUILD = build
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
INCLUDES = -I. -I.. -I../geometry -I../.. -I../../../utils -I../../../maths -I../../../patterns -I../../../containers -I../../../../../src -I../../../../../external/backward

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):..:../geometry:../..:../../../maths:../../../utils:../../../patterns:../../../containers:../../../../../external/backward

###################################################
# Set objects architecture specific
OBJECTS_TOOL     = Backward.o
OBJECTS_UTILS    = PendingData.o ILogger.o Logger.o Exception.o
OBJECTS_OPENGL   = OpenGL.o GLObject.o GLShader.o GLVertexArray.o GLVertexBuffer.o GLAttrib.o GLTextures.o GLWindow.o
OBJECTS          = $(OBJECTS_TOOL) $(OBJECTS_UTILS) $(OBJECTS_OPENGL) \
                   Example01.o Example02.o Example03.o Example04.o Example05.o main.o
TARGET = Examples

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -g -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`
DEFINES = -DCHECK_OPENGL

###################################################
# Set Libraries
ifeq ($(ARCHI),Darwin)
INCLUDES += -I/opt/local/include -I/usr/local/include -I/opt/X11/include -I/opt/X11/lib
LIBS = -L/opt/local/lib -L/usr/local/lib -framework OpenGL -lglew -lglfw -lSOIL
else ifeq ($(ARCHI),Linux)
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11\
       -lGLEW -lglfw -ldl -ldw -lSOIL -lbfd
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

$(TARGET): $(OBJECTS) Makefile
	@$(call print-link,"Linking","$(TARGET)","$(BUILD)/$@","")
	@cd $(BUILD) && $(CXX) $(OBJECTS) -o $(TARGET) $(LIBS) $(LDFLAGS)

%.o: %.cpp
%.o: %.cpp $(BUILD)/%.d
	@$(call print-comp,"Compiling C++","$(TARGET)","$<")
ifeq ($(ARCHI),Darwin)
	@mkdir -p $(BUILD)
endif
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ $(BUILD) 2> /dev/null

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
