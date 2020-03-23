pkglib_LTLIBRARIES += revenue/revenue.la

revenue_revenue_la_CPPFLAGS =
revenue_revenue_la_CPPFLAGS += $(AM_CPPFLAGS)

revenue_revenue_la_LDFLAGS =
revenue_revenue_la_LDFLAGS += $(AM_LDFLAGS)

revenue_revenue_la_LIBADD = 

revenue_revenue_la_SOURCES =
revenue_revenue_la_SOURCES += revenue/main.cpp revenue/revenue.h
revenue_revenue_la_SOURCES += revenue/billing.cpp revenue/billing.h
