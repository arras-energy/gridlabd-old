// gldcore/string.h
// Copyright (C) 2021, Regents of the Leland Stanford Junior University

#ifndef _VARCHAR_H
#define _VARCHAR_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Class: varchar
//
// The varchar class is a simple replacement for most char[SIZE] declarations,
// e.g., replace "char a[N];" with "varchar a(N);".  In general, most string 
// functions will work the same.  Some functions needs to use the ".get_string()"
// and ".get_size()" members to access the buffer pointer and buffer size.
//
// Important note: this class is not thread safe. It is primarly intented as an
// easy replacement for strings allocated on the stack.
//
class varchar
{
private:
	// Member: str
	// The buffer containing the character string
	char *str;

	// Member: sz
	// The maximum size of the character string allowed in the buffer
	size_t sz;
public:

	// Constructor: varchar
	varchar();
	varchar(const char *src, size_t len=0);
	varchar(size_t len);
	varchar(varchar &s);

	// Destructor: ~varchar
	~varchar();
private:

	// Method: internal reallocation of buffer
	void _realloc(char *str,size_t len=-1);
public:

	// Method: 
	inline operator const char *() { return str; };
	inline char *get_string() { return str; };
	inline const size_t get_size() { return sz; };
public:
	inline bool operator == (const char *s) { return strcmp(s,str)==0; };
	inline bool operator >= (const char *s) { return strcmp(s,str)>=0; };
	inline bool operator > (const char *s) { return strcmp(s,str)>0; };
	inline bool operator != (const char *s) { return strcmp(s,str)!=0; };
	inline bool operator < (const char *s) { return strcmp(s,str)<0; };
	inline bool operator <= (const char *s) { return strcmp(s,str)<=0; };
public:
	char *resize(size_t len, bool keep=false);
	void clear(size_t len, int c=0);
	void copy(const char *s, size_t len=-1);
	void copy(varchar &s);
public:
	inline char &operator [] (size_t n) { return str[n]; };
	varchar &operator = (const char *s) { copy(s); return *this; };
	varchar &operator = (varchar &s) { copy(s); return *this; };
public:
	size_t length();
	int format(const char *fmt, ...);
	int format(const char *fmt, va_list ptr);
	int format(size_t size, const char *fmt, va_list ptr);
	void append(const char *str);
};

size_t strlen(varchar &dst);
void *memset(varchar &dst, int c, size_t len);

int sprintf(varchar &dst, const char *fmt, ...);
int snprintf(varchar &dst, size_t sz, const char *fmt, ...);
int vsprintf(varchar &dst, const char *fmt, va_list ptr);
int vsnprintf(varchar &dst, size_t sz, const char *fmt, va_list ptr);

varchar &strcpy(varchar &dst, const char *src);
varchar &strcpy(varchar &dst, const char *src, size_t sz);
varchar &strcat(varchar &dst, const char *src);

#endif
