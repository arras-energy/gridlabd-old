pkglib_LTLIBRARIES += tariff/tariff.la

tariff_tariff_la_CPPFLAGS =
tariff_tariff_la_CPPFLAGS += $(AM_CPPFLAGS)

tariff_tariff_la_LDFLAGS =
tariff_tariff_la_LDFLAGS += $(AM_LDFLAGS)

tariff_tariff_la_LIBADD = 

tariff_tariff_la_SOURCES =
tariff_tariff_la_SOURCES += tariff/main.cpp 
tariff_tariff_la_SOURCES += tariff/tariff.cpp tariff/tariff.h
