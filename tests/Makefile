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
INCLUDES = -I. -Icommon -Icells -Iopengl -I../src/Common -I../src/Common/Containers -I../src/OpenGL -I../src/SimTaDyn -I../src/SimMap/Graph -I../src/SimMap/RTree

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):common:cells:opengl:../src/Common:../src/Common/Containers:-I../src/OpenGL:../src/SimTaDyn:../src/SimMap/Graph:../src/SimMap/RTree

###################################################
# Set objects architecture specific
OBJECTS = \
	File.o FileTests.o \
	Vector3D.o VectorTests.o \
	Color.o ColorTests.o \
	SetTests.o CollectionTests.o \
	GLBufferTests.o \
	main.o
#	BoundingBox.o BoundingBoxTests.o

#
TARGET = UnitTest

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O3 --coverage -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = --coverage `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0` -lcppunit
DEFINES =

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
