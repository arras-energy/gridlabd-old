pkglib_LTLIBRARIES += module/tape_plot/tape_plot.la

module_tape_plot_tape_plot_la_CPPFLAGS =
module_tape_plot_tape_plot_la_CPPFLAGS += $(AM_CPPFLAGS)
module_tape_plot_tape_plot_la_CPPFLAGS += -I$(top_srcdir)/module/tape

module_tape_plot_tape_plot_la_LDFLAGS =
module_tape_plot_tape_plot_la_LDFLAGS += $(AM_LDFLAGS)

module_tape_plot_tape_plot_la_LIBADD =

module_tape_plot_tape_plot_la_SOURCES = module/tape_plot/tape_plot.cpp module/tape_plot/tape_plot.h  
