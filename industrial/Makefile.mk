pkglib_LTLIBRARIES += industrial/industrial.la

industrial_industrial_la_CPPFLAGS =
industrial_industrial_la_CPPFLAGS += $(AM_CPPFLAGS)

industrial_industrial_la_LDFLAGS =
industrial_industrial_la_LDFLAGS += $(AM_LDFLAGS)

industrial_industrial_la_LIBADD = 

industrial_industrial_la_SOURCES = industrial/init.cpp
industrial_industrial_la_SOURCES += industrial/industrial.h industrial/industrial.cpp

dist_pkgdata_DATA += industrial/naics_data_file.csv