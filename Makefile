SHELL=/bin/bash

# CC setup
XC8_VER=/v2.41/
XC8_CC=/bin/xc8-cc
CC=XC8_PATH+XC8_VER+XC8_CC
CC=avr-gcc

ELF_TO_HEX=avr-objcopy
ELF_SIZE=avr-size

# DFP setup
DFP_FAMILY=/AtmegaDFP
DFP_VER=/2.41
DFP=$(DFP_PATH)$(DFP_FAMILY)$(DFP_VER)


# target device setup
UC=atmega328p
BOARD=arduino_pro_mini_3v3
# BOARD=arduino_nano

# upload port
#PORT := $(if $(PORT),$(PORT),COM11)
PORT_FLAG := $(if $(PORT), -P $(PORT), )

#build directory
BUILD_DIR=./build

# project name
P_NAME=arduino_common

# binaries
HEX=$(BUILD_DIR)/$(P_NAME).hex
ELF=$(BUILD_DIR)/$(P_NAME).elf

# included directories
include Makefile_dirs.mk

VPATH = $(foreach dir, $(INCLUDE_DIRS), $(dir):)
#$(info VPATH= $(VPATH))

# source files
define get_source_from_dirs
		SRC_FILES += $$(wildcard $(1)/*.c)
endef

SRC_FILES=
#$(info $(INCLUDE_DIRS))
$(foreach dir,$(INCLUDE_DIRS),$(eval $(call get_source_from_dirs,$(dir))))

#$(info src= $(SRC_FILES))
#$(info )

# objects
OBJS2=$(foreach path, $(SRC_FILES),$(path:.c=.o))
OBJS1=$(foreach path, $(OBJS2),$(notdir $(path)))
OBJS=$(addprefix $(BUILD_DIR)/, $(OBJS1))
#OBJS=$(OBJS1)
#VPATH+=$(BUILD_DIR)


$(info  objs= $(OBJS))
#$(info )

# Compler options
CFLAGS = -mmcu=$(UC) -Wall -Os -g
CFLAGS += -L$(DFP)/gcc/dev/atmega328p
CFLAGS += -D$(BOARD)

# Linker options
LDFLAGS = -mmcu=$(UC)

all: $(BUILD_DIR) $(HEX)



# object files
$(BUILD_DIR)/%.o: %.c
		@echo ""
		@echo "compiling object $@"
		$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -c -o $@ $^


# make elf 
$(ELF): $(OBJS)
		@echo ""
		@echo linking objects
		$(CC) $(LDFLAGS) -o $@ $^
	

# make hex
$(HEX): $(ELF)
		@echo ""
		@echo making hex
		$(ELF_TO_HEX) -O ihex -R .eeprom $^ $@
		@echo ""
		@echo "Build Complete!"
		@echo ""
		@$(ELF_SIZE) -C --mcu=$(UC) $(ELF)


# build dir
$(BUILD_DIR):
		@echo making build directory
		mkdir -p $@


# upload file to mcu
upload:
	avrdude.exe -p $(UC) -c arduino $(PORT_FLAG) -b 57600 -U flash:w:$(HEX)

upload_new :  $(HEX) upload
	@echo

clean:
	rm -f $(BUILD_DIR)/*
