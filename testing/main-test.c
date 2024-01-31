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
 *   Main driver for unti tests of pomdp-solve and related
 *   programs/code.  
 */

#define MAIN_C

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*  To allow testing routines to be added and removed easily, we store
    the routine names in an array and loop over them in the main()
    function.  So first we need function prototypes for the test
    routines, then an array of these test routines. We also have an
    array of strings for giving a message about what it is testing and
    a boolean array of flags so we can pick and choice which routines
    to test. Adding a new test routine requires not only adding the
    routine and its forward declaration to this file, but making the
    entry in each one of the arrays below. 
*/

/* Forward declaration so we can reference them in the array. */
extern int IL_unitTest(  );
extern int DV_unitTest(  );

/* The array of function names to be used for testing.  The code loops
   over this array, calling each function in sequential order. */
static void *TestRoutines[] = {
  IL_unitTest,
  DV_unitTest,
  NULL
};


/**********************************************************************/
int 
main( int argc, char **argv ) {

  int i;
  int error_count = 0;

  printf( "\n++++++++++++++++++++++++++++++++++++++++\n");
  printf( "            POMDP Units Tests \n" );
  printf( "++++++++++++++++++++++++++++++++++++++++\n\n");

  for ( i = 0; TestRoutines[i] != NULL; i++ )
    {
	 /* Yuck, this is messy.  The rpice you pay for compactness and
	    extendibility. The ((void (*)(void)) part casts the array
	    element into a function pointer and then the (*x)() (where x is
	    the other mess) is the dereferencing and calling portion. */

	 error_count += (*((int (*)(void)) TestRoutines[i]))();
    
    } /* for i */

  if ( error_count > 0 )
    {
	 printf( "\n++++++++++++++++++++++++++++++++++++++++\n");
	 printf( "Testing Completed: FAILED. Total Errors = %d\n", error_count );
	 printf( "++++++++++++++++++++++++++++++++++++++++\n");
	 return 1;
    }

  printf( "\n++++++++++++++++++++++++++++++++++++++++\n");
  printf( "Testing Completed: PASSED.\n" );
  printf( "++++++++++++++++++++++++++++++++++++++++\n");

  return 0;

} /* main */
/**********************************************************************/

