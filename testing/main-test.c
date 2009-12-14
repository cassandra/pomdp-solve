/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    main.c
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    August, 1998
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
 *    $RCSfile: main-test.c,v $
 *    $Source: /u/cvs/proj/pomdp-solve/testing/main-test.c,v $
 *    $Revision: 1.2 $
 *    $Date: 2005/02/02 23:21:31 $
 *  </RCS_KEYWORD>
 *
 *  <COPYRIGHT>
 *
 *    2005, Anthony R. Cassandra
 *
 *    All Rights Reserved
 *                          
 *    Permission to use, copy, modify, and distribute this software and its
 *    documentation for any purpose other than its incorporation into a
 *    commercial product is hereby granted without fee, provided that the
 *    above copyright notice appear in all copies and that both that
 *    copyright notice and this permission notice appear in supporting
 *    documentation.
 * 
 *    ANTHONY CASSANDRA DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 *    INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY
 *    PARTICULAR PURPOSE.  IN NO EVENT SHALL ANTHONY CASSANDRA BE LIABLE FOR
 *    ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *  </COPYRIGHT>
 *
 *</SOURCE_HEADER>
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

