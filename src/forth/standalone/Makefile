.PHONY: clean build coverage with-code-coverage

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
INCLUDES = -I. -I.. -I../../common -I../../common/SharedLibrary

###################################################
# Path for Makefile to find *.o
VPATH=.:$(BUILD):..:../../common:../../common/SharedLibrary

###################################################
# Set objects architecture specific
OBJECTS_POCO       = Exception.o SharedLibrary_UNIX.o SharedLibrary.o
OBJECTS_FORTH      = ForthReader.o ForthDico.o ForthPrimitives.o ForthInner.o
OBJECTS_STANDALONE =
OBJECTS            = $(OBJECTS_POCO) $(OBJECTS_FORTH) $(OBJECTS_STANDALONE) main.o
TARGET             = SimForth

###################################################
# Compil options
CXX = g++
CXXFLAGS = -W -Wall -Wextra -O2 -g -std=c++11
LDFLAGS = -ldl
DEFINES =

###################################################
# Set Libraries
LIBS =

###################################################
# Files dependencies
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD)/$*.Td
POSTCOMPILE = mv -f $(BUILD)/$*.Td $(BUILD)/$*.d

###################################################
# Code coverage
ifneq ($(filter with-code-coverage, $(MAKECMDGOALS)),)
CXXFLAGS += --coverage
LDFLAGS += --coverage
endif

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
# Code coverage
with-code-coverage: all

coverage-report: $(TARGET)
	@mkdir -p coverage_report 2> /dev/null
	@rm -fr coverage_report/* 2> /dev/null
	@gcovr -r ../../. --html -o coverage_report/coverage.html --html-details
	@firefox coverage_report/coverage.html

###################################################
# Clean Target
clean:
	@echo "*** Cleaning"
	@rm -fr *~ ../*~ $(BUILD) coverage_report 2> /dev/null

###################################################
# Create a temporary folder to store *.o
$(DEPFILES): | $(BUILD)
$(OBJECTS): | $(BUILD)
$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.d: ;
.PRECIOUS: $(BUILD)/%.d

-include $(patsubst %,$(BUILD)/%.d,$(basename $(OBJECTS)))
