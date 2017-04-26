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
# Set include paths
INCLUDES = -I. -I../tools \
           -Icommon -I../src/Common -I../src/Common/Containers \
           -Iopengl -I../src/OpenGL \
           -Isimmap -I../src/SimMap/Graph \
           -I../src/SimTaDyn

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):../tools:common:../src/Common:../src/Common/Containers:opengl:../src/OpenGL:simmap:../src/SimMap/Graph:../src/SimTaDyn

###################################################
# Set objects architecture specific
OBJECTS = \
	Backward.o Logger.o SimTaDynLogger.o \
	File.o FileTests.o \
	Vector3D.o VectorTests.o \
	Color.o ColorTests.o \
	SetTests.o CollectionTests.o \
	Exception.o GLObject.o GLBufferTests.o \
	BasicGraph.o BasicArcTests.o BasicNodeTests.o BasicGraphTests.o \
        GraphAlgorithm.o GraphAlgoTests.o \
	main.o
#	BoundingBox.o BoundingBoxTests.o

#
TARGET = UnitTest

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O0 --coverage -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = --coverage `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
DEFINES =

###################################################
# Set Libraries
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11 -lGLEW -ldl -ldw

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
	@rm -fr *~ gcov gui/*~ cells/*~ common/*~ $(BUILD) 2> /dev/null

###################################################
# Code coverage
coverage: $(TARGET) gcov
	./build/$(TARGET)
	gcovr -r ../. --html -o gcov/coverage.html --html-details

gcov:
	@mkdir -p gcov

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)
$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
