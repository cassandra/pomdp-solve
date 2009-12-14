/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    params.c
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    July, 1998
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
 *    $RCSfile: double-vector-test.c,v $
 *    $Source: /u/cvs/proj/pomdp-solve/testing/double-vector-test.c,v $
 *    $Revision: 1.1 $
 *    $Date: 2005/02/02 23:21:31 $
 *  </RCS_KEYWORD>
 *
 *  <COPYRIGHT>
 *
 *    2005 Anthony R. Cassandra
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


#include <double-vector.h>

#include "global-test.h"

/* From the mdp.a library which will be linked in. */

extern int gNumStates;

/************************************************************/
int
TEST_DV_vectors( )
{

#define NUM_STATE_SIZES   3

  int num_states[NUM_STATE_SIZES] = { 5, 20, 250 };

  int error_count = 0;
  char msg[1024];
  DoubleVector dv1, dv2;
  int i, n;

  for( n = 0; n < NUM_STATE_SIZES; n++ )
    {
	 gNumStates = num_states[n];

	 dv1 = DV_new();
	 for ( i = 0; i < gNumStates; i++ )
	   dv1[i] = gNumStates - i;
	 
	 dv2 = DV_duplicate( dv1 );
	 
	 if ( ! DV_isSame( dv1, dv2, 1e-9 ))
	   Error( "Duplicated vector failed isSame() test" );
	 
	 for ( i = 0; i < gNumStates; i++ )
	   dv2[i] = i;
	 
	 if ( DV_isSame( dv1, dv2, 1e-9 ))
	   Error( "Differing vectors passed isSame() test" );

	 if ( DV_isLexicographicallyBetter( dv2, dv1, 1e-9 ))
	   Error( "Smaller vector passed isLexicographicallyBetter() test" );

	 if ( ! DV_isLexicographicallyBetter( dv1, dv2, 1e-9 ))
	   Error( "Larger vector failed isLexicographicallyBetter() test" );

	 DV_copy( dv1, dv2 );

	 if ( ! DV_isSame( dv1, dv2, 1e-9 ))
	   Error( "Copied vector failed isSame() test" );

	 if ( DV_isLexicographicallyBetter( dv1, dv2, 1e-9 ))
	   Error( "Equal vector passed isLexicographicallyBetter() test" );

	 if ( DV_isDominated( dv1, dv2 ))
	   Error( "Equal vector passed isDominated() test" );

	 for ( i = 0; i < gNumStates; i++ )
	   dv2[i] = dv1[i] + 1e-9;

	 if ( DV_isDominated( dv1, dv2 ))
	   Error( "Dominated vector failed isDominated() test" );

	 if ( DV_isLexicographicallyBetter( dv2, dv1, 1e-7 ))
	   Error( "Small diff vector passed isLexicographicallyBetter() test" );

	 if ( ! DV_isLexicographicallyBetter( dv2, dv1, 1e-11 ))
	   Error( "Diff vector failed isLexicographicallyBetter() test" );

	 DV_copy( dv1, dv2 );
	 dv2[gNumStates-1] = dv1[gNumStates-1] + 1e-4;

	 if ( ! DV_isLexicographicallyBetter( dv2, dv1, 1e-7 ))
	   Error( "Diff vector failed isLexicographicallyBetter() test" );

	 DV_copy( dv1, dv2 );

	 for ( i = 0; i < gNumStates; i++ )
	   dv2[i] = dv1[i] - 1e-9;
	 dv2[gNumStates-1] = dv1[gNumStates-1] + 1e-4;

	 if ( ! DV_isLexicographicallyBetter( dv2, dv1, 1e-7 ))
	   Error( "Diff vector failed isLexicographicallyBetter() test" );
	 
	 for ( i = 0; i < gNumStates; i++ )
	   {  
		if ( DV_isZero( dv1, 1e-9 ))
		  Error( "Non-zero vector failed isZero() test" );
		
		dv1[i] = 0.0;
	   }
	 
	 if ( ! DV_isZero( dv1, 1e-9 ))
	   Error( "Zero vector failed isZero() test" );
	 
	 DV_destroy( dv1 );
	 DV_destroy( dv2 );

    } /* for n */

  return error_count;

} /* TEST_DV_vectors */

/************************************************************/
int
TEST_DV_lists( )
{
#define NUM_LIST_SIZES   5

  int list_sizes[NUM_LIST_SIZES] = { 0, 1, 2, 10, 250 };

  int error_count = 0;
  char msg[1024];
  DoubleVector dv1, dv2;
  DoubleVectorNode node1, node2;
  DoubleVectorList list1, list2, list3;
  int i, n, s;

  /* Not worrying about different values here. */
  gNumStates = 10;

  for ( n = 0; n < NUM_LIST_SIZES; n++ )
    { 

	 /**************/
	 /* PREPENDING */
	 /**************/

	 list1 = DV_newList( NULL );

	 for ( i = 0; i < list_sizes[n]; i++ )
	   { 
		/* Make vector all same number */
		dv1 = DV_new();
		dv2 = DV_new();
		for ( s = 0; s < gNumStates; s++ )
		  {
		    dv1[s] = i;
		    dv2[s] = i;
		  }

		DV_prependList( list1, dv1, NULL );

		if ( ! DV_isSame( list1->head->v, dv2, 1e-9 ))
		  Error( "Prepend not putting items at front of list" );

		DV_destroy( dv2 );

	   } /* for i */

	 if ( DV_sizeOfList( list1 ) != list_sizes[n] )
	   {
		sprintf( msg, "Post-prepend sizeOf() failed for list size %d", 
			    list_sizes[n] );
		Error( msg );
	   }

	 DV_destroyList( list1 );


	 /*************/
	 /* APPENDING */
	 /*************/

	 list1 = DV_newList( NULL );

	 for ( i = 0; i < list_sizes[n]; i++ )
	   { 
		/* Make vector all same number */
		dv1 = DV_new();
		dv2 = DV_new();
		for ( s = 0; s < gNumStates; s++ )
		  {
		    dv1[s] = i;
		    dv2[s] = i;
		  }

		DV_appendList( list1, dv1, NULL );

		if ( ! DV_isSame( list1->tail->v, dv2, 1e-9 ))
		  Error( "Append not putting items at end of list" );

		DV_destroy( dv2 );

	   } /* for i */

	 if ( DV_sizeOfList( list1 ) != list_sizes[n] )
	   {
		sprintf( msg, "Post-append sizeOf() failed for list size %d", 
			    list_sizes[n] );
		Error( msg );
	   }

	 /***************/
	 /* RENUMBERING */
	 /***************/

	 if ( list1->head != NULL )
	   {
		list1->head->id = 89;
		list1->tail->id = 67;
	   }

	 DV_renumberList( list1);

	 node1 = list1->head;
	 i = 0;
	 while( node1 != NULL )
	   {
		if ( node1->id != i )
		  Error( "Renumbering list failure." );

		node1 = node1->next;
		i++;
	   }

	 /***********/
	 /* FINDING */
	 /***********/

	 /* Make something not in the list */
	 dv2 = DV_new();
	 for ( s = 0; s < gNumStates; s++ )
	   dv2[s] = list_sizes[n] + 10;

	 if ( DV_findNode( list1, dv2, 1e-9 ) != NULL )
	   Error( "Find node succeeded for bogus vector." );

	 DV_destroy( dv2 );

	 /* Now search for each thing in the list (depends on specially
	    appending the vectors and renumbering the list. */ 
	 for ( i = 0; i < list_sizes[n]; i++ )
	   { 
		/* Make vector all same number as we did when we added
		   them. */
		dv2 = DV_new();
		for ( s = 0; s < gNumStates; s++ )
		    dv2[s] = i;

		node1 = DV_findNode( list1, dv2, 1e-9 );
		
		if ( node1 == NULL )
		  Error( "Could not find valid node in list." );

		if (( node1 != NULL )
		    && (( ! DV_isSame( node1->v, dv2, 1e-9 ))
			   || ( node1->id != i )))
		  {
		    sprintf( msg, "Trouble finding node %d in list size %d.",
				   i, list_sizes[n] );
		    Error( msg );
		  }
		
		DV_destroy( dv2 );

	   } /* for i */

	 /***********/
	 /* COPYING */
	 /***********/

	 list2 = DV_duplicateList( list1 );

	 if ( ! DV_isSameList( list1, list2, 1e-9 ))
	   Error( "Failed isSameList() after copying." );

	 /* Then here's a manual list comparison with error messages. */

	 node1 = list1->head;
	 node2 = list2->head;
	 while (( node1 != NULL ) && ( node2 != NULL ))
	   {
		if ( node1 == node2 )
		  Error( "Same node pointers after list copy." );

		if ( node1->v == node2->v )
		  Error( "Same vector pointers after list copy." );

		if ( node1->id != node2->id )
		  Error( "Mismatched IDs after list copy." );

		if ( ! DV_isSame( node1->v, node2->v, 1e-9 ))
		  Error( "Non-matching vectors after list copy." );

		node1 = node1->next;
		node2 = node2->next;
	   }

	 if (( node1 != NULL ) || ( node2 != NULL ))
		Error( "Non-matching list lengths after list copy." );

	 DV_destroyList( list2 );

	 /************/
	 /* CLEARING */
	 /************/

	 DV_clearList( list1 );

	 if ( DV_sizeOfList( list1 ) != 0 )
	   {
		sprintf( msg, "Clear list failed for list size %d", 
			    list_sizes[n] );
		Error( msg );
	   }

	 DV_destroyList( list1 );


	 /**************/
	 /* SIMILARITY */
	 /**************/

	 list1 = DV_newList( NULL );
	 list2 = DV_newList( NULL );

	 for ( i = 0; i < list_sizes[n]; i++ )
	   { 
		/* Make vector all same number */
		dv1 = DV_new();
		dv2 = DV_new();
		for ( s = 0; s < gNumStates; s++ )
		  {
		    dv1[s] = i;
		    dv2[s] = i;
		  }

		DV_appendList( list1, dv1, NULL );
		DV_prependList( list2, dv2, NULL );

	   } /* for i */

	 if (( list_sizes[n] > 1 )
		&& ( DV_isSameList( list1, list2, 1e-9 )))
	   Error( "Similar list being reported as identical." );

	 if ( ! DV_isSimilaraList( list1, list2, 1e-9 ))
	   Error( "Similar lists failing isSimilarList() test." );

	 dv1 = DV_new();
	 for ( s = 0; s < gNumStates; s++ )
	   dv1[s] = -600;

	 DV_appendList( list1, dv1, NULL );
	 
	 if ( DV_isSimilaraList( list1, list2, 1e-9 ))
	   Error( "Dissimilar lists passing isSimilarList() test." );

	 DV_destroyList( list1 );
	 DV_destroyList( list2 );

    } /* for n */


  /*********/
  /* UNION */
  /*********/

  list1 = DV_newList( NULL );
  list2 = DV_newList( NULL );
  list3 = DV_newList( NULL );

  for ( i = 0; i < 47; i++ )
    { 
	 /* Make vector all same number */
	 dv1 = DV_new();
	 dv2 = DV_new();
	 for ( s = 0; s < gNumStates; s++ )
	   {
		dv1[s] = i;
		dv2[s] = i;
	   }

		DV_appendList( list1, dv1, NULL );
		DV_appendList( list3, dv2, NULL );

    } /* for i */

  for ( i = 47; i < 91; i++ )
    { 
	 /* Make vector all same number */
	 dv1 = DV_new();
	 dv2 = DV_new();
	 for ( s = 0; s < gNumStates; s++ )
	   {
		dv1[s] = i;
		dv2[s] = i;
	   }

		DV_appendList( list2, dv1, NULL );
		DV_appendList( list3, dv2, NULL );

    } /* for i */

  DV_unionTwoLists( list1, list2 );

  if ( DV_sizeOfList( list1 ) != 91 )
    Error( "Wrong list size after list union." );

  if ( ! DV_isSameList( list1, list3, 1e-9 ))
    Error( "Union of two lists failed." );

  DV_destroyList( list1 );
  /* union operation should free list2 memory */
  DV_destroyList( list3 );

  return error_count;

} /* TEST_DV_lists */

/************************************************************/
int
DV_unitTest()
{
  int error_count = 0;

  StartUnitTest( "double-vector.c" );

  error_count += TEST_DV_vectors();
  error_count += TEST_DV_lists();

  EndUnitTest( "double-vector.c" );

  return error_count;

} /* DV_unitTests */
