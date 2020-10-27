pkglib_LTLIBRARIES += module/generators/generators.la

module_generators_generators_la_CPPFLAGS =
module_generators_generators_la_CPPFLAGS += $(AM_CPPFLAGS)
module_generators_generators_la_CPPFLAGS += -I$(top_srcdir)/module/powerflow

module_generators_generators_la_LDFLAGS =
module_generators_generators_la_LDFLAGS += $(AM_LDFLAGS)

module_generators_generators_la_LIBADD =

module_generators_generators_la_SOURCES =
module_generators_generators_la_SOURCES += module/generators/battery.cpp
module_generators_generators_la_SOURCES += module/generators/battery.h
module_generators_generators_la_SOURCES += module/generators/central_dg_control.cpp
module_generators_generators_la_SOURCES += module/generators/central_dg_control.h
module_generators_generators_la_SOURCES += module/generators/dc_dc_converter.cpp
module_generators_generators_la_SOURCES += module/generators/dc_dc_converter.h
module_generators_generators_la_SOURCES += module/generators/diesel_dg.cpp
module_generators_generators_la_SOURCES += module/generators/diesel_dg.h
module_generators_generators_la_SOURCES += module/generators/energy_storage.cpp
module_generators_generators_la_SOURCES += module/generators/energy_storage.h
module_generators_generators_la_SOURCES += module/generators/generators.h
module_generators_generators_la_SOURCES += module/generators/init.cpp
module_generators_generators_la_SOURCES += module/generators/inverter.cpp
module_generators_generators_la_SOURCES += module/generators/inverter.h
module_generators_generators_la_SOURCES += module/generators/main.cpp
module_generators_generators_la_SOURCES += module/generators/microturbine.cpp
module_generators_generators_la_SOURCES += module/generators/microturbine.h
module_generators_generators_la_SOURCES += module/generators/power_electronics.cpp
module_generators_generators_la_SOURCES += module/generators/power_electronics.h
module_generators_generators_la_SOURCES += module/generators/rectifier.cpp
module_generators_generators_la_SOURCES += module/generators/rectifier.h
module_generators_generators_la_SOURCES += module/generators/solar.cpp
module_generators_generators_la_SOURCES += module/generators/solar.h
module_generators_generators_la_SOURCES += module/generators/windturb_dg.cpp
module_generators_generators_la_SOURCES += module/generators/windturb_dg.h
module_generators_generators_la_SOURCES += module/generators/controller_dg.cpp
module_generators_generators_la_SOURCES += module/generators/controller_dg.h
