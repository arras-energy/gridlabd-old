# module.mk -- list of GridLAB-D modules that are considered part of this build
include assert/Makefile.mk
include climate/Makefile.mk
include commercial/Makefile.mk
include generators/Makefile.mk
include market/Makefile.mk
include mysql/Makefile.mk
include powerflow/Makefile.mk
include reliability/Makefile.mk
include residential/Makefile.mk
include tape_file/Makefile.mk
include tape/Makefile.mk
include tape_plot/Makefile.mk

# TODO: removed until 4.2 upgrades are implemented
#include connection/Makefile.mk 
#include optimize/Makefile.mk 
