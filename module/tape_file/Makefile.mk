pkglib_LTLIBRARIES += module/tape_file/tape_file.la

module_tape_file_tape_file_la_CPPFLAGS =
module_tape_file_tape_file_la_CPPFLAGS += $(AM_CPPFLAGS)
module_tape_file_tape_file_la_CPPFLAGS += -I$(top_srcdir)/module/tape

module_tape_file_tape_file_la_LDFLAGS =
module_tape_file_tape_file_la_LDFLAGS += $(AM_LDFLAGS)

module_tape_file_tape_file_la_LIBADD =

module_tape_file_tape_file_la_SOURCES = module/tape_file/tape_file.cpp module/tape_file/tape_file.h  
