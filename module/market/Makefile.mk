pkglib_LTLIBRARIES += module/market/market.la

module_market_market_la_CPPFLAGS =
module_market_market_la_CPPFLAGS += $(AM_CPPFLAGS)

module_market_market_la_LDFLAGS =
module_market_market_la_LDFLAGS += $(AM_LDFLAGS)

module_market_market_la_LIBADD =

module_market_market_la_SOURCES =
module_market_market_la_SOURCES += module/market/auction.cpp
module_market_market_la_SOURCES += module/market/auction.h
module_market_market_la_SOURCES += module/market/bid.cpp
module_market_market_la_SOURCES += module/market/bid.h
module_market_market_la_SOURCES += module/market/collect.cpp
module_market_market_la_SOURCES += module/market/collect.h
module_market_market_la_SOURCES += module/market/controller.cpp
module_market_market_la_SOURCES += module/market/controller.h
module_market_market_la_SOURCES += module/market/curve.cpp
module_market_market_la_SOURCES += module/market/curve.h
module_market_market_la_SOURCES += module/market/double_controller.cpp
module_market_market_la_SOURCES += module/market/double_controller.h
module_market_market_la_SOURCES += module/market/generator_controller.cpp
module_market_market_la_SOURCES += module/market/generator_controller.h
module_market_market_la_SOURCES += module/market/init.cpp
module_market_market_la_SOURCES += module/market/main.cpp
module_market_market_la_SOURCES += module/market/market.h
module_market_market_la_SOURCES += module/market/passive_controller.cpp
module_market_market_la_SOURCES += module/market/passive_controller.h
module_market_market_la_SOURCES += module/market/stub_bidder.cpp
module_market_market_la_SOURCES += module/market/stub_bidder.h
module_market_market_la_SOURCES += module/market/stubauction.cpp
module_market_market_la_SOURCES += module/market/stubauction.h
module_market_market_la_SOURCES += module/market/supervisory_control.cpp
module_market_market_la_SOURCES += module/market/supervisory_control.h
