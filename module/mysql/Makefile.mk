pkglib_LTLIBRARIES += module/mysql/mysql.la

module_mysql_mysql_la_CPPFLAGS =
module_mysql_mysql_la_CPPFLAGS += $(MYSQL_CPPFLAGS)
module_mysql_mysql_la_CPPFLAGS += $(AM_CPPFLAGS)

module_mysql_mysql_la_LDFLAGS = -lpthread
module_mysql_mysql_la_LDFLAGS += $(MYSQL_LDFLAGS)
module_mysql_mysql_la_LDFLAGS += $(AM_LDFLAGS)

module_mysql_mysql_la_LIBADD =
module_mysql_mysql_la_LIBADD += $(MYSQL_LIBS)

module_mysql_mysql_la_SOURCES =
module_mysql_mysql_la_SOURCES += module/mysql/collector.cpp
module_mysql_mysql_la_SOURCES += module/mysql/collector.h
module_mysql_mysql_la_SOURCES += module/mysql/database.cpp
module_mysql_mysql_la_SOURCES += module/mysql/database.h
module_mysql_mysql_la_SOURCES += module/mysql/init.cpp
module_mysql_mysql_la_SOURCES += module/mysql/main.cpp
module_mysql_mysql_la_SOURCES += module/mysql/player.cpp
module_mysql_mysql_la_SOURCES += module/mysql/player.h
module_mysql_mysql_la_SOURCES += module/mysql/recorder.cpp
module_mysql_mysql_la_SOURCES += module/mysql/recorder.h
