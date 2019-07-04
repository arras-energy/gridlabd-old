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
				PT_method, "submit", get_submit_offset(), PT_DESCRIPTION, "submit order (format as JSON or ORDERBOOK)",
				NULL) < 1 )
			throw "unable to publish orderbook properties";
	}
}

int orderbook::create(void)
{
	sell = new std::list<ORDER>;
	buy = new std::list<ORDER>;
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

ORDER *orderbook::json_to_order(const char *buffer)
{
	// TODO: extract JSON content into ORDER
	return NULL;
}

int orderbook::update_market(ORDER *order)
{
	// TODO: implement market update with market order (or limit orders only if order==NULL)
	return 1; // 1 on success, 0 on failure
}

int orderbook::submit(char *buffer, size_t len)
{
	verbose("orderbook::sell_market(char *buffer='%s', size_t len=%d)\n", buffer, len);
	ORDER *order;
	if ( len == 0 )
	{
		if ( buffer[0] == '{' ) // JSON format
		{
			order = json_to_order(buffer);
			if ( order == NULL )
			{
				return 0; // reject order
			}
		}
		else
		{
			order = new ORDER;
			memcpy(order,(ORDER*)buffer,sizeof(ORDER));
		}
		switch ( order->type )
		{
		case BUYMARKET:
		case SELLMARKET:
			return update_market(order);
		case BUYLIMIT:
			buy->push_front(*order);
			break;
		case SELLLIMIT:
			sell->push_front(*order);
			break;
		case CANCEL:
			// TODO: delete existing limit order (using id)
			break;
		default:
			warning("ignoring unrecognized order type %d", order->type);
			break;
		}
		return update_market();
	}
	else
		return 0; // no outgoing message
}
