# module.mk -- list of GridLAB-D modules that are considered part of this build
include module/assert/Makefile.mk
include module/climate/Makefile.mk
include module/commercial/Makefile.mk
include module/generators/Makefile.mk
include module/industrial/Makefile.mk
include module/influxdb/Makefile.mk
include module/market/Makefile.mk
include module/optimize/Makefile.mk 
include module/powerflow/Makefile.mk
include module/reliability/Makefile.mk
include module/residential/Makefile.mk
include module/resilience/Makefile.mk
include module/revenue/Makefile.mk

# TODO: deprecating these soon
include module/tape/Makefile.mk
include module/tape_file/Makefile.mk
include module/tape_plot/Makefile.mk

# TODO: removed until HiPAS upgrades are implemented
#include connection/Makefile.mk 
