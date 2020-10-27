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
public:
	typedef enum {CANCEL, BUYMARKET, SELLMARKET, BUYLIMIT, SELLLIMIT} ORDERTYPE;
private:
	ORDERTYPE type;
	unsigned long long id;
	double quantity;
	double price;
	unsigned long long end;
	double value;
public:
	inline order(void) { type=CANCEL; id=0; quantity=price=value=0.0; end=0; };
	inline order(const char *str) { type=CANCEL; id=0; quantity=price=value=0.0; end=0; }; // TODO: parse string
	inline ~order(void) {};
	double const match(order&);
public:
	inline ORDERTYPE get_type(void) const { return type; };
	inline double get_price(void) const { return price; };
	inline double get_quantity(void) const { return quantity; };
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
	int fill(order&);
	int add_sell(order&);
	int add_buy(order&);
	int update_market(void);
	int update_market(order&);
public:
	static CLASS *oclass;
	static orderbook *defaults;
};

#endif // _ORDERBOOK_H
