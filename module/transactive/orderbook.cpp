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
EXPORT_METHOD(orderbook,submit);

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
				PT_double, "buy", get_buy_price_offset(), PT_DESCRIPTION, "current buy price",
				PT_double, "sell", get_sell_price_offset(), PT_DESCRIPTION, "current sell price",
				PT_char32, "unit", get_unit_offset(), PT_DESCRIPTION, "quantity unit (e.g., MW)",
				PT_timestamp, "hold", get_hold_offset(), PT_DESCRIPTION, "hold time",
				PT_method, "submit", get_submit_offset(), PT_DESCRIPTION, "submit order (format as JSON or ORDERBOOK)",
				NULL) < 1 )
			throw "unable to publish orderbook properties";
	}
}

int orderbook::create(void)
{
	sell = new std::list<order>;
	buy = new std::list<order>;
	return 1; // return 1 on success, 0 on failure
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

bool lesser(const order &a, const order &b)
{
	return a.get_price() < b.get_price();
}
bool greater(const order &a, const order &b)
{
	return a.get_price() > b.get_price();
}

int orderbook::fill(order &o)
{
	if ( o.get_type() == order::BUYMARKET )
	{
		buy_price = sell->front().get_price();
	}
	else if ( o.get_type() == order::SELLMARKET )
	{
		sell_price = buy->front().get_price();
	}
	// TODO: other order types
	return 1; // 1 on success, 0 on failure
}

int orderbook::submit(char *buffer, size_t len)
{
	verbose("orderbook::submit(char *buffer='%s', size_t len=%d)\n", buffer, len);
	order o;
	if ( len == 0 ) // read data from buffer
	{
		if ( buffer[0] == '{' ) // JSON format
		{
			o = order(buffer);
		}
		else if ( sizeof(order) == len ) // binary format
		{
			o = *(order*)buffer;
		}
		else
			throw "invalid order data";
		switch ( o.get_type() )
		{
		case order::BUYMARKET:
		case order::SELLMARKET:
			return update_market(o);
		case order::BUYLIMIT:
			buy->push_front(o);
			buy->sort(greater);
			break;
		case order::SELLLIMIT:
			sell->push_front(o);
			sell->sort(lesser);
			break;
		case order::CANCEL:
			// TODO: delete existing limit order (using id)
			break;
		default:
			warning("ignoring unrecognized order type %d", o.get_type());
			break;
		}
		return update_market();
	}
	else // write data to buffer
		return 0; // no outgoing message
}

int orderbook::update_market(void)
{
	return 0;
}

int orderbook::update_market(order&)
{
	return update_market();;
}
