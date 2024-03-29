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
  Module: AA

  A very simple associative array that maps string keys to string
  values. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "associative-array.h"

/*******************************************************/
AssocArray 
AA_new( int size, int width )
{
  /*
    Constructor that create memory for associative array.  Takes two
    sizes to simplify the memory management.
   */
  AssocArray assoc;
  int i;

  assoc = (AssocArray) XMALLOC( sizeof( *assoc ));

  assoc->max_size = size;
  assoc->cur_size = 0;

  assoc->keys = (char **) XMALLOC( size * sizeof(char*));
  assoc->values = (char **) XMALLOC( size * sizeof(char*));
  for ( i = 0; i < size; i++ )
    {
	 assoc->keys[i] = (char *) XMALLOC( width * sizeof(char));
	 assoc->values[i] = (char *) XMALLOC( width * sizeof(char));

    } /* for i */

  return assoc;

} /* AA_new */
/*******************************************************/
void
AA_delete( AssocArray assoc )
{
  /*
    Associative array destructor that free all the memory that was
    dynamicaly allocated.
   */
  int i;

  if ( assoc == NULL )
    return;

  for ( i = 0; i < assoc->max_size; i++ )
    {
	 XFREE( assoc->keys[i] );
	 XFREE( assoc->values[i] );
    } /* for i */

  XFREE( assoc->keys );
  XFREE( assoc->values );

  XFREE( assoc );

} /* AA_delete */

/*******************************************************/
int
AA_size( AssocArray assoc )
{
  /*
    Returns the current size (used spaces) in the associative array,
    or -1 if the array is NULL.
  */
  if ( assoc == NULL )
    return -1;

  return assoc->cur_size;

} /* AA_size */

/*******************************************************/
int
AA_put( AssocArray assoc, char* key, char* value )
{
  /*
    Puts an entry into the associative array. If it is a new key,
    returns '1'. If it replaces an existing key, it returns 0.  If it
    is not added due to some other error it returns -1. (e.g., array
    is full, bad args. )
  */
  int i;

  if (( assoc == NULL ) || ( key == NULL ) || ( value == NULL ))
    return -1;

  for ( i = 0; i < assoc->cur_size; i++ )
    {
	 if ( strcmp( assoc->keys[i], key ) == 0 )
	   {
		strcpy( assoc->values[i], value );
		return 0;
	   } /* if found existing key */
    } /* for i */

  /* else, new entry to add */
  if ( assoc->cur_size >= assoc->max_size )
    return -1;

  strcpy( assoc->keys[assoc->cur_size], key );
  strcpy( assoc->values[assoc->cur_size], value );
  assoc->cur_size += 1;

  return 1;
} /* AA_put */

/*******************************************************/
char*
AA_get( AssocArray assoc, char* key )
{
  /*
    Retrieve an entry from the associative aray based on its key.
    Returns NULL is the key has no value in the array.
   */
  int i;

  if (( assoc == NULL ) || ( key == NULL ))
    return NULL;

  for ( i = 0; i < assoc->cur_size; i++ )
    {
	 if ( strcmp( assoc->keys[i], key ) == 0 )
	   {
		return ( assoc->values[i] );
	   } /* if found existing key */
    } /* for i */

  return NULL;

} /* AA_put */

/*******************************************************/
void
AA_fprintf( AssocArray assoc, FILE* file )
{
  int i;

  if ( assoc == NULL )
    {
	 fprintf( file, "NULL" );
	 return;
    }
  
  for ( i = 0; i < assoc->cur_size; i++ )
    {
	 fprintf( file, "%s = %s\n", assoc->keys[i], assoc->values[i] );
    } /* for i */
  
  
} /* AA_fprintf */

/*******************************************************/
void
AA_printf( AssocArray assoc )
{
  AA_fprintf( assoc, stdout );
} /* AA_printf */
