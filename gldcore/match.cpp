/* $Id: match.c 4738 2014-07-03 00:55:39Z dchassin $
 *	From _Beautiful Code_, edited by Andy Oram and Greg Wilson, CR 2007 O'Reilly Media, Inc.
 *	"A Regular Expression Matcher", Brian Kernighan
 *
 *	c - matches any literal character c
 *	. - matches any single character 
 *	^ - matches the beginning of the input string 
 *	$ - matches the end of the input string 
 *	* - matches zero or more occurences of the previous character
 *
 */

#include "gldcore.h"

// SET_MYCONTEXT(DMC_MATCH) // used only if IN_MYCONTEXT is present in this module

DEPRECATED CDECL int match(const char *regexp, const char *text)
{
	GldRegex re(regexp);
	return re.match(text);
}

GldRegex::GldRegex(const char *regexp)
{
	re = strdup(regexp);
}

GldRegex::~GldRegex(void)
{
	free((void*)re);
}

int GldRegex::match(const char *text)
{
	return match(re,text);
}

/* match: search for regexp anywhere in text */ 
int GldRegex::match(const char *regexp, const char *text)
{
	if ( regexp[0] == '^' )
	{
		return matchhere(regexp+1, text);
	}
	do 
	{    /* must look even if string is empty */
		if ( matchhere(regexp, text) )
		{
			return 1;
		}
	} while ( *text++ != '\0' );
	return 0;
}

/* matchhere: search for regexp at beginning of text */
int GldRegex::matchhere(const char *regexp, const char *text)
{
	int force = 0;
	if ( regexp[0] == '\\' )
	{
		force = 1;
	}
	if ( regexp[0] == '\0' )
	{
		return 1;
	}
	if ( (regexp[1] == '*') && !force )
	{
		return matchstar(regexp[0], regexp+2, text);
	}
	if ( regexp[0] == '$' && regexp[1] == '\0' )
	{
		return *text == '\0';
	}
	if ( *text!='\0' && ((regexp[0]=='.' && !force) || (regexp[1]==*text && force) || regexp[0]==*text) )
	{
		return matchhere(regexp+1+force, text+1);
	}
	return 0;
}

int GldRegex::matchhere_orig(const char *regexp, const char *text)
{
	if ( regexp[0] == '\0' )
	{
		return 1;
	}
	if ( regexp[1] == '*' )
	{
		return matchstar(regexp[0], regexp+2, text);
	}
	if ( regexp[0] == '$' && regexp[1] == '\0' )
	{
		return *text == '\0';
	}
	if ( *text!='\0' && (regexp[0]=='.' || regexp[0]==*text) )
	{
		return matchhere(regexp+1, text+1);
	}
	return 0;
}

/* matchstar: search for c*regexp at beginning of text */
int GldRegex::matchstar(int c, const char *regexp, const char *text)
{
	do
	{     /* a * matches zero or more instances */
		if ( matchhere(regexp, text) )
		{
			return 1;
		}
	} while ( *text != '\0' && (*text++ == c || c == '.') );
	return 0;
}

/* EOF */
