pkglib_LTLIBRARIES += module/industrial/industrial.la

module_industrial_industrial_la_CPPFLAGS =
module_industrial_industrial_la_CPPFLAGS += $(AM_CPPFLAGS)

module_industrial_industrial_la_LDFLAGS =
module_industrial_industrial_la_LDFLAGS += $(AM_LDFLAGS)

module_industrial_industrial_la_LIBADD = 

module_industrial_industrial_la_SOURCES = module/industrial/init.cpp
module_industrial_industrial_la_SOURCES += module/industrial/industrial.h module/industrial/industrial.cpp

dist_pkgdata_DATA += module/industrial/naics_data_file.csv
