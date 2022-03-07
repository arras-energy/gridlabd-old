include gldcore/link/engine/Makefile.mk
include gldcore/link/matlab/Makefile.mk
include gldcore/link/python/Makefile.mk
include gldcore/rt/Makefile.mk
include gldcore/converters/Makefile.mk
include gldcore/solvers/Makefile.mk
include gldcore/scripts/Makefile.mk

dist_pkgdata_DATA += gldcore/tzinfo.txt
dist_pkgdata_DATA += gldcore/unitfile.txt
dist_pkgdata_DATA += gldcore/gridlabd.cnf

gridlabd_bin_SOURCES = 
gridlabd_bin_SOURCES += gldcore/aggregate.cpp gldcore/aggregate.h
gridlabd_bin_SOURCES += gldcore/class.cpp gldcore/class.h
gridlabd_bin_SOURCES += gldcore/cmdarg.cpp gldcore/cmdarg.h
gridlabd_bin_SOURCES += gldcore/compare.cpp gldcore/compare.h
gridlabd_bin_SOURCES += gldcore/complex.h
gridlabd_bin_SOURCES += gldcore/console.h
gridlabd_bin_SOURCES += gldcore/convert.cpp gldcore/convert.h
gridlabd_bin_SOURCES += gldcore/curl.cpp gldcore/curl.h
gridlabd_bin_SOURCES += gldcore/daemon.cpp gldcore/daemon.h
gridlabd_bin_SOURCES += gldcore/debug.cpp gldcore/debug.h
gridlabd_bin_SOURCES += gldcore/deltamode.cpp gldcore/deltamode.h
gridlabd_bin_SOURCES += gldcore/enduse.cpp gldcore/enduse.h
gridlabd_bin_SOURCES += gldcore/environment.cpp gldcore/environment.h
gridlabd_bin_SOURCES += gldcore/exception.cpp gldcore/exception.h
gridlabd_bin_SOURCES += gldcore/exec.cpp gldcore/exec.h
gridlabd_bin_SOURCES += gldcore/find.cpp gldcore/find.h
gridlabd_bin_SOURCES += gldcore/gld_sock.h
gridlabd_bin_SOURCES += gldcore/globals.cpp gldcore/globals.h
gridlabd_bin_SOURCES += gldcore/gridlabd.h
gridlabd_bin_SOURCES += gldcore/gui.cpp gldcore/gui.h
gridlabd_bin_SOURCES += gldcore/http_client.cpp gldcore/http_client.h
gridlabd_bin_SOURCES += gldcore/index.cpp gldcore/index.h
gridlabd_bin_SOURCES += gldcore/instance.cpp gldcore/instance.h
gridlabd_bin_SOURCES += gldcore/instance_cnx.cpp gldcore/instance_cnx.h
gridlabd_bin_SOURCES += gldcore/instance_slave.cpp gldcore/instance_slave.h
gridlabd_bin_SOURCES += gldcore/interpolate.cpp gldcore/interpolate.h
gridlabd_bin_SOURCES += gldcore/job.cpp gldcore/job.h
gridlabd_bin_SOURCES += gldcore/json.cpp gldcore/json.h
gridlabd_bin_SOURCES += gldcore/kill.cpp gldcore/kill.h
gridlabd_bin_SOURCES += gldcore/kml.cpp gldcore/kml.h
gridlabd_bin_SOURCES += gldcore/legal.cpp gldcore/legal.h
gridlabd_bin_SOURCES += gldcore/linkage.cpp gldcore/linkage.h
gridlabd_bin_SOURCES += gldcore/link.cpp gldcore/link.h
gridlabd_bin_SOURCES += gldcore/list.cpp gldcore/list.h
gridlabd_bin_SOURCES += gldcore/load.cpp gldcore/load.h
gridlabd_bin_SOURCES += gldcore/loadshape.cpp gldcore/loadshape.h
gridlabd_bin_SOURCES += gldcore/local.cpp gldcore/local.h
gridlabd_bin_SOURCES += gldcore/lock.cpp gldcore/lock.h
gridlabd_bin_SOURCES += gldcore/main.cpp gldcore/main.h
gridlabd_bin_SOURCES += gldcore/match.cpp gldcore/match.h
gridlabd_bin_SOURCES += gldcore/matlab.cpp gldcore/matlab.h
gridlabd_bin_SOURCES += gldcore/module.cpp gldcore/module.h
gridlabd_bin_SOURCES += gldcore/object.cpp gldcore/object.h
gridlabd_bin_SOURCES += gldcore/output.cpp gldcore/output.h
gridlabd_bin_SOURCES += gldcore/platform.h
gridlabd_bin_SOURCES += gldcore/property.cpp gldcore/property.h
gridlabd_bin_SOURCES += gldcore/python_embed.cpp gldcore/python_embed.h
gridlabd_bin_SOURCES += gldcore/python_property.cpp gldcore/python_property.h
gridlabd_bin_SOURCES += gldcore/random.cpp gldcore/random.h
gridlabd_bin_SOURCES += gldcore/realtime.cpp gldcore/realtime.h
gridlabd_bin_SOURCES += gldcore/sanitize.cpp gldcore/sanitize.h
gridlabd_bin_SOURCES += gldcore/save.cpp gldcore/save.h
gridlabd_bin_SOURCES += gldcore/schedule.cpp gldcore/schedule.h
gridlabd_bin_SOURCES += gldcore/server.cpp gldcore/server.h
gridlabd_bin_SOURCES += gldcore/setup.cpp gldcore/setup.h
gridlabd_bin_SOURCES += gldcore/stream.cpp gldcore/stream.h
gridlabd_bin_SOURCES += gldcore/stream_type.h
gridlabd_bin_SOURCES += gldcore/test.cpp gldcore/test.h
gridlabd_bin_SOURCES += gldcore/threadpool.cpp gldcore/threadpool.h
gridlabd_bin_SOURCES += gldcore/timestamp.cpp gldcore/timestamp.h
gridlabd_bin_SOURCES += gldcore/transform.cpp gldcore/transform.h
gridlabd_bin_SOURCES += gldcore/unit.cpp gldcore/unit.h
gridlabd_bin_SOURCES += gldcore/validate.cpp gldcore/validate.h
gridlabd_bin_SOURCES += gldcore/version.cpp gldcore/version.h
gridlabd_bin_SOURCES += gldcore/link/python/python.cpp

EXTRA_gridlabd_bin_SOURCES =
EXTRA_gridlabd_bin_SOURCES += gldcore/cmex.c gldcore/cmex.h
EXTRA_gridlabd_bin_SOURCES += gldcore/ufile.c gldcore/ufile.h
EXTRA_gridlabd_bin_SOURCES += gldcore/xcore.cpp gldcore/xcore.h

bin_PROGRAMS += gridlabd.bin

gridlabd_bin_CPPFLAGS = -DMAIN_PYTHON -fPIC
gridlabd_bin_CPPFLAGS += $(XERCES_CPPFLAGS)
gridlabd_bin_CPPFLAGS += $(AM_CPPFLAGS)

gridlabd_bin_LDFLAGS =
gridlabd_bin_LDFLAGS += $(XERCES_LDFLAGS)
gridlabd_bin_LDFLAGS += $(AM_LDFLAGS)

gridlabd_bin_LDADD =
gridlabd_bin_LDADD += $(XERCES_LIB)
gridlabd_bin_LDADD += $(CURSES_LIB)
gridlabd_bin_LDADD += -ldl -lcurl

bin_SCRIPTS += gldcore/gridlabd 

gridlabd_bin_SOURCES += gldcore/build.h
BUILT_SOURCES += gldcore/build.h
CLEANFILES += gldcore/build.h origin.txt

pkginclude_HEADERS =
pkginclude_HEADERS += gldcore/build.h
pkginclude_HEADERS += gldcore/class.h
pkginclude_HEADERS += gldcore/complex.h
pkginclude_HEADERS += gldcore/debug.h
pkginclude_HEADERS += gldcore/enduse.h
pkginclude_HEADERS += gldcore/exception.h
pkginclude_HEADERS += gldcore/loadshape.h
pkginclude_HEADERS += gldcore/lock.h
pkginclude_HEADERS += gldcore/module.h
pkginclude_HEADERS += gldcore/object.h
pkginclude_HEADERS += gldcore/property.h
pkginclude_HEADERS += gldcore/schedule.h
pkginclude_HEADERS += gldcore/test.h
pkginclude_HEADERS += gldcore/version.h

gridlabddir = $(prefix)/share/gridlabd
gridlabd_DATA = origin.txt

gldcore/gridlabd.in: gldcore/gridlabd.m4sh
	@autom4te -l m4sh $< > $@
	@echo "$@ updated, please run 'make reconfigure && make TARGET' again" && false

gldcore/build.h: buildnum

buildnum: utilities/build_number
	@/bin/bash -c "$(top_srcdir)/utilities/build_number"
	@/bin/bash -c "$(top_srcdir)/utilities/update_origin" > origin.txt

weather:
	@(echo "Installing weather data manager" && mkdir -p $(prefix)/share/gridlabd/weather && chmod 2777 $(prefix)/share/gridlabd/weather && chmod 1755 $(bindir)/gridlabd-weather)
	@(echo "Updating weather data index" && export GLD_ETC=$(prefix)/share/gridlabd && $(bindir)/gridlabd-weather fetch_index)

template: # TODO

library: # TODO	
