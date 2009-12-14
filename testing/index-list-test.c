/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    alpha.h
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    July, 1998
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
 *    $RCSfile: index-list-test.c,v $
 *    $Source: /u/cvs/proj/pomdp-solve/testing/index-list-test.c,v $
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
 *   Unit tests for index-list.[ch]
 * 
 */

#include <index-list.h>

#include "global-test.h"

/************************************************************/
int
TEST_IL_compare( IndexList index_list, 
			  int *test_list, 
			  int test_list_size )
{
  int i;

  for ( i = 0; i < test_list_size; i++ )
    {
	 if ( test_list[i] != index_list->element[i] )
	   return 0;
	 
    } /* for i */
  
  return 1;
  
} /* TEST_IL_compare */

/************************************************************/
void
TEST_IL_setTestList( int *test_list, int test_list_size )
{
  int i;

  /* Just set to a descending sequence. */
  for ( i = 0; i < test_list_size; i++ )
    {
	 test_list[i] = test_list_size - i;
    }
  
} /* TEST_IL_setTestList */

/************************************************************/
int
TEST_IL_append()
{
  /*
    Simple test that tries to create a few lists of varioous sizes and
    ensures the right values end up in the right locations.
   */

/* Set these if you want to change the number of maximum size of the
   lists that are used rfor testing. */
#define MAX_LIST_SIZE 12000
#define NUM_TEST_LISTS 10

  IndexList i_list;
  int t_list[MAX_LIST_SIZE];
  int t_list_size;
  int error_count = 0;
  int t, i;
  char msg[1024];

  int test_sizes[NUM_TEST_LISTS] = { 0, 1, 2, 9, 10, 11, 25, 100, 255, 12000 };

  for ( t = 0; t < NUM_TEST_LISTS; t++ )
    { 
	 t_list_size = test_sizes[t];

	 i_list = IL_new();

	 TEST_IL_setTestList( t_list, t_list_size );

	 for ( i = 0; i < t_list_size; i++ )
	   IL_append( i_list, t_list[i] );

	 if ( !  TEST_IL_compare( i_list, t_list, t_list_size ))
	   {
		sprintf( msg, "Size %d list failed", t_list_size );
		Error( msg );
	   }

	 if ( IL_sizeOf( i_list ) != t_list_size )
	   {
		
		sprintf( msg, "Size %d list reports wrong size", t_list_size );
		Error( msg );
	   }


	 IL_dispose( i_list );
	 
    } // for t */

  return error_count;

} /* TEST_IL_append */

/************************************************************/
int
IL_unitTest()
{
  int error_count = 0;

  StartUnitTest( "index-list.c" );

  error_count += TEST_IL_append();

  EndUnitTest( "index-list.c" );

  return error_count;
} /* TEST_IL_unitTests */
