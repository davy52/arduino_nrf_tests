SHELL=/bin/bash

# CC setup
XC8_VER=/v2.41/
XC8_CC=/bin/xc8-cc
CC=XC8_PATH+XC8_VER+XC8_CC
CC=xc8-cc

# DFP setup
DFP_FAMILY=/AtmegaDFP
DFP_VER=/2.41
DFP=$(DFP_PATH)$(DFP_FAMILY)$(DFP_VER)


# target device setup
UC=atmega328p

#build directory
BUILD_DIR=../build

# project name
P_NAME=arduino_nrf

# binaries
HEX=$(BUILD_DIR)/$(P_NAME).hex
ELF=$(BUILD_DIR)/$(P_NAME).elf

# included directories
include mk_include_dirs.mk

# source files
define get_source_from_dirs
		SRC_FILES += $$(wildcard $(1)/*.c)
endef

SRC_FILES=
$(info $(INCLUDE_DIRS))
$(foreach dir,$(INCLUDE_DIRS),$(eval $(call get_source_from_dirs,$(dir))))

# objects
OBJS_=$(foreach file, $(SRC_FILES),$(file:.c=.o))
$(info objs= $(OBJS))

# Compler options
CFLAGS = -mcpu=$(UC) -mdfp=$(DFP) -Wall -Og
CFLAGS += -L$(DFP)/gcc/dev/atmega328p -Xlinker --verbose

all: $(HEX)

# make hex
$(HEX): $(ELF)
		@echo ""
		@echo making hex
		$(CC) $(CFLAGS) -o $(HEX) $(ELF)


# make elf 
$(ELF): $(OBJS)
		@echo ""
		@echo linking objects
		$(CC) $(CFLAGS) -o $(ELF) $(OBJS)
	
	
# object files
%.o: %.c $(BUILD_DIR)
		@echo ""
		@echo "compiling object $@"
		$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -c -o $@ $<


# build dir
$(BUILD_DIR):
		@echo making build directory
		mkdir -p $@

clean:
	rm -f $(OBJS) $(HEX) $(ELF)
