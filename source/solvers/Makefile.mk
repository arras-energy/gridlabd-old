pkglib_LTLIBRARIES += source/solvers/glsolvers.la

source_solvers_glsolvers_la_CPPFLAGS =
source_solvers_glsolvers_la_CPPFLAGS += $(AM_CPPFLAGS)

source_solvers_glsolvers_la_LDFLAGS =
source_solvers_glsolvers_la_LDFLAGS += $(AM_LDFLAGS)

source_solvers_glsolvers_la_LIBADD =

source_solvers_glsolvers_la_SOURCES =
source_solvers_glsolvers_la_SOURCES += source/solvers/etp.cpp
source_solvers_glsolvers_la_SOURCES += source/solvers/main.cpp
source_solvers_glsolvers_la_SOURCES += source/solvers/modified_euler.cpp
