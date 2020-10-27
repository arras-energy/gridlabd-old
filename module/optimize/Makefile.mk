pkglib_LTLIBRARIES += module/optimize/optimize.la

module_optimize_optimize_la_CPPFLAGS =
module_optimize_optimize_la_CPPFLAGS += $(AM_CPPFLAGS)

module_optimize_optimize_la_LDFLAGS =
module_optimize_optimize_la_LDFLAGS += $(AM_LDFLAGS)

module_optimize_optimize_la_LIBADD =

module_optimize_optimize_la_SOURCES =
module_optimize_optimize_la_SOURCES += module/optimize/main.cpp module/optimize/optimize.h
module_optimize_optimize_la_SOURCES += module/optimize/simple.cpp module/optimize/simple.h
