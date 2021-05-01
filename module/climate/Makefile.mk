pkglib_LTLIBRARIES += module/climate/climate.la

module_climate_climate_la_CPPFLAGS =
module_climate_climate_la_CPPFLAGS += $(AM_CPPFLAGS)

module_climate_climate_la_LDFLAGS =
module_climate_climate_la_LDFLAGS += $(AM_LDFLAGS)

module_climate_climate_la_LIBADD =

module_climate_climate_la_SOURCES =
module_climate_climate_la_SOURCES += module/climate/init.cpp module/climate/main.cpp
module_climate_climate_la_SOURCES += module/climate/climate.cpp module/climate/climate.h
module_climate_climate_la_SOURCES += module/climate/csv_reader.cpp module/climate/csv_reader.h
module_climate_climate_la_SOURCES += module/climate/solar_angles.cpp module/climate/solar_angles.h
module_climate_climate_la_SOURCES += module/climate/weather.cpp module/climate/weather.h
module_climate_climate_la_SOURCES += module/climate/weather_reader.cpp module/climate/weather_reader.h
