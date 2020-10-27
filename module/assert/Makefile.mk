pkglib_LTLIBRARIES += module/assert/assert.la

module_assert_assert_la_CPPFLAGS =
module_assert_assert_la_CPPFLAGS += $(AM_CPPFLAGS)

module_assert_assert_la_LDFLAGS =
module_assert_assert_la_LDFLAGS += $(AM_LDFLAGS)

module_assert_assert_la_LIBADD = 

module_assert_assert_la_SOURCES =
module_assert_assert_la_SOURCES += module/assert/assert.cpp module/assert/assert.h
module_assert_assert_la_SOURCES += module/assert/complex_assert.cpp module/assert/complex_assert.h
module_assert_assert_la_SOURCES += module/assert/double_assert.cpp module/assert/double_assert.h
module_assert_assert_la_SOURCES += module/assert/enum_assert.cpp module/assert/enum_assert.h
module_assert_assert_la_SOURCES += module/assert/int_assert.cpp module/assert/int_assert.h
module_assert_assert_la_SOURCES += module/assert/init.cpp module/assert/main.cpp
