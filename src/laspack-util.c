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
 * laspack-util.c
 *
 * Miscellaneous utilities for LASPACK data structures.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* The LAPACK modules */
#include <laspack/errhandl.h>
#include <laspack/itersolv.h>
#include <laspack/lastypes.h>
#include <laspack/operats.h>
#include <laspack/precond.h>
#include <laspack/qmatrix.h>
#include <laspack/rtc.h>
#include <laspack/vector.h>

#include "global.h"
#include "laspack-util.h"

/**********************************************************************/
/********   Utility routines for LASPACK library     ******************/
/**********************************************************************/

/**********************************************************************/
void showVector( Vector *v ) {
  int i;

  printf( "Vector: %s\n", V_GetName( v ) );
  
  for ( i = 0; i < V_GetDim( v ); i++ )
      printf( "%.3lf ", V_GetCmp( v, i+1 ) );
  printf( "\n" );

} /* showVector */
/**********************************************************************/
void showQMatrix( QMatrix *M ) {
  int i, j;

  printf( "Matrix: %s\n", Q_GetName( M ) );
  
  for ( i = 0; i < Q_GetDim( M ); i++ ) {
    printf( "Row %d: ", i );
    for ( j = 0; j < Q_GetDim( M ); j++ )
      if ( Equal( Q_GetEl( M, i+1, j+1 ), 0.0, 1e-5 ))
        continue;
      else
        printf( "(%d) %.3lf ", j, Q_GetEl( M, i+1, j+1 ) );
    printf( "\n" );
  } /* for i */

}  /* showQMatrix */
/**********************************************************************/
