pkglib_LTLIBRARIES += module/influxdb/influxdb.la

module_influxdb_influxdb_la_CPPFLAGS =
module_influxdb_influxdb_la_CPPFLAGS += $(INFLUXDB_CPPFLAGS)
module_influxdb_influxdb_la_CPPFLAGS += $(AM_CPPFLAGS)

module_influxdb_influxdb_la_LDFLAGS = -lpthread -lcurl
module_influxdb_influxdb_la_LDFLAGS += $(INFLUXDB_LDFLAGS)
module_influxdb_influxdb_la_LDFLAGS += $(AM_LDFLAGS)

module_influxdb_influxdb_la_LIBADD =
module_influxdb_influxdb_la_LIBADD += $(INFLUXDB_LIBS)

module_influxdb_influxdb_la_SOURCES =
module_influxdb_influxdb_la_SOURCES += module/influxdb/collector.cpp module/influxdb/collector.h
module_influxdb_influxdb_la_SOURCES += module/influxdb/database.cpp module/influxdb/database.h
module_influxdb_influxdb_la_SOURCES += module/influxdb/influxdb.cpp module/influxdb/influxdb.h
module_influxdb_influxdb_la_SOURCES += module/influxdb/jsondata.h
module_influxdb_influxdb_la_SOURCES += module/influxdb/player.cpp module/influxdb/player.h
module_influxdb_influxdb_la_SOURCES += module/influxdb/recorder.cpp module/influxdb/recorder.h
