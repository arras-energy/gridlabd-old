pkglib_LTLIBRARIES += module/reliability/reliability.la

module_reliability_reliability_la_CPPFLAGS =
module_reliability_reliability_la_CPPFLAGS += $(AM_CPPFLAGS)

module_reliability_reliability_la_LDFLAGS =
module_reliability_reliability_la_LDFLAGS += $(AM_LDFLAGS)

module_reliability_reliability_la_LIBADD =

module_reliability_reliability_la_SOURCES =
module_reliability_reliability_la_SOURCES += module/reliability/reliability.h
module_reliability_reliability_la_SOURCES += module/reliability/init.cpp module/reliability/main.cpp
module_reliability_reliability_la_SOURCES += module/reliability/cost.cpp module/reliability/cost.h
module_reliability_reliability_la_SOURCES += module/reliability/eventgen.cpp module/reliability/eventgen.h
module_reliability_reliability_la_SOURCES += module/reliability/metrics.cpp module/reliability/metrics.h
module_reliability_reliability_la_SOURCES += third_party/jsonCpp/jsoncpp.cpp
