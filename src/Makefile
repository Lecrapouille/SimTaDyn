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
ifeq ($(DEST),)
DEST = build
endif

###################################################
# Set include paths
INCLUDES = -I. -Igui -Icells -Icommon -I/usr/include/mysql

###################################################
# Path for Makefile to find *.o
VPATH=.:$(DEST):gui:cells:common

###################################################
# Set objects architecture specific
OBJECTS_COMMON = color.o vector3D.o boundingbox.o
OBJECTS_GUI = camera2D.o renderer.o opgl.o gui.o
OBJECTS_CELLS = nodes.o arcs.o shapefile.o
OBJECTS = $(OBJECTS_COMMON) $(OBJECTS_GUI) $(OBJECTS_CELLS) main.o
TARGET = SimTaDyn

###################################################
# Compil options
#CC = gcc
#CFLAGS = -W -Wall -Wextra -O2 -g `pkg-config --cflags gtk+-2.0 glib-2.0 gtkglext-1.0 pango`
#LDFLAGS = `pkg-config --libs gtk+-2.0 glib-2.0 gtkglext-1.0 pango`
CC=g++
CFLAGS = -W -Wall -Wextra -O2 -g -std=c++11 `pkg-config --cflags gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2`
LDFLAGS = `pkg-config --libs gtkmm-2.4 glibmm-2.4 gtkglextmm-1.2`
DEFINES =

###################################################
# Set Libraries
LIBS = -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0   \
       -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0        \
       -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0     \
       -lcairo

###################################################
# Main entry
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@$(call print-link,"Linking","$(TARGET)","$(DEST)/$@","")
	@cd $(DEST) && $(CC) $(OBJECTS) -o $(TARGET) $(LIBS) $(LDFLAGS)

%.o: %.cpp %.hpp
	@$(call print-comp,"Compiling C","$(TARGET)","$<")
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(DEST)/$@)

%.o: %.cpp
	@$(call print-comp,"Compiling C","$(TARGET)","$<")
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(DEST)/$@)

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ gui/*~ cells/*~ $(DEST) 2> /dev/null

###################################################
# Create a temporary folder to store *.o
$(OBJECTS): | $(DEST)
$(DEST):
	@mkdir -p $(DEST)
