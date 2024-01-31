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
 *   Wrappers for memory allocation routines.
 */

#include <stdlib.h>
#include <string.h>
#include "common.h"

/**********************************************************************/
void *
XA_malloc (size_t num)
{
  void *new = (void *) malloc (num);
  if (!new)
    Abort("Out of memory in malloc()!");
  return new;
}
/**********************************************************************/
void *
XA_realloc (void *p, size_t num)
{
  void *new;

  if (!p)
    return XA_malloc (num);

  new = (void *) realloc (p, num);
  if (!new)
    Abort("Out of memory in realloc()!");

  return new;
}
/**********************************************************************/
void *
XA_calloc (size_t num, size_t size)
{
  void *new;

#if HAVE_MEMSET
  new = XA_malloc (num * size);
  memset(new, '\0', num * size);
#else

#if HAVE_BZERO
  new = XA_malloc (num * size);
  bzero(new, num * size);
#else
  new = calloc (num, size);
  if (!new)
    Abort("Out of memory in calloc()!");
#endif

#endif

  return new;
}
/**********************************************************************/
