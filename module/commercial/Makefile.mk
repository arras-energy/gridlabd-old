pkglib_LTLIBRARIES += module/commercial/commercial.la

module_commercial_commercial_la_CPPFLAGS =
module_commercial_commercial_la_CPPFLAGS += $(AM_CPPFLAGS)

module_commercial_commercial_la_LDFLAGS =
module_commercial_commercial_la_LDFLAGS += $(AM_LDFLAGS)

module_commercial_commercial_la_LIBADD =

module_commercial_commercial_la_SOURCES = module/commercial/commercial.h
module_commercial_commercial_la_SOURCES += module/commercial/init.cpp module/commercial/main.cpp
module_commercial_commercial_la_SOURCES += module/commercial/ceus.h module/commercial/ceus.cpp
module_commercial_commercial_la_SOURCES += module/commercial/hvac.cpp module/commercial/hvac.h
module_commercial_commercial_la_SOURCES += module/commercial/multizone.cpp module/commercial/multizone.h
module_commercial_commercial_la_SOURCES += module/commercial/office.cpp module/commercial/office.h
module_commercial_commercial_la_SOURCES += module/commercial/solvers.cpp module/commercial/solvers.h
module_commercial_commercial_la_SOURCES += module/commercial/parking.cpp module/commercial/parking.h
