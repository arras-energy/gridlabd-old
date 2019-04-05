/*
 * -- SuperLU MT routine (version 1.0) --
 * Univ. of California Berkeley, Xerox Palo Alto Research Center,
 * and Lawrence Berkeley National Lab.
 * August 15, 1997
 *
 * This routine should NOT be optimized.
 */
int await(volatile int *status)
{
    while ( *status ) ;
    return 0;
}
