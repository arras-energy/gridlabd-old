pkglib_LTLIBRARIES += reliability/reliability.la

reliability_reliability_la_CPPFLAGS =
reliability_reliability_la_CPPFLAGS += $(AM_CPPFLAGS)

reliability_reliability_la_LDFLAGS =
reliability_reliability_la_LDFLAGS += $(AM_LDFLAGS)

reliability_reliability_la_LIBADD =

reliability_reliability_la_SOURCES =
reliability_reliability_la_SOURCES += reliability/reliability.h
reliability_reliability_la_SOURCES += reliability/init.cpp reliability/main.cpp
reliability_reliability_la_SOURCES += reliability/cost.cpp reliability/cost.h
reliability_reliability_la_SOURCES += reliability/eventgen.cpp reliability/eventgen.h
reliability_reliability_la_SOURCES += reliability/metrics.cpp reliability/metrics.h
reliability_reliability_la_SOURCES += third_party/jsonCpp/jsoncpp.cpp
