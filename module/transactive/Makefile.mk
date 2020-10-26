# transactive/Makefile.mk
# Copyright (C) 2016, Stanford University
# Author: dchassin@slac.stanford.edu

pkglib_LTLIBRARIES += module/transactive/transactive.la

module_transactive_transactive_la_CPPFLAGS =
module_transactive_transactive_la_CPPFLAGS += $(AM_CPPFLAGS)

module_transactive_transactive_la_LDFLAGS =
module_transactive_transactive_la_LDFLAGS += $(AM_LDFLAGS)

module_transactive_transactive_la_LIBADD = 

module_transactive_transactive_la_SOURCES =
module_transactive_transactive_la_SOURCES += module/transactive/house.cpp 
module_transactive_transactive_la_SOURCES += module/transactive/house.h
module_transactive_transactive_la_SOURCES += module/transactive/agent.cpp 
module_transactive_transactive_la_SOURCES += module/transactive/agent.h
module_transactive_transactive_la_SOURCES += module/transactive/main.cpp
