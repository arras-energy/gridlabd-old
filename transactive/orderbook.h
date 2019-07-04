// transactive/orderbook.h
// Copyright (C) 2019, Stanford University
// Author: dchassin@slac.stanford.edu

#ifndef _ORDERBOOK_H
#define _ORDERBOOK_H

#include "gridlabd.h"

#include <list>

DECL_METHOD(orderbook,submit);

typedef enum 
{
	CANCEL = 0,
	BUYMARKET = 1,
	SELLMARKET = 2,
	BUYLIMIT = 3,
	SELLLIMIT = 4,
} ORDERTYPE;

typedef struct s_order 
{
	unsigned short type;
	unsigned long long id;
	double quantity;
	double price;
	unsigned long long start;
	unsigned long long duration;
} ORDER;

class orderbook : public gld_object {
private:
	std::list<ORDER> *sell;
	std::list<ORDER> *buy;
public:
	GL_METHOD(orderbook,submit);
public:
	/* required implementations */
	orderbook(MODULE *module);
	int create(void);
	int init(OBJECT *parent);
	TIMESTAMP commit(TIMESTAMP t1, TIMESTAMP t2);
private:
	ORDER *json_to_order(const char *buffer);
	int update_market(ORDER *order=NULL);
public:
	static CLASS *oclass;
	static orderbook *defaults;
};

#endif // _ORDERBOOK_H
