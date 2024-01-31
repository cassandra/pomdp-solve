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
 *   Data structure and routines for maintaining a dynamic list of
 *   indices.
 *
 *   A list of indices is an ordered list of non-negative integers.
 * 
 */

#include "global.h"
#include "xalloc.h"
#include "index-list.h"

/*************************************************************/
IndexList
IL_new()
{
  IndexList list;

  list = (IndexList) XMALLOC( sizeof( *list ));

  list->size = 0;

  list->element = (int *) XMALLOC( INITIAL_CAPACITY * sizeof(int));

  list->capacity = INITIAL_CAPACITY;

  return list;
} /* IL_new */

/*************************************************************/
void
IL_dispose( IndexList list )
{

  if ( list == NULL )
    {
	 Warning( "Cannot dispose NULL list." );
	 return;
    }

  XFREE( list->element );

  XFREE( list );

} /* IL_dispose */

/*************************************************************/
void
IL_resize( IndexList list )
{
  int i;
  int* old_element;

  if ( list == NULL )
    {
	 Warning( "Cannot resize NULL list." );
	 return;
    }

  old_element = list->element;

  list->element = (int *) XMALLOC( 2 * list->capacity * sizeof(int) );
  
  list->capacity *= 2;
  
  for ( i = 0; i < list->size; i++ )
    list->element[i] = old_element[i];

  XFREE( old_element );

} /* IL_resize */

/*************************************************************/
void
IL_append( IndexList list, int value )
{
  if ( list == NULL )
    {
	 Warning( "Cannot append to NULL list." );
	 return;
    }

  if ( list->size >= list->capacity )
    {
	 IL_resize( list );
    }

  list->element[list->size] = value;
  (list->size)++;
  
} /* IL_append */
/*************************************************************/
int
IL_sizeOf( IndexList list )
{
  if ( list == NULL )
    {
	 Warning( "Cannot get size of NULL list." );
	 return 0;
    }

  return list->size;

} /* IL_sizeOf */
