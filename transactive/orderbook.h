// transactive/orderbook.h
// Copyright (C) 2019, Stanford University
// Author: dchassin@slac.stanford.edu

#ifndef _ORDERBOOK_H
#define _ORDERBOOK_H

#include "gridlabd.h"

#include <list>

DECL_METHOD(orderbook,submit);

class order 
{
private:
	enum {CANCEL, BUYMARKET, SELLMARKET, BUYLIMIT, SELLLIMIT} type;
	unsigned long long id;
	double quantity;
	double price;
	unsigned long long end;
	double value;
public:
	order(const char *);
	~order(void);
	double match(order&);
};

class orderbook : public gld_object 
{
private:
	std::list<order> *sell;
	std::list<order> *buy;
public:
	GL_ATOMIC(double,sell_price);
	GL_ATOMIC(double,buy_price);
	GL_STRING(char32,unit);
	GL_ATOMIC(TIMESTAMP,hold);
	GL_METHOD(orderbook,submit);
public:
	orderbook(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP, TIMESTAMP);
private:
	ORDER *json_to_order(const char *buffer);
	int fill(order *);
	int add_sell(ORDER *order);
	int add_buy(ORDER *order);
	int update_market(ORDER *order=NULL);
public:
	static CLASS *oclass;
	static orderbook *defaults;
};

#endif // _ORDERBOOK_H
