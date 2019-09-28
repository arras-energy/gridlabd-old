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
 *   -D INITRC=n  return code from init()
 *
 */
#include <string.h>
#include <stdio.h>

#ifndef NOINIT
int init(void)
{
#ifndef INITRC
	printf("hello.c: initialization ok\n");
	return 0;
#else
	printf("hello.c: initialization failed\n");
	return INITRC;
#endif
}
#endif

#ifndef NOPARSER
int parser(const char *text)
{
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
