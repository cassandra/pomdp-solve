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

/*
 *   Header file for all globally defined items in the mdp library.
 */

#ifndef MDP_COMMON_H
#define MDP_COMMON_H

#ifdef DMALLOC

#include "dmalloc.h"

#define XCALLOC(num, size) calloc( (num), (size) )
#define XMALLOC(size) malloc( size )
#define XREALLOC(p, size) realloc( (p), (size) )
#define XFREE(stale) free(stale)

#else

#define XCALLOC(num, size) calloc( (num), (size) )
#define XMALLOC(size) malloc( size )
#define XREALLOC(p, size) realloc( (p), (size) )
#define XFREE(stale) free(stale)

#endif

#endif

