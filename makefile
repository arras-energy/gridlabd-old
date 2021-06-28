# makefile
#
# GridLAB-D makefile
#
# USAGE
#
#   make [TARGETS ...]
#
# FILES
#
#   FOLDERS/makefile       Makefiles for subfolders (automatically included)
#   makefile.conf          Configuration makefile (optional, included if found in current folder)
#   SOURCE/makefile.conf   Delivery options (optional, included if found in source folder)
#

all: build

#
# Check version of make
#
ifndef GNUMAKE
$(error GNU Make is required)
endif

#
# Disable automatic rules
#

.SUFFIXES:
    MAKEFLAGS += -r

####################################################
#
# Basic parameters
#

# Basename of the makefile 
MAKEFILE = $(notdir $(firstword $(MAKEFILE_LIST)))

# Source folder for the build
SOURCE = $(dir $(firstword $(MAKEFILE_LIST)))
ifeq ($(SOURCE),./)
SOURCE = $(PWD)/
endif

# Build folder for the build
ifeq ($(SOURCE),$(PWD)/)
BUILD = $(PWD)/build/
else
BUILD = $(PWD)/
endif

####################################################
#
# Make configuration
#

# Default configuration
include $(SOURCE)makefile.conf

# Local configuration
-include ./makefile.conf

ifdef VERBOSE
ECHO = 
else
ECHO = @
endif

####################################################
#
# Build folders and targets
#

INCLUDE_DIRS =
BUILD_FOLDERS = $(BUILD)/{bin,lib,share,etc,include}
BUILD_TARGETS = 
INSTALL_TARGETS =
CLEAN_TARGETS =
INCLUDE = $(shell $(SOURCE)utilities/configure $(SOURCE)$(MAKEFILE))
PYTHONCFLAGS = $(shell python3-config --cflags)
PYTHONLDFLAGS = $(shell python3-config --ldflags)

include $(INCLUDE)

$(shell mkdir -p $(BUILD_FOLDERS))

####################################################
#
# Standard targets
#

info: 
	@echo PACKAGE_NAME = $(PACKAGE_NAME)
	@echo PACKAGE = $(PACKAGE)
	@echo INSTALL = $(INSTALL)
	@echo INCLUDE = $(INCLUDE)
	@echo SOURCE = $(SOURCE)
	@echo BUILD_TARGETS = $(BUILD_TARGETS)
	@echo INSTALL_TARGETS = $(INSTALL_TARGETS)
	@echo CLEAN_TARGETS = $(CLEAN_TARGETS)

help: 
	@echo "Syntax: make -f $(MAKEFILE) TARGET ..."
	@echo "Valid targets:"
	@echo "  build     build $(PACKAGE)"
	@echo "  clean     clean up build files"
	@echo "  help      obtain this help"
	@echo "  info      display make configuration"
	@echo "  install   install $(PACKAGE) to $(INSTALL)"

build: $(BUILD_TARGETS)

install: build $(INSTALL_TARGETS)

clean: $(CLEAN_TARGETS)

