SHELL=/bin/bash

# CC setup
XC8_VER="/v2.45/"
XC8_CC="/bin/xc8-cc"
CC=XC8_PATH+XC8_VER+XC8_CC

# DFP setup


# target device setup
UC="atmega328p"

#build directory
BUILD_DIR="../build"

# source files
include mk_src_files.mk

# header files
include mk_header_files.mk

# compile all
all: scr objects
	
# object files
objects: $(HEADER_DIR)
		$(CC)

# source files
scr: $(SRC_FILES) 
		$(cc) -mcpu=$(UC) -mdfp=$(DFP
		) -I $(HEADER_FILES) 