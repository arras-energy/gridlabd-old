pkglib_LTLIBRARIES += module/powerflow/powerflow.la

module_powerflow_powerflow_la_CPPFLAGS = -DSOLVER_PY -I$(shell python3 -c 'import numpy; print(numpy.get_include())')
module_powerflow_powerflow_la_CPPFLAGS += -I$(top_srcdir)/third_party/superLU_MT
module_powerflow_powerflow_la_CPPFLAGS += -I$(top_srcdir)/module/powerflow
module_powerflow_powerflow_la_CPPFLAGS += $(AM_CPPFLAGS)

module_powerflow_powerflow_la_LDFLAGS =
module_powerflow_powerflow_la_LDFLAGS += $(AM_LDFLAGS)
module_powerflow_powerflow_la_LDFLAGS += -ldl

module_powerflow_powerflow_la_LIBADD =
module_powerflow_powerflow_la_LIBADD += third_party/superLU_MT/libsuperlu.la
module_powerflow_powerflow_la_LIBADD += $(PTHREAD_CFLAGS)
module_powerflow_powerflow_la_LIBADD += $(PTHREAD_LIBS)
module_powerflow_powerflow_la_LIBADD += -ldl

module_powerflow_powerflow_la_SOURCES = module/powerflow/init.cpp module/powerflow/main.cpp

module_powerflow_powerflow_la_SOURCES += module/powerflow/billdump.cpp module/powerflow/billdump.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/building.cpp module/powerflow/building.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/capacitor.cpp module/powerflow/capacitor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/currdump.cpp module/powerflow/currdump.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/emissions.cpp module/powerflow/emissions.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/fault_check.cpp module/powerflow/fault_check.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/frequency_gen.cpp module/powerflow/frequency_gen.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/fuse.cpp module/powerflow/fuse.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/impedance_dump.cpp module/powerflow/impedance_dump.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/line_configuration.cpp
module_powerflow_powerflow_la_SOURCES += module/powerflow/line_configuration.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/line.cpp module/powerflow/line.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/line_sensor.cpp module/powerflow/line_sensor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/line_spacing.cpp module/powerflow/line_spacing.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/link.cpp module/powerflow/link.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/load.cpp module/powerflow/load.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/load_tracker.cpp module/powerflow/load_tracker.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/meter.cpp module/powerflow/meter.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/motor.cpp module/powerflow/motor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/node.cpp module/powerflow/node.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/overhead_line_conductor.cpp module/powerflow/overhead_line_conductor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/overhead_line.cpp module/powerflow/overhead_line.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/pole.h module/powerflow/pole.cpp
module_powerflow_powerflow_la_SOURCES += module/powerflow/pole_configuration.h module/powerflow/pole_configuration.cpp
module_powerflow_powerflow_la_SOURCES += module/powerflow/pole_mount.h module/powerflow/pole_mount.cpp
module_powerflow_powerflow_la_SOURCES += module/powerflow/powerflow.cpp module/powerflow/powerflow.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/powerflow_library.cpp module/powerflow/powerflow_library.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/powerflow_object.cpp module/powerflow/powerflow_object.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/power_metrics.cpp module/powerflow/power_metrics.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/pqload.cpp module/powerflow/pqload.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/recloser.cpp module/powerflow/recloser.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/regulator_configuration.cpp module/powerflow/regulator_configuration.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/regulator.cpp module/powerflow/regulator.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/restoration.cpp module/powerflow/restoration.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/sectionalizer.cpp module/powerflow/sectionalizer.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/series_reactor.cpp module/powerflow/series_reactor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/solver_nr.cpp module/powerflow/solver_nr.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/solver_py.cpp module/powerflow/solver_py.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/substation.cpp module/powerflow/substation.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/switch_coordinator.cpp module/powerflow/switch_coordinate.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/switch_object.cpp module/powerflow/switch_object.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/transformer_configuration.cpp module/powerflow/transformer_configuration.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/transformer.cpp module/powerflow/transformer.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplex_line_conductor.cpp module/powerflow/triplex_line_conductor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplex_line_configuration.cpp module/powerflow/triplex_line_configuration.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplex_line.cpp module/powerflow/triplex_line.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplex_load.h module/powerflow/triplex_load.cpp
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplex_meter.cpp module/powerflow/triplex_meter.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplex_node.cpp module/powerflow/triplex_node.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/underground_line_conductor.cpp module/powerflow/underground_line_conductor.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/underground_line.cpp module/powerflow/underground_line.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/vfd.cpp module/powerflow/vfd.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/voltdump.cpp module/powerflow/voltdump.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/volt_var_control.cpp module/powerflow/volt_var_control.h

module_powerflow_powerflow_la_SOURCES += module/powerflow/meter_test.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/overheadline_test.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/regulator_test.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/transformer_test.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/triplexline_test.h
module_powerflow_powerflow_la_SOURCES += module/powerflow/undergroundline_test.h

dist_pkgdata_DATA += module/powerflow/solver_py.conf
dist_pkgdata_DATA += module/powerflow/building_defaults.csv

bin_SCRIPTS += module/powerflow/check_powerflow.py
