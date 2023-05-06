# File: Makefile.am
#
# GridLAB-D 4.0
# Copyright (C) 2008 Battelle Memorial Institute
# This file is distributed under the same terms as GridLAB-D.
#
# HiPAS GridLAB-D 4.2
#   Copyright (C) 2020 Regents of the Leland Stanford Junior University
#
# Authors:
#   DP Chassin (dchassin@slac.stanford.edu)

$(info Current target is $(shell $(top_srcdir)/build-aux/version.sh --name))

# !!! must match version specified in configure.ac
OPT=$(shell mkdir -p /usr/local/opt/gridlabd)
PYVER = $(shell $(top_srcdir)/build-aux/version.sh --python)
PYCFLAGS = $(shell python$(PYVER)-config --cflags)
PYLDFLAGS = $(shell python$(PYVER)-config --ldflags)

SYSPYTHON = python$(PYVER)
PYTHONCONFIG = $(SYSPYTHON)-config

AM_INIT_AUTOMAKE = subdir-objects 
AUTOMAKE_OPTIONS = -Wno-portability
ACLOCAL_AMFLAGS = -I m4

CC = $(PTHREAD_CC)
CXX = $(PTHREAD_CXX)

AM_CPPFLAGS = -Wall -fPIC
AM_CPPFLAGS += -I$(top_srcdir)/source #-I$(top_srcdir)/build-dir/source
AM_CPPFLAGS += $(GLD_CPPFLAGS)
AM_CPPFLAGS += $(PYCFLAGS)

AM_CFLAGS = -Wall
AM_CFLAGS +=  -I$(top_srcdir)/source #-I$(top_srcdir)/build-dir/source
AM_CFLAGS += $(PTHREAD_CFLAGS)
AM_CFLAGS += $(GLD_CFLAGS)  #-I$(top_srcdir)/build-dir/source
AM_CFLAGS += $(PYCFLAGS)

AM_CXXFLAGS = -Wall -fPIC
AM_CXXFLAGS += $(PTHREAD_CFLAGS)
AM_CXXFLAGS += $(GLD_CXXFLAGS) #-I$(top_srcdir)/build-dir/source
AM_CXXFLAGS += $(PYCFLAGS)

AM_LDFLAGS = $(PYLDFLAGS)
AM_LDFLAGS += -no-undefined
AM_LDFLAGS += -module
AM_LDFLAGS += -avoid-version
AM_LDFLAGS += -export-dynamic
AM_LDFLAGS += $(GLD_LDFLAGS) -lpython$(PYVER)

# makefile fragments populate these variables
BUILT_SOURCES = 
CLEANFILES =
EXTRA_DIST =
dist_pkgdata_DATA = requirements.txt
pkgdata_DATA =
noinst_LTLIBRARIES =
pkglib_LTLIBRARIES =
bin_PROGRAMS =
bin_SCRIPTS =
dist_doc_DATA =

docs_targets =

check-target:
	@test $(prefix) = $(shell build-aux/version.sh --install) || ( echo "ERROR [Makefile]: prefix and target mismatch; ./configure is outdated. Build using '--clean' option to fix this." > /dev/stderr ; false )

# required modules
include $(top_srcdir)/third_party/Makefile.mk
include $(top_srcdir)/source/Makefile.mk
include $(top_srcdir)/runtime/Makefile.mk
include $(top_srcdir)/converters/Makefile.mk
include $(top_srcdir)/tools/Makefile.mk
include $(top_srcdir)/cloud/Makefile.mk
include $(top_srcdir)/utilities/Makefile.mk
include $(top_srcdir)/python/Makefile.mk
include $(top_srcdir)/geodata/Makefile.mk
include $(top_srcdir)/subcommands/Makefile.mk

# approved modules
include $(top_srcdir)/modules.mk

BUILT_SOURCES += python-requirements

#
# Post install report
#

install-exec-hook: | index docs
	@cp ${top_srcdir}/{COPYRIGHT,LICENSE} $(prefix)
	@echo ""
	@echo "Install complete. Here are some useful commands now:"
	@echo ""
	@echo "To include this version in your command path:"
	@echo "  export PATH='$(DESTDIR)$(bindir):$$PATH'"
	@echo ""
	@echo "To run this version directly without using the command path:"
	@echo "  $(DESTDIR)$(bindir)/gridlabd"
	@echo ""
	@echo "To make this version the default for all users on this system:"
	@echo "  $(DESTDIR)$(bindir)/gridlabd version set $(shell basename $(exec_prefix))"
	@echo ""

#
# Python setup
#

PYENV=$(bindir)/pkgenv
PYBIN=$(PYENV)/bin
PYLIB=$(PYENV)/lib
PYINC=$(PYENV)/include
ENVPYTHON=$(PYBIN)/python$(PYVER)
PYACTIVATE=source $(PYBIN)/activate

python-requirements: | $(PYENV) python-venv check-target

python-venv:
	@test ! -z "$(VIRTUAL_ENV)" || ( echo "ERROR [Makefile]: no active python venv; did you forget to run '. $(HOME)/.gridlabd/bin/activate'" > /dev/stderr ; false )

# NB: it's ok to use the runner's python to generate the requirements.txt file
requirements.txt: python/requirements.csv
	@python3 -m pip -q install pandas==2.0.0
	@python3 python/requirements.py $< >$@

$(PYENV): requirements.txt
	@$(SYSPYTHON) --version 1>/dev/null || ( echo "ERROR [Makefile]: $(SYSPYTHON) is not found" > /dev/stderr ; false )
	@python$(PYVER)-config --prefix 1>/dev/null || ( echo "ERROR [Makefile]: python$(PYVER)-dev is not installed" > /dev/stderr ; false )
	@echo "Processing python requirements..."
	@mkdir -p $(PYBIN) $(PYLIB) $(PYINC)
	@$(SYSPYTHON) -m venv --symlinks $(PYENV)
	@$(ENVPYTHON) --version 1>/dev/null || ( echo "ERROR [Makefile]: $(ENVPYTHON) is not found" > /dev/stderr ; false )
	@$(ENVPYTHON) -m pip install --upgrade pip
	@$(ENVPYTHON) -m pip install pandas==2.0.0
	@$(ENVPYTHON) python/requirements.py --buildenv | bash
	@$(ENVPYTHON) -m pip install -r requirements.txt 
	@touch $(PYENV)

# for doxygen commands
include $(top_srcdir)/aminclude.mk

dist_doc_DATA += COPYRIGHT
dist_doc_DATA += LICENSE

help:
	@echo "Cleaning targets:"
	@echo "  clean          - Erase from the build tree the files built by 'make all'"
	@echo "  distclean      - Same as 'make clean' plus erase anything ./configure created"
	@echo "  mostlyclean    - Erase intermediate files (such as .o files)"
	@echo ""
	@echo "Building targets:"
	@echo "  all            - Build $(PACKAGE) (same as 'make')"
	@echo "  install        - Install $(PACKAGE) to $(prefix)"
	@echo "  system         - Build and install $(PACKAGE) to the system folders"
	@echo "  install-strip  - Same as install, but follow by stripping debug symbols"
	@echo "  install-clean  - Same as install-system, but runs distclean, autoreconf, and configure first"
	@echo "  install-system - Same as install, but links target to /usr/local for all system users"
	@echo "  uninstall      - Erase files installed by 'make install'"
	@echo ""
	@echo "Cleaning targets:"
	@echo "  clean        - Erase from the build tree the files built by 'make all'"
	@echo "  distclean    - Same as 'make clean' plus erase anything ./configure created"
	@echo "  mostlyclean  - Erase intermediate files (such as .o files)"
	@echo ""
	@echo "Testing targets:"
	@echo "  validate  - Run the test/validation suite (requires Python)"
	@echo ""
	@echo "Install images:"
	@echo "  image             - Create install image"
	@echo "  aws-image         - Upload AWS install image"
	@echo "  aws-image-default - Upload AWS install image as default for this platform"
	@wcho "  aws-install       - Upload AWS install script"
	@echo ""
	@echo "Documentation targets:"
	@echo "  html                 - Generate all html documentation"
	@echo "  module-html          - Generate module documentation (only if doxygen was found"
	@echo "                         by ./configure)"
	@echo "  developer-html       - Generate developer documentation (only if natural-docs was"
	@echo "                         found by ./configure)"
	@echo "  troubleshooting-html - Generate troubleshooting documentation (only if gawk was"
	@echo "                         was found by ./configure)"
	@echo ""
	@echo "Data targets:"
	@echo "  index        - Download all indexes from data archives"
	@echo "  weather      - Download the weather data index from the weather archive"
	@echo "  library      - Download the library index from the library archive"
	@echo "  template     - Download the template index from the template archive"
	@echo ""
	@echo "Developer targets (may require special tools):"
	@echo "  clean-wc   - Recursively remove all files and directories not under"
	@echo "               revision control (will prompt to confirm first)"
	@echo "  ctags      - Generate vi-style tags files"
	@echo "  tags       - Generate emacs-style TAGS files"
	@echo "  maintainer-clean"

over: clean all

S3BUCKET=$(shell $(top_srcdir)/build-aux/version.sh --bucket)
LONGNAME=$(shell $(top_srcdir)/build-aux/version.sh --name)
SHORTNAME=$(shell $(top_srcdir)/build-aux/version.sh --sysspec)

image: $(prefix).tarz

$(prefix).tarz: $(prefix)
	@echo "Creating $(prefix).tarz..."
	@grep INSTALL $(top_srcdir)/setup/$(shell $(top_srcdir)/build-aux/version.sh --sysspec).sh 1>$(prefix)/share/gridlabd/setup.sh
	@cd $(dir $(prefix)); tar cfz $(LONGNAME).tarz $(notdir $(prefix))

aws-image: $(prefix).tarz
	@aws --version || ( echo "ERROR [Makefile]: aws CLI is required to upload image" >/dev/stderr ; false )
	@aws s3 cp $(realpath $(prefix).tarz) s3://$(S3BUCKET)/$(LONGNAME).tarz --acl public-read

aws-image-default: aws-image
	@aws --version || ( echo "ERROR [Makefile]: aws CLI is required to create default image" >/dev/stderr ; false )
	@aws s3 cp s3://$(S3BUCKET)/$(LONGNAME).tarz s3://$(S3BUCKET)/$(SHORTNAME).tarz --acl public-read

aws-install: 
	@aws --version || ( echo "ERROR [Makefile]: aws CLI is required to upload installer" >/dev/stderr ; false )
	@aws s3 cp $(top_srcdir)/install.sh s3://$(S3BUCKET)/install.sh --acl public-read


# Recursively remove all files and directories not under revision control
clean-wc:
	@echo -e "\nThis removes all unversioned files and directories in the working copy."
	@unset REPLY && read -t 60 -p "Clean working copy (type 'yes' to proceed)? " && test "`echo "$$REPLY" | tr '[:upper:]' '[:lower:]'`" = "yes"
	. $(top_srcdir)/utilities/cleanwc

install-validate: install validate

check-local validate:
	@(export LD_LIBRARY_PATH=.:$${LD_LIBRARY_PATH:-${libdir}}; ${bindir}/gridlabd -D keep_progress=TRUE --validate -D keep_progress=TRUE || (utilities/save_validation_output;exit 1))

clean-local: python-clean
	@rm -rf $(top_srcdir)/documents/html
	@rm -rf $(top_srcdir)/validate.txt
	@rm -f $(top_srcdir)/gridlabd.spec
	@rm -f $(top_srcdir)/gridlabd.bin origin.txt
	@rm -rf $(top_srcdir)/build
	@(cd $(top_srcdir) ; build-aux/clean_dirs.sh)

distclean-local: python-clean
	@(cd $(top_srcdir) ; build-aux/clean_dirs.sh --depends)
	@rm -rf $(top_srcdir)/autom4te.cache
	@rm -f $(top_srcdir)/aclocal.m4
	@rm -f $(top_srcdir)/autoscan.log
	@rm -f $(top_srcdir)/m4/libtool.m4
	@rm -f $(top_srcdir)/ltoptions.m4
	@rm -f $(top_srcdir)/ltsugar.m4
	@rm -f $(top_srcdir)/ltversion.m4
	@rm -f $(top_srcdir)/lt~obsolete.m4
	@rm -f $(top_srcdir)/configure
	@rm -f $(top_srcdir)/config.log
	@rm -f $(top_srcdir)/config.status
	@rm -f $(top_srcdir)/gridlabd.spec
	@rm -f $(top_srcdir)/libtool
	@rm -f $(top_srcdir)/Makefile
	@rm -f $(top_srcdir)/Makefile.in
	@rm -f $(top_srcdir)/build-aux/ar-lib
	@rm -f $(top_srcdir)/build-aux/compile
	@rm -f $(top_srcdir)/build-aux/config.log
	@rm -f $(top_srcdir)/build-aux/config.guess
	@rm -f $(top_srcdir)/build-aux/config.sub
	@rm -f $(top_srcdir)/build-aux/install-sh
	@rm -f $(top_srcdir)/build-aux/ltmain.sh
	@rm -f $(top_srcdir)/build-aux/missing
	@rm -f $(top_srcdir)/build-aux/depcomp
	@rm -f $(top_srcdir)/gridlabd.spec
	@rm -f $(top_srcdir)/gridlabd.bin origin.txt
	@rm -rf $(top_srcdir)/build
	@rm -rf $(top_srcdir)/documents/html
	@rm -f $(top_srcdir)/validate.txt

libtool: $(LIBTOOL_DEPS)
	$(SHELL) $(PWD)/config.status libtool > /dev/null

#
# Run this after changes that affect build-aux/version.sh output
#

docker: $(PREFIX)/docker.img

docker-debug: docker/Dockerfile Makefile
	docker build -f docker/Dockerfile -t $(PACKAGE)/$(PACKAGE_VERSION)-$(PACKAGE_BRANCH):latest --build-arg BRANCH=$(PACKAGE_ORIGIN) --build-arg RUN_VALIDATION=no docker
	@mkdir -p $(prefix)
	docker save $(PACKAGE)/$(PACKAGE_VERSION)-$(PACKAGE_BRANCH):latest -o $(prefix)/docker.img

$(PREFIX)/docker.img: docker/Dockerfile Makefile
	docker build -f docker/Dockerfile -t $(PACKAGE)/$(PACKAGE_VERSION)-$(PACKAGE_BRANCH):latest --build-arg BRANCH=$(PACKAGE_ORIGIN) --build-arg RUN_VALIDATION=yes docker
	@mkdir -p $(prefix)
	docker save $(PACKAGE)/$(PACKAGE_VERSION)-$(PACKAGE_BRANCH):latest -o $(prefix)/docker.img

system: python-install install
	@echo "Setting $$($(top_srcdir)/build-aux/version.sh --install) to the current system version"
	@$(prefix)/bin/gridlabd version set
	@test "$$(gridlabd --version=install)" = "$$($(top_srcdir)/build-aux/version.sh --install)" || echo 'WARNING : build and install versions differ; use build `--clean` option to fix this problem'

index: weather library template

html-local: module-html developer-html troubleshooting-html
	@mkdir -p $(prefix)/share/doc/developer
	@cp $(top_srcdir)/documents/index.html $(prefix)/share/doc/index.html
	@cp -R $(top_srcdir)/documents/html/* $(prefix)/share/doc/
	@cp -R $(top_srcdir)/source/html/* $(prefix)/share/doc/developer/

module-html:
	@mkdir -p $(top_srcdir)/documents/html/modules
	@doxygen $(top_srcdir)/doxygen/gridlabd.conf

developer-html: # TODO
	@(cd $(top_srcdir)/source/html ; natural_docs . ; cd - > /dev/null )

troubleshooting-html:
	@mkdir -p $(top_srcdir)/documents/html/troubleshooting/
	@(cd $(top_srcdir) ; gawk -f utilities/troubleshooting.awk */*.cpp > $(top_srcdir)/documents/html/troubleshooting/index.html )

documents/gridlabd.pdf: documents/gridlabd.glm
	(cd documents ; $(DESTDIR)$(bindir)/gridlabd gridlabd.glm | echo "WARNING: unable to update documents/gridlabd.pdf")
	test -f documents/gridlabd.md && pandoc -V geometry:landscape documents/gridlabd.md -o documents/gridlabd.pdf

docs: $(docs_targets) # documents/gridlabd.pdf

docs_toc:
	utilities/regen_toc >'docs/User manual/0 - Cover/1 - Contents.md'
