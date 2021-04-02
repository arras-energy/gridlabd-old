// gldcore/string.h
// Copyright (C) 2021, Regents of the Leland Stanford Junior University
//
// The varchar class is designed to serve as a simple replacement for
// most char[SIZE] declarations.
//

#ifndef _VARCHAR_H
#define _VARCHAR_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class varchar
{
private:
	char *str;
	size_t sz;
public:
	varchar();
	varchar(char *src, size_t len=0);
	varchar(size_t len);
	varchar(varchar &s);
	~varchar();
private:
	void _realloc(char *str,size_t len=-1);
public:
	inline const char *get_string() { return str; };
	inline const size_t get_size() { return sz; };
public:
	char *resize(size_t len, bool keep=false);
	void clear(size_t len, int c=0);
	void copy(const char *s, size_t len=-1);
	void copy(varchar &s);
public:
	inline operator const char *() { return str; };
	inline operator void *() { return str; };
	inline operator char *() { return str; };
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

int asprintf(varchar *dst, const char *fmt, ...);
int sprintf(varchar &dst, const char *fmt, ...);
int snprintf(varchar &dst, size_t sz, const char *fmt, ...);
int vsprintf(varchar &dst, const char *fmt, va_list ptr);
int vsnprintf(varchar &dst, size_t sz, const char *fmt, va_list ptr);

varchar &strcpy(varchar &dst, const char *src);
varchar &strcpy(varchar &dst, const char *src, size_t sz);
varchar &strcat(varchar &dst, const char *src);

#endif
