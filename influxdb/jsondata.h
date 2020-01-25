// File: jsondata.h
// Copyright (C) 2020 Regents of the Leland Stanford Junior University

#ifndef _JSONDATA_H
#define _JSONDATA_H

#include <iostream>

class jsondata
{
	typedef enum {EMPTY, DICT, LIST, STRING, BOOLEAN, INTEGER} JSONTYPE;
	JSONTYPE type;
public:

	inline jsondata(const char *str=NULL) 
	{
		type = EMPTY;
		std::cout << "jsondata(str='" << str << "')" << std::endl;
	};
	
	inline ~jsondata(void) 
	{

	};

	inline const char *operator [] (int n) 
	{
		return NULL; 
	};
	
	inline const char *operator [] (const char *n) 
	{
		return NULL; 
	};
};

#endif
