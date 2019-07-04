// transactive/orderbook.h
// Copyright (C) 2019, Stanford University
// Author: dchassin@slac.stanford.edu

#ifndef _ORDERBOOK_H
#define _ORDERBOOK_H

#include "gridlabd.h"

DECL_METHOD(orderbook,sell_market);
DECL_METHOD(orderbook,buy_market);
DECL_METHOD(orderbook,sell_limit);
DECL_METHOD(orderbook,buy_limit);

class orderbook : public gld_object {
public:
	GL_METHOD(orderbook,sell_market);
	GL_METHOD(orderbook,buy_market);
	GL_METHOD(orderbook,sell_limit);
	GL_METHOD(orderbook,buy_limit);
public:
	/* required implementations */
	orderbook(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
public:
	static CLASS *oclass;
	static orderbook *defaults;
};

#endif // _ORDERBOOK_H
