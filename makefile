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
# Build options
#

PYTHON_EXEC=$(shell which python3)
PYTHON_CONFIG=$(shell which python3-config)

####################################################
#
# Build folders and targets
#

INCLUDE_DIRS =
BUILD_TARGETS = 
INSTALL_TARGETS =
CLEAN_TARGETS =
BUILD_FOLDERS = $(BUILD)/{bin,lib,share,etc,include}
INCLUDE = $(shell $(SOURCE)utilities/configure $(SOURCE)$(MAKEFILE))
PYTHONCFLAGS = $(shell python3-config --cflags) 
PYTHONLDFLAGS = $(shell python3-config --ldflags)

BUILDINFO =
BUILDINFO += -DPYTHON_EXEC='"$(PYTHON_EXEC)"'
BUILDINFO += -DPACKAGE='"$(PACKAGE)"'
BUILDINFO += -DPACKAGE_NAME='"$(PACKAGE_NAME)"'
BUILDINFO += -DREV_MAJOR=$(REV_MAJOR)
BUILDINFO += -DREV_MINOR=$(REV_MINOR)
BUILDINFO += -DREV_PATCH=$(REV_PATCH)
BUILDINFO += -DPACKAGE_VERSION='"$(REV_MAJOR).$(REV_MINOR).$(REV_PATCH)"'
BUILDINFO += -DPACKAGE_STRING='"$(PACKAGE_NAME) $(REV_MAJOR).$(REV_MINOR).$(REV_PATCH)"'
BUILDINFO += -DBUILDNUM=$(shell date +"%y%m%d")
BUILDINFO += -DBRANCH='"$(shell cd $(SOURCE) && git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 _ | sed 's/_+/_/g;s/_$$//;s/^_//')"'
BUILDINFO += -DREV_YEAR=2021
BUILDINFO += -DBUILD_NAME='"$(shell cd $(SOURCE) && git remote | head -n 1)"'
BUILDINFO += -DBUILD_URL='"$(shell cd $(SOURCE) && git remote -v | head -n 1 | cut -f1 -d' ' | cut -f2 | sed -e 's/\.git//')"'
BUILDINFO += -DBUILD_BRANCH='"$(shell cd $(SOURCE) && git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 _ | sed 's/_+/_/g;s/_$$//;s/^_//')"'
BUILDINFO += -DBUILD_SYSTEM='"$(shell uname -s)"'
BUILDINFO += -DBUILD_RELEASE='"$(shell uname -sr)"'
BUILDINFO += -DBUILD_ID='"$(shell cd $(SOURCE) && git log -n 1 | sed -n '1s/^commit //p')"'
BUILDINFO += -DBUILD_OPTIONS='""'
BUILDINFO += -DBUILD_STATUS='""'

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

