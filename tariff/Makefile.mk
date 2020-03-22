pkglib_LTLIBRARIES += tariff/tariff.la

tariff_tariff_la_CPPFLAGS =
tariff_tariff_la_CPPFLAGS += $(AM_CPPFLAGS)

tariff_tariff_la_LDFLAGS =
tariff_tariff_la_LDFLAGS += $(AM_LDFLAGS)

tariff_tariff_la_LIBADD = 

tariff_tariff_la_SOURCES =
tariff_tariff_la_SOURCES += tariff/main.cpp 
tariff_tariff_la_SOURCES += tariff/tariff.cpp tariff/tariff.h
# tariff_tariff_la_SOURCES += tariff/{{{class1}}}.cpp tariff/{{{class1}}}.h
# tariff_tariff_la_SOURCES += tariff/{{{class2}}}.cpp tariff/{{{class2}}}.h
