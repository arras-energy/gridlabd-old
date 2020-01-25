pkglib_LTLIBRARIES += influxdb/influxdb.la

influxdb_influxdb_la_CPPFLAGS =
influxdb_influxdb_la_CPPFLAGS += $(INFLUXDB_CPPFLAGS)
influxdb_influxdb_la_CPPFLAGS += $(AM_CPPFLAGS)

influxdb_influxdb_la_LDFLAGS = -lpthread -lcurl
influxdb_influxdb_la_LDFLAGS += $(INFLUXDB_LDFLAGS)
influxdb_influxdb_la_LDFLAGS += $(AM_LDFLAGS)

influxdb_influxdb_la_LIBADD =
influxdb_influxdb_la_LIBADD += $(INFLUXDB_LIBS)

influxdb_influxdb_la_SOURCES =
influxdb_influxdb_la_SOURCES += influxdb/collector.cpp influxdb/collector.h
influxdb_influxdb_la_SOURCES += influxdb/database.cpp influxdb/database.h
influxdb_influxdb_la_SOURCES += influxdb/influxdb.cpp influxdb/influxdb.h
influxdb_influxdb_la_SOURCES += influxdb/player.cpp influxdb/player.h
influxdb_influxdb_la_SOURCES += influxdb/recorder.cpp influxdb/recorder.h
