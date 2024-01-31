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

#include <double-vector.h>

#include "global-test.h"

/************************************************************/
int
TEST_DV_zzz( zzz )
{

  int error_count = 0;
  char msg[1024];

  if ( zzz )
    {
	 sprintf( msg, "Size %d list failed", t_list_size );
	 Error( msg );
    }

  return error_count;

} /* TEST_DV_zzz */

/************************************************************/
int
DV_unitTest()
{
  int error_count = 0;

  StartUnitTest( "double-vector.c" );

  error_count += TEST_DV_zzz();

  EndUnitTest( "double-vector.c" );

  return error_count;

} /* DV_unitTests */
