pkglib_LTLIBRARIES += source/link/engine/glxengine.la

source_link_engine_glxengine_la_CPPFLAGS =
source_link_engine_glxengine_la_CPPFLAGS += $(AM_CPPFLAGS)

source_link_engine_glxengine_la_LDFLAGS =
source_link_engine_glxengine_la_LDFLAGS += -module -no-undefined -avoid-version -version-info 1:0:0
source_link_engine_glxengine_la_LDFLAGS += $(AM_LDFLAGS)

source_link_engine_glxengine_la_LIBADD =

source_link_engine_glxengine_la_SOURCES = 
source_link_engine_glxengine_la_SOURCES += source/link/engine/engine.cpp
source_link_engine_glxengine_la_SOURCES += source/link/engine/engine.h
source_link_engine_glxengine_la_SOURCES += source/link/engine/udpconnection.cpp

uninstall-hook:
	-rmdir $(DESTDIR)$(pkglibdir)
