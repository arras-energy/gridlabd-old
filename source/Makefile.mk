include source/link/engine/Makefile.mk
include source/solvers/Makefile.mk

gridlabd_bin_SOURCES = 
gridlabd_bin_SOURCES += source/aggregate.cpp source/aggregate.h
gridlabd_bin_SOURCES += source/class.cpp source/class.h
gridlabd_bin_SOURCES += source/cmdarg.cpp source/cmdarg.h
gridlabd_bin_SOURCES += source/compare.cpp source/compare.h
gridlabd_bin_SOURCES += source/complex.h
gridlabd_bin_SOURCES += source/console.h
gridlabd_bin_SOURCES += source/convert.cpp source/convert.h
gridlabd_bin_SOURCES += source/curl.cpp source/curl.h
gridlabd_bin_SOURCES += source/daemon.cpp source/daemon.h
gridlabd_bin_SOURCES += source/debug.cpp source/debug.h
gridlabd_bin_SOURCES += source/deltamode.cpp source/deltamode.h
gridlabd_bin_SOURCES += source/enduse.cpp source/enduse.h
gridlabd_bin_SOURCES += source/environment.cpp source/environment.h
gridlabd_bin_SOURCES += source/exception.cpp source/exception.h
gridlabd_bin_SOURCES += source/exec.cpp source/exec.h
gridlabd_bin_SOURCES += source/find.cpp source/find.h
gridlabd_bin_SOURCES += source/gld_sock.h
gridlabd_bin_SOURCES += source/globals.cpp source/globals.h
gridlabd_bin_SOURCES += source/gridlabd.h
gridlabd_bin_SOURCES += source/gui.cpp source/gui.h
gridlabd_bin_SOURCES += source/http_client.cpp source/http_client.h
gridlabd_bin_SOURCES += source/index.cpp source/index.h
gridlabd_bin_SOURCES += source/instance.cpp source/instance.h
gridlabd_bin_SOURCES += source/instance_cnx.cpp source/instance_cnx.h
gridlabd_bin_SOURCES += source/instance_slave.cpp source/instance_slave.h
gridlabd_bin_SOURCES += source/interpolate.cpp source/interpolate.h
gridlabd_bin_SOURCES += source/job.cpp source/job.h
gridlabd_bin_SOURCES += source/json.cpp source/json.h
gridlabd_bin_SOURCES += source/kill.cpp source/kill.h
gridlabd_bin_SOURCES += source/kml.cpp source/kml.h
gridlabd_bin_SOURCES += source/legal.cpp source/legal.h
gridlabd_bin_SOURCES += source/linkage.cpp source/linkage.h
gridlabd_bin_SOURCES += source/link.cpp source/link.h
gridlabd_bin_SOURCES += source/list.cpp source/list.h
gridlabd_bin_SOURCES += source/load.cpp source/load.h
gridlabd_bin_SOURCES += source/loadshape.cpp source/loadshape.h
gridlabd_bin_SOURCES += source/local.cpp source/local.h
gridlabd_bin_SOURCES += source/lock.cpp source/lock.h
gridlabd_bin_SOURCES += source/main.cpp source/main.h
gridlabd_bin_SOURCES += source/match.cpp source/match.h
gridlabd_bin_SOURCES += source/matlab.cpp source/matlab.h
gridlabd_bin_SOURCES += source/module.cpp source/module.h
gridlabd_bin_SOURCES += source/object.cpp source/object.h
gridlabd_bin_SOURCES += source/output.cpp source/output.h
gridlabd_bin_SOURCES += source/platform.h
gridlabd_bin_SOURCES += source/property.cpp source/property.h
gridlabd_bin_SOURCES += source/python_embed.cpp source/python_embed.h
gridlabd_bin_SOURCES += source/python_property.cpp source/python_property.h
gridlabd_bin_SOURCES += source/random.cpp source/random.h
gridlabd_bin_SOURCES += source/realtime.cpp source/realtime.h
gridlabd_bin_SOURCES += source/sanitize.cpp source/sanitize.h
gridlabd_bin_SOURCES += source/save.cpp source/save.h
gridlabd_bin_SOURCES += source/schedule.cpp source/schedule.h
gridlabd_bin_SOURCES += source/server.cpp source/server.h
gridlabd_bin_SOURCES += source/setup.cpp source/setup.h
gridlabd_bin_SOURCES += source/stream.cpp source/stream.h
gridlabd_bin_SOURCES += source/stream_type.h
gridlabd_bin_SOURCES += source/test.cpp source/test.h
gridlabd_bin_SOURCES += source/threadpool.cpp source/threadpool.h
gridlabd_bin_SOURCES += source/timestamp.cpp source/timestamp.h
gridlabd_bin_SOURCES += source/transform.cpp source/transform.h
gridlabd_bin_SOURCES += source/unit.cpp source/unit.h
gridlabd_bin_SOURCES += source/validate.cpp source/validate.h
gridlabd_bin_SOURCES += source/version.cpp source/version.h
gridlabd_bin_SOURCES += python/python.cpp

EXTRA_gridlabd_bin_SOURCES =
EXTRA_gridlabd_bin_SOURCES += source/cmex.c source/cmex.h
EXTRA_gridlabd_bin_SOURCES += source/ufile.c source/ufile.h
EXTRA_gridlabd_bin_SOURCES += source/xcore.cpp source/xcore.h

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

bin_SCRIPTS += source/gridlabd 

gridlabd_bin_SOURCES += $(top_srcdir)/source/build.h
BUILT_SOURCES += $(top_srcdir)/source/build.h
CLEANFILES += $(top_srcdir)/source/build.h origin.txt

pkginclude_HEADERS =
pkginclude_HEADERS += $(top_srcdir)/source/build.h
pkginclude_HEADERS += source/class.h
pkginclude_HEADERS += source/complex.h
pkginclude_HEADERS += source/debug.h
pkginclude_HEADERS += source/enduse.h
pkginclude_HEADERS += source/exception.h
pkginclude_HEADERS += source/loadshape.h
pkginclude_HEADERS += source/lock.h
pkginclude_HEADERS += source/module.h
pkginclude_HEADERS += source/object.h
pkginclude_HEADERS += source/property.h
pkginclude_HEADERS += source/schedule.h
pkginclude_HEADERS += source/test.h
pkginclude_HEADERS += source/version.h

gridlabddir = $(prefix)/share/gridlabd
gridlabd_DATA = origin.txt

# source/gridlabd.in: source/gridlabd.m4sh
# 	@autom4te -l m4sh $< > $@
# 	@echo "$@ updated, please run 'make reconfigure && make TARGET' again" && false

$(top_srcdir)/source/build.h: buildnum

buildnum: utilities/build_number
	@/bin/bash -c "$(top_srcdir)/utilities/build_number"
	@/bin/bash -c "$(top_srcdir)/utilities/update_origin" > origin.txt

weather:
	@(echo "Installing weather data manager" && mkdir -p $(prefix)/share/gridlabd/weather && chmod 2777 $(prefix)/share/gridlabd/weather && chmod 1755 $(bindir)/gridlabd-weather)
	@(echo "Updating weather data index" && export GLD_ETC=$(prefix)/share/gridlabd && $(bindir)/gridlabd-weather fetch_index)

template: # TODO

library: # TODO	
