
.PHONY: clean build coverage gcov

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
# Path where to store the generated coverage docs
DOC = ../doc/coverage
# Path where to store lcov rapport
RAPPORT = build/rapport.info

###################################################
# Operating system detection
ifeq ($(OS),Windows_NT)
ARCHI := Windows
else
ARCHI := $(shell uname -s)
endif

###################################################
# Set include paths
INCLUDES = -I. -I../external/backward -I../src			\
-I../src/common/patterns -I../src/common/managers		\
-I../src/common/utils -I../src/common/maths			\
-I../src/common/containers -I../src/common/graph-theory		\
-I../src/common/graphics/OpenGL -I../src/common/graphics/RTree	\
-I../src/common/graphics -I../src/core -I../src/core/loaders	\
-I../src/forth -I../src/ui -Isimmap -Icommon -Iopengl

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):../external/backward:../src:../src/common/patterns:../src/common/managers:../src/common/utils:../src/common/maths:../src/common/containers:../src/common/graph-theory :../src/common/graphics/OpenGL:../src/common/graphics/RTree:../src/common/graphics:../src/core:../src/core/loaders:../src/forth:../src/ui:simmap:common:opengl

###################################################
# Set objects architecture specific
OBJECTS = Backward.o ILogger.o Logger.o PendingData.o File.o FileTests.o Maths.o	\
	VectorTests.o MatrixTests.o TransformationTests.o		\
	BoundingBoxTests.o FilteringTests.o Color.o ColorTests.o	\
	SetTests.o CollectionTests.o Exception.o OpenGL.o GLObject.o	\
	GLShader.o GLVertexArray.o GLAttrib.o GLTextures.o GLWindow.o	\
	Graph.o BasicArcTests.o BasicNodeTests.o BasicGraphTests.o	\
	GraphAlgorithm.o GraphAlgoTests.o GLBufferTests.o		\
	OpenGLTests.o main.o

#
TARGET = UnitTest

###################################################
# Compil options
CXX = g++
DEFINES =

###################################################
# Set Libraries
ifeq ($(ARCHI),Darwin)
CXXFLAGS = -W -Wall -Wextra -O0 -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
LIBS = -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -framework OpenGL -lglew -lglfw -lSOIL
else ifeq ($(ARCHI),Linux)
CXXFLAGS = -W -Wall -Wextra -O0 --coverage -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = --coverage `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread      \
       -lX11 -lGLEW  -lglfw -lSOIL -ldl -ldw
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
	@$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ $(DOC) gui/*~ cells/*~ common/*~ $(BUILD) 2> /dev/null

###################################################
# Code coverage
coverage: $(TARGET)
	@$(call print-link,"Running","$(TARGET)","$(RAPPORT)","")
	./build/$(TARGET)
	@$(call print-link,"Documentation","$(RAPPORT)","$(DOC)","")
	@lcov --quiet --directory .. -c -o $(RAPPORT)
	@lcov --quiet --remove $(RAPPORT) '/usr*' -o $(RAPPORT)
	@genhtml --quiet -o $(DOC) -t "SimTaDyn Unit Tests" $(RAPPORT)

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
