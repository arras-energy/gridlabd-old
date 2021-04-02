// gldcore/string.h
// Copyright (C) 2021, Regents of the Leland Stanford Junior University

#include "varchar.h"

#define ASSERT(TEST,MESSAGE) if ( !(TEST) ) { throw MESSAGE; }

varchar::varchar()
{
	str = NULL;
	sz = 0;
}

varchar::varchar(const char *src, size_t len)
{
	if ( src )
	{
		if ( len == 0 )
		{
			str = strdup(src);
			ASSERT(str!=NULL,"varchar::varchar() strdup failed");
			sz = strlen(src)+1;
		}
		else
		{
			str = (char*)malloc(len+1);
			ASSERT(str!=NULL,"varchar::varchar() malloc failed");
			strncpy(str,src,len);
			str[len] = '\0';
			sz = len+1;
		}
	}
	else
	{
		str = NULL;
		sz = 0;
	}
}

varchar::varchar(size_t len)
{
	if ( len > 0 )
	{
		str = (char*)malloc(len+1);
		ASSERT(str!=NULL,"varchar::varchar() malloc failed");
		memset(str,0,len+1);
		sz = len+1;
	}
	else
	{
		str = NULL;
		sz = 0;
	}
}

varchar::varchar(varchar &s)
{
	if ( s.str != NULL && s.sz > 0 )
	{
		str = strdup(s.get_string());
		ASSERT(str!=NULL,"varchar::varchar() strdup failed");
		sz = s.sz;
	}
	else
	{
		str = NULL;
		sz = 0;
	}
}

varchar::~varchar()
{
	if ( str != NULL )
	{
		free(str);
	}
	str = NULL;
	sz = 0;
}

void varchar::_realloc(char *s, size_t len)
{
	if ( str != NULL )
	{
		free(str);
	}
	str = s;
	if ( len < 0 )
	{
		len = strlen(s);
	}
	sz = len;
}

char *varchar::resize(size_t len, bool keep)
{
	if ( len > sz )
	{
		char *tmp = (char*)malloc(len+1);
		ASSERT(tmp!=NULL,"varchar::resize() malloc failed");
		if ( str != NULL )
		{
			if ( keep )
			{
				strcpy(tmp,str);
			}
			else
			{
				memset(tmp,0,len+1);
			}
			free(str);
		}
		else
		{
			memset(tmp,0,len+1);
		}
		str = tmp;
		sz = len;
	}
	return str;
}

void varchar::clear(size_t len, int c)
{
	memset(resize(len),c,len);
}

void varchar::copy(const char *s, size_t len)
{
	if ( len < 0 )
	{
		len = strlen(s);
	}
	resize(len,false);
	strcpy(str,s);
}

void varchar::copy(varchar &s)
{
	copy(s.str);
}

size_t varchar::length()
{
	return str ? strlen(str) : 0;
}

int varchar::format(const char *fmt, ...)
{
	va_list ptr;
	va_start(ptr,fmt);
	size_t len = format(fmt,ptr);
	va_end(ptr);
	return len;
}

int varchar::format(const char *fmt, va_list ptr)
{
	char *buf;
	size_t len = vasprintf(&buf,fmt,ptr);
	ASSERT(len>=0,"varchar::format() vasprintf failed");
	_realloc(buf,len);
	return len;
}

int varchar::format(size_t size, const char *fmt, va_list ptr)
{
	char *buf;
	size_t len = vasprintf(&buf,fmt,ptr);
	ASSERT(len>=0,"varchar::format() vasprintf failed");
	if ( len > size )
	{
		buf[len] = '\0';
	}
	_realloc(buf,len);
	return len;
}

void varchar::append(const char *s)
{
	size_t len = strlen(str);
	size_t slen = strlen(s);
	resize(len+slen+1,true);
	strcpy(str+len,s);
	return;
}

size_t strlen(varchar &dst)
{
	return dst.length();
}
void *memset(varchar &dst, int c, size_t len)
{
	dst.clear(len,c);
	return (void*)dst.get_string();
}

int sprintf(varchar &dst, const char *fmt, ...)
{
	va_list ptr;
	va_start(ptr,fmt);
	size_t len = dst.format(fmt,ptr);
	va_end(ptr);
	return len;
}

int snprintf(varchar &dst, size_t sz, const char *fmt, ...)
{
	va_list ptr;
	va_start(ptr,fmt);
	size_t len = dst.format(sz,fmt,ptr);
	va_end(ptr);
	return len;
}

int vsprintf(varchar &dst, const char *fmt, va_list ptr)
{
	return dst.format(fmt,ptr);
}

int vsnprintf(varchar &dst, size_t sz, const char *fmt, va_list ptr)
{
	return dst.format(sz,fmt,ptr);
}

varchar &strcpy(varchar &dst, const char *src)
{
	dst.copy(src);
	return dst;
}

varchar &strcpy(varchar &dst, const char *src, size_t sz)
{
	dst.copy(src);
	return dst;
}

varchar &strcat(varchar &dst, const char *src)
{
	dst.append(src);
	return dst;
}



