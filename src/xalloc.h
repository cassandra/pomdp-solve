/*
 * pomdp-solve
 *
 * Copyright © 1998-2003, Anthony R. Cassandra
 * Copyright © 1994-1997, Brown University
 *
 * Licensed under the Creative Commons Attribution-NonCommercial 4.0 
 * International License (CC BY-NC 4.0).
 *
 * See the LICENSE.txt file for details.
 */

#include <stdlib.h>

#ifndef XALLOC_H
#define XALLOC_H

#define XCALLOC(num, size) XA_calloc( (num), (size) )
#define XMALLOC(size) XA_malloc( size )
#define XREALLOC(p, size) XA_realloc( (p), (size) )
#define XFREE(stale)                            do {        \
        if (stale) { free (stale);  stale = 0; }            \
                                                } while (0)

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

/*
 *   Wrappers for memory allocation routines.
 */
extern void *XA_malloc (size_t num);
extern void *XA_realloc (void *p, size_t num);
extern void *XA_calloc (size_t num, size_t size);

#endif
