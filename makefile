# makefile
#
# GridLAB-D makefile
#
# USAGE
#
#   make [TARGETS ...]
#
# DESCRIPTION
#
#   The build process first creates the intermediate files in the BUILD folder.  Then the
#   application files are copied to the PREFIX folder.  Finally, the permissions are set and
#   the install files are linked in the INSTALL folder through the PREFIX/current link.
#
#   Links create by 'install' target:
#
#      BUILD/bin/gridlabd --> PREFIX/current/bin/gridlabd
#      BUILD/lib/gridlabd --> PREFIX/current/lib/
#      BUILD/include/gridlabd --> PREFIX/current/include
#      BUILD/share/doc/gridlabd --> PREFIX/current/doc
#      BUILD/share/gridlabd --> PREFIX/current/share
# 
# FILES
#
#   FOLDERS/makefile       Makefiles for subfolders (automatically included)
#   SOURCE/makefile.conf   Delivery options (optional, included if found in source folder)
#   makefile.conf          Configuration makefile (optional, included if found in current folder)
#
# OPTIONS
#
#   VERBOSE = no           Disables verbose output during build
#   SILENT = yes           Enables silent processing (only errors are output)
#   PYTHON_EXEC = pathname Specify the python executable to use when running applications
#   BUILD = path/          Specify where the build intermediate files are maintained
#   PREFIX = path/         Specify where the application folder is created
#   INSTALL = path/        Specify where the application install links are created
#

# system requirements
PYTHON_MAJOR_REQ = 3
PYTHON_MINOR_REQ = 9
DARWIN_MAJOR_REQ = 18
LINUX_MAJOR_REQ = 5

# default target
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

#
# Check version of system and tools
#

SYSTEM = $(shell uname -s)

ifeq ($(SYSTEM),Darwin)

DARWIN = $(shell uname -r)
DARWIN_MAJOR = $(shell echo $(DARWIN) | cut -f1 -d.)
DARWIN_OK = $(shell test $(DARWIN_MAJOR) -ge $(DARWIN_MAJOR_REQ) && echo yes || echo no)
ifeq ($(DARWIN_OK),no)
$(error build requires Darwin $(DARWIN_MAJOR_REQ).0 or better (your system is Darwin $(DARWIN)))
endif

else

ifeq ($(SYSTEM),Linux)

LINUX = $(shell uname -r)
LINUX_MAJOR = $(shell echo $(LINUX) | cut -f1 -d.)
LINUX_OK = $(shell test $(LINUX_MAJOR) -ge $(LINUX_MAJOR_REQ) && echo yes || echo no)
ifeq ($(LINUX_OK),no)
$(error build requires Linux $(LINUX_MAJOR_REQ).0 or better (your system is Darwin $(LINUX)))
endif

endif

endif

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
ifndef BUILD
ifeq ($(SOURCE),$(PWD)/)
BUILD = $(PWD)/build/
else
BUILD = $(PWD)/
endif
else
ifneq ($(shell basename $(BUILD)x),x)
BUILD+=/
endif
endif

####################################################
#
# Make configuration
#

# Default configuration
include $(SOURCE)makefile.conf

# Local configuration
-include ./makefile.conf

# control echo of commands
ifeq ($(VERBOSE),yes)
ECHO = 
else
ECHO = @
endif

# control output of progress
ifdef ($(SILENT),yes)
INFO = @true
else 
INFO = @echo
endif

####################################################
#
# Build options
#

# locate and verify python system
ifndef PYTHON_EXEC
PYTHON_EXEC = $(shell which python3)
ifeq ($(PYTHON_EXEC),)
$(error "python3 not found")
endif
endif
PYTHON_CONFIG = $(PYTHON_EXEC)-config
PYTHON_VERSION = $(shell $(PYTHON_EXEC) --version | cut -f2 -d' ')
PYTHON_MAJOR = $(shell echo $(PYTHON_VERSION) | cut -f1 -d.)
PYTHON_MINOR = $(shell echo $(PYTHON_VERSION) | cut -f2 -d.)
PYTHON_OK = $(shell test $(PYTHON_MAJOR) -ge $(PYTHON_MAJOR_REQ) -a $(PYTHON_MINOR) -ge $(PYTHON_MINOR_REQ) && echo yes || echo no)
ifeq ($(PYTHON_OK),no)
$(error Python $(PYTHON_MAJOR).$(PYTHON_MINOR) is not supported (must be $(PYTHON_MAJOR_REQ).$(PYTHON_MINOR_REQ) or higher))
endif

# default C++ flags
ifndef CFLAGS
CFLAGS = -W -O2 -g
endif
CFLAGS += -fPIC

# default link flags
ifndef LDFLAGS
LDFLAGS =
endif

# default archive flags
ifndef ARFLAGS
ifeq ($(SYSTEM),Darwin)
ARFLAGS = -dynamiclib
else
ARFLAGS = -shared
endif
endif

# default shared object flags
ifndef SOFLAGS
SOFLAGS =
endif

####################################################
#
# Build folders and targets
#

# install prefix (actual install folder)
ifndef PREFIX
PREFIX = /usr/local/opt/$(PACKAGE)/$(VERSION)-$(BUILDNUM)-$(BRANCH)/
else
ifneq ($(shell basename $(PREFIX)x),x)
PREFIX+=/
endif
endif

# target lists
INCLUDE_DIRS = # list of folders that must be included when building modules
BUILD_TARGETS = # list of files that must be built
INSTALL_TARGETS = # list of files that must be installed
CLEAN_TARGETS = # list of files that must be deleted when cleaning
BUILD_FOLDERS = # folders that must be included in build process

# important settings
PREFIX_FOLDERS = $(PREFIX){bin,lib,share,include,doc}
PYTHONCFLAGS = $(shell python3-config --cflags) 
PYTHONLDFLAGS = $(shell python3-config --ldflags)

# version/build/branch info
VERSION = $(REV_MAJOR).$(REV_MINOR).$(REV_PATCH)
BUILDNUM = $(shell date +"%y%m%d")
ifneq ($(shell which git),)
BRANCH = $(shell cd $(SOURCE) && git rev-parse --abbrev-ref HEAD | git rev-parse --abbrev-ref HEAD | tr -c A-Za-z0-9 _ | sed 's/_+/_/g;s/_$$//;s/^_//')
else
BRANCH = local
endif

# macros defined in C++ compiler commands
BUILDINFO =
BUILDINFO += -DPYTHON_EXEC='"$(PYTHON_EXEC)"'
BUILDINFO += -DPACKAGE='"$(PACKAGE)"'
BUILDINFO += -DPACKAGE_NAME='"$(PACKAGE_NAME)"'
BUILDINFO += -DREV_MAJOR=$(REV_MAJOR)
BUILDINFO += -DREV_MINOR=$(REV_MINOR)
BUILDINFO += -DREV_PATCH=$(REV_PATCH)
BUILDINFO += -DPACKAGE_VERSION='"$(VERSION)"'
BUILDINFO += -DPACKAGE_STRING='"$(PACKAGE_NAME) $(REV_MAJOR).$(REV_MINOR).$(REV_PATCH)"'
BUILDINFO += -DBUILDNUM=$(BUILDNUM)
BUILDINFO += -DBRANCH='"$(BRANCH)"'
BUILDINFO += -DREV_YEAR=2021
BUILDINFO += -DBUILD_BRANCH='"$(BRANCH)"'
BUILDINFO += -DBUILD_SYSTEM='"$(shell uname -s)"'
BUILDINFO += -DBUILD_RELEASE='"$(shell uname -sr)"'
ifneq ($(shell which git),)
BUILDINFO += -DBUILD_NAME='"$(shell cd $(SOURCE) && git remote | head -n 1)"'
BUILDINFO += -DBUILD_URL='"$(shell cd $(SOURCE) && git remote -v | head -n 1 | cut -f1 -d' ' | cut -f2 | sed -e 's/\.git//')"'
BUILDINFO += -DBUILD_ID='"$(shell cd $(SOURCE) && git log -n 1 | sed -n '1s/^commit //p')"'
else
BUILDINFO += -DBUILD_NAME='"local"'
BUILDINFO += -DBUILD_URL='"file:://$PWD"'
BUILDINFO += -DBUILD_ID='"local"'	
endif
BUILDINFO += -DBUILD_OPTIONS='""'
BUILDINFO += -DBUILD_STATUS='""'

# subfolder makefiles to include
BIN=$(PREFIX)bin/
ETC=$(PREFIX)share/
LIB=$(PREFIX)lib/
DOC=$(PREFIX)doc/
INC=$(PREFIX)include/
INCLUDE = $(shell $(SOURCE)utilities/configure $(SOURCE)$(MAKEFILE))
include $(INCLUDE)
PREFIX_TARGETS += $(BIN_TARGETS) $(ETC_TARGETS) $(LIB_TARGETS) $(DOC_TARGETS) $(INC_TARGETS)

####################################################
#
# License and copyright targets
#
$(PREFIX)LICENSE: $(SOURCE)LICENSE
	$(INFO) "  TXT $@"
	$(ECHO)cp $< $@

$(PREFIX)COPYRIGHT: $(SOURCE)COPYRIGHT
	$(INFO) "  TXT $@"
	$(ECHO)$(SOURCE)utilities/replace -i $< -o $@ MAJOR=$(REV_MAJOR) MINOR=$(REV_MINOR) YEAR=$(shell date +'%Y')

BUILD_TARGETS += $(PREFIX)LICENSE $(PREFIX)COPYRIGHT

# target to build application folder
prefix: $(BUILD_TARGETS) $(PREFIX_TARGETS)

####################################################
#
# Install options
#

# install folder (links to install folder)
ifndef INSTALL
INSTALL = /usr/local
else
ifneq ($(shell basename $(INSTALL)x),x)
INSTALL+=/
endif
endif

# aggregate targets
INSTALL_TARGETS += $(BIN_TARGETS) $(ETC_TARGETS) $(LIB_TARGETS) $(DOC_TARGETS) $(INC_TARGETS)

# install rules
$(INSTALL)%: $(SOURCE)%
	$(ECHO)cp $< %@

####################################################
#
# Clean options
#

# rm options for clean targets
ifndef CLEAN
CLEAN = rm -rf
endif

# standard clean targets
CLEAN_TARGETS += $(PREFIX_TARGETS) $(BUILD_TARGETS) $(GLDCORE_OBJECTS)

# create build folders
$(shell mkdir -p $(BUILD_FOLDERS) $(PREFIX_FOLDERS))

####################################################
#
# Standard targets
#

info: 
	@echo PACKAGE_NAME = $(PACKAGE_NAME)
	@echo PACKAGE = $(PACKAGE)
	@echo SOURCE = $(SOURCE)
	@echo BUILD = $(BUILD)
	@echo PREFIX = $(PREFIX)
	@echo INSTALL = $(INSTALL)

info_targets: info
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

build: prefix $(PREFIX)LICENSE $(PREFIX)COPYRIGHT
	$(INFO) $(BUILD) ok

install: prefix $(INSTALL_TARGETS)
	$(error install target not implemented)
	$(INFO) $(INSTALL) ok

clean: 
	@$(CLEAN) $(CLEAN_TARGETS)
	$(INFO) $(PREFIX) clean
	@$(CLEAN) $(BUILD_FOLDERS) 
	$(INFO) $(BUILD) clean

