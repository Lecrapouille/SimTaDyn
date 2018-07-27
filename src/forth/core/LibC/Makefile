################################################################################
###
ifeq ($(SRCS),)
$(error "Missing passing C file name as makefile parameter. Do something like: make SRCS=library-name.c")
endif
ifeq ($(BUILD),)
$(error "Missing passing build directory name as makefile parameter. Do something like: make BUILD=/tmp")
endif

OBJS=$(SRCS:.c=.o)
MAKEFLAGS += --no-print-directory
VPATH=$(BUILD):.

################################################################################
### Compiler
CC=gcc -W -Wall

################################################################################
### Detect the operating system: Unix, OSX and Windows
ifeq ($(OS),Windows_NT)
ARCHI := Windows
else
ARCHI := $(shell uname -s)
endif

###################################################
# Define colors
CLR_DEFAULT = $(shell echo '\033[00m')
CLR_RED     = $(shell echo '\033[31m')
CLR_GREEN   = $(shell echo '\033[32m')
CLR_YELLOW  = $(shell echo '\033[33m')
CLR_BLUE    = $(shell echo '\033[34m')
CLR_PURPLE  = $(shell echo '\033[35m')
CLR_CYAN    = $(shell echo '\033[36m')

###################################################
# Display predefine colorful messages
print-simple = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT)"
print-from = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT) <= $(CLR_YELLOW)$3$(CLR_DEFAULT)"
print-to = \
	@echo "$(CLR_PURPLE)*** $1:$(CLR_DEFAULT) $(CLR_CYAN)$2$(CLR_DEFAULT) => $(CLR_YELLOW)$3$(CLR_DEFAULT) $4"

################################################################################
### Compile a dynamic library
all: $(OBJS)

%.o: %.c
ifeq ($(ARCHI),Darwin)
	@$(CC) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(CC) -dynamiclib -undefined suppress -flat_namespace $(abspath $(BUILD)/$@) -o (patsubst %.o,%.dylib,$@) $(EXTLIBS)
else ifeq ($(ARCHI),Linux)
	@$(call print-to,"Compiling","$<","$(patsubst %.o,%.so,$(abspath $(BUILD)/$@))")
	@$(CC) -c -fpic $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(CC) -shared $(abspath $(BUILD)/$@) -o $(patsubst %.o,%.so,$(abspath $(BUILD)/$@)) $(EXTLIBS)
else ifeq ($(OS),Windows_NT)
	@$(CC) -c -fpic $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(CC) -shared (abspath $(BUILD)/$@) -o $(patsubst %.o,%.dll,$@) -Wl,--out-implib,$(patsubst %.o,%.a,$@) $(EXTLIBS)
else
	error "I dunno how to compile dynamic lib with this architecture"
endif

################################################################################
###
.PHONY: clean
clean:
	@cd $(BUILD) && rm -fr *~ *.o *.dylib *.dll *.so
