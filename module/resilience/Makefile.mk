pkglib_LTLIBRARIES += module/resilience/resilience.la

module_resilience_resilience_la_CPPFLAGS =
module_resilience_resilience_la_CPPFLAGS += $(AM_CPPFLAGS)

module_resilience_resilience_la_LDFLAGS =
module_resilience_resilience_la_LDFLAGS += $(AM_LDFLAGS)

module_resilience_resilience_la_LIBADD =

module_resilience_resilience_la_SOURCES =
module_resilience_resilience_la_SOURCES += module/resilience/resilience.h module/resilience/main.cpp
module_resilience_resilience_la_SOURCES += module/resilience/cost.cpp module/resilience/cost.h
module_resilience_resilience_la_SOURCES += module/resilience/impact.cpp module/resilience/impact.h
module_resilience_resilience_la_SOURCES += module/resilience/metrics.cpp module/resilience/metrics.h
module_resilience_resilience_la_SOURCES += module/resilience/outage.cpp module/resilience/outage.h
