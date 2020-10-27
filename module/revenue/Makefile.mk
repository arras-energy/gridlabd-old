pkglib_LTLIBRARIES += module/revenue/revenue.la

module_revenue_revenue_la_CPPFLAGS =
module_revenue_revenue_la_CPPFLAGS += $(AM_CPPFLAGS)

module_revenue_revenue_la_LDFLAGS =
module_revenue_revenue_la_LDFLAGS += $(AM_LDFLAGS)

module_revenue_revenue_la_LIBADD = 

module_revenue_revenue_la_SOURCES =
module_revenue_revenue_la_SOURCES += module/revenue/main.cpp 
module_revenue_revenue_la_SOURCES += module/revenue/revenue.h
module_revenue_revenue_la_SOURCES += module/revenue/billing.cpp module/revenue/billing.h
module_revenue_revenue_la_SOURCES += module/revenue/tariff.cpp module/revenue/tariff.h
