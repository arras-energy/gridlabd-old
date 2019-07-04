// transactive/orderbook.cpp
// Copyright (C) 2019, Stanford University
// Author: dchassin@slac.stanford.edu

#include "transactive.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <complex.h>

EXPORT_CREATE(orderbook);
EXPORT_INIT(orderbook);
EXPORT_COMMIT(orderbook);
EXPORT_METHOD(orderbook,sell_market);
EXPORT_METHOD(orderbook,buy_market);
EXPORT_METHOD(orderbook,sell_limit);
EXPORT_METHOD(orderbook,buy_limit);

CLASS *orderbook::oclass = NULL;
orderbook *orderbook::defaults = NULL;

orderbook::orderbook(MODULE *module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"orderbook",sizeof(orderbook),PC_AUTOLOCK|PC_OBSERVER);
		if (oclass==NULL)
			throw "unable to register class orderbook";
		else
			oclass->trl = TRL_CONCEPT;
		defaults = this;
		if ( gl_publish_variable(oclass,
				PT_method, "sell_market", get_sell_market_offset(), PT_DESCRIPTION, "sell market bid (quantity, start, duration)",
				PT_method, "buy_market", get_buy_market_offset(), PT_DESCRIPTION, "buy market bid (quantity, start, duration)",
				PT_method, "sell_limit", get_sell_limit_offset(), PT_DESCRIPTION, "sell limit bid (quantity, price, start, duration)",
				PT_method, "buy_limit", get_buy_limit_offset(), PT_DESCRIPTION, "buy limit bid (quantity, price, start, duration)",
				NULL) < 1 )
			throw "unable to publish orderbook properties";
	}
}

int orderbook::create(void)
{
	return 1; /* return 1 on success, 0 on failure */
}

int orderbook::init(OBJECT *parent)
{
	gld_property connect(parent,"connect");
	if ( !connect.is_valid() )
		warning("parent does not implement 'connect' method");
	connect.call(get_name());

	return 1;
}

TIMESTAMP orderbook::commit(TIMESTAMP t1, TIMESTAMP t2)
{
	return TS_NEVER;
}

int orderbook::sell_market(char *buffer, size_t len)
{
	verbose("orderbook::sell_market(char *buffer='%s', size_t len=%d)\n", buffer, len);
	if ( len == 0 )
	{
		return 1;
	}
	else
		return 0; // no outgoing message
}

int orderbook::buy_market(char *buffer, size_t len)
{
	verbose("orderbook::buy_market(char *buffer='%s', size_t len=%d)\n", buffer, len);
	if ( len == 0 )
	{
		return 1;
	}
	else
		return 0; // no outgoing message
}

int orderbook::sell_limit(char *buffer, size_t len)
{
	verbose("orderbook::sell_limit(char *buffer='%s', size_t len=%d)\n", buffer, len);
	if ( len == 0 )
	{
		return 1;
	}
	else
		return 0; // no outgoing message
}

int orderbook::buy_limit(char *buffer, size_t len)
{
	verbose("orderbook::buy_limit(char *buffer='%s', size_t len=%d)\n", buffer, len);
	if ( len == 0 )
	{
		return 1;
	}
	else
		return 0; // no outgoing message
}

