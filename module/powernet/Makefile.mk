# powernet/Makefile.mk
# Copyright (C) 2016, Stanford University
# Author: dchassin@slac.stanford.edu

pkglib_LTLIBRARIES += module/powernet/powernet.la

module_powernet_powernet_la_CPPFLAGS =
module_powernet_powernet_la_CPPFLAGS += $(AM_CPPFLAGS)

module_powernet_powernet_la_LDFLAGS =
module_powernet_powernet_la_LDFLAGS += $(AM_LDFLAGS)

module_powernet_powernet_la_LIBADD = 

module_powernet_powernet_la_SOURCES = module/powernet/main.cpp
module_powernet_powernet_la_SOURCES += module/powernet/agent.cpp module/powernet/agent.h
module_powernet_powernet_la_SOURCES += module/powernet/house.cpp module/powernet/house.h
module_powernet_powernet_la_SOURCES += module/powernet/message.cpp module/powernet/message.h
