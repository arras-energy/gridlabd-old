pkglib_LTLIBRARIES += module/tape/tape.la

module_tape_tape_la_CPPFLAGS =
module_tape_tape_la_CPPFLAGS += -I$(top_srcdir)/third_party/jsonCpp
module_tape_tape_la_CPPFLAGS += -I$(top_srcdir)/module/powerflow
module_tape_tape_la_CPPFLAGS += $(AM_CPPFLAGS) 

module_tape_tape_la_LDFLAGS =
module_tape_tape_la_LDFLAGS += $(AM_LDFLAGS)

module_tape_tape_la_LIBADD =
module_tape_tape_la_LIBADD += third_party/jsonCpp/libjsoncpp.la
module_tape_tape_la_LIBADD += -ldl

module_tape_tape_la_SOURCES = module/tape/main.cpp

module_tape_tape_la_SOURCES += module/tape/file.cpp module/tape/file.h
module_tape_tape_la_SOURCES += module/tape/group_recorder.h module/tape/group_recorder.cpp
module_tape_tape_la_SOURCES += module/tape/metrics_collector.cpp module/tape/metrics_collector.h
module_tape_tape_la_SOURCES += module/tape/metrics_collector_writer.cpp module/tape/metrics_collector_writer.h
module_tape_tape_la_SOURCES += module/tape/violation_recorder.h module/tape/violation_recorder.cpp
module_tape_tape_la_SOURCES += module/tape/histogram.cpp module/tape/histogram.h
module_tape_tape_la_SOURCES += module/tape/loadshape.cpp module/tape/loadshape.h
module_tape_tape_la_SOURCES += module/tape/tape.cpp module/tape/tape.h
module_tape_tape_la_SOURCES += module/tape/memory.cpp module/tape/memory.h
module_tape_tape_la_SOURCES += module/tape/odbc.cpp module/tape/odbc.h

module_tape_tape_la_SOURCES += module/tape/multi_recorder.cpp
module_tape_tape_la_SOURCES += module/tape/collector.cpp
module_tape_tape_la_SOURCES += module/tape/player.cpp
module_tape_tape_la_SOURCES += module/tape/recorder.cpp
module_tape_tape_la_SOURCES += module/tape/shaper.cpp
