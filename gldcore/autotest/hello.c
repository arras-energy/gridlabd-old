/* hello.c
 * 
 * This is a loader extension test code. 
 * It adds the term "hello" to the loader's valid syntax.
 *
 * Normal usage:
 *   bash% gcc -shared -o hello.so -fPIC ../hello.c
 *
 * Testing ptions
 *   -D NOINIT    omit init() function
 *   -D NOPARSER  omit parser() function
 *   -D INITRC    return code from init()
 *
 */
#include <string.h>
#include <stdio.h>

#ifndef NOINIT
int init(void)
{
	printf("initialization ok");
#ifndef INITRC
	return 0;
#else
	return INITRC;
#endif
}
#endif

#ifndef NOPARSER
int parser(const char *text)
{
	int len = 0;
	if ( strncmp(text,"hello",5) == 0 )
	{
		return 5;
	}
	else
	{
		return 0;
	}
}
#endif
