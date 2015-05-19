/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    finite-grid.c
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    April, 2003
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
 *    $RCSfile: finite-grid.c,v $
 *    $Source: /u/cvs/proj/pomdp-solve/src/finite-grid.c,v $
 *    $Revision: 1.12 $
 *    $Date: 2005/02/22 18:49:57 $
 *  </RCS_KEYWORD>
 *
 *  <COPYRIGHT>
 *
 *    2003, Anthony R. Cassandra
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
  For solving a POMDP using only a finite set of belief points.
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "mdp/mdp.h"

#include "global.h"
#include "pomdp.h"
#include "alpha.h"
#include "parsimonious.h"
#include "belief.h"
#include "params.h"
#include "projection.h"
#include "enumeration.h"
#include "linear-support.h"
#include "two-pass.h"
#include "witness.h"
#include "inc-prune.h"
#include "finite-grid.h"
#include "random.h"
#include "common.h"

/* This contains the belief states that define the finite grid. 
*/
BeliefList gFiniteGrid = NULL;

/* A useful work area */
double *gFGTempBelief = NULL;

/**********************************************************************/
double FG_computeError( AlphaList prev_list, 
				    AlphaList cur_list,
				    PomdpSolveParams param )
{
  /* The finite grid method uses its own stopping criteria, and this
	is the method that returns the error difference between two alpha
	vector lists for the given set of belief points in the grid. 
	
	Calculates the maximal difference between to alpha vector list
	with respect to a zset of belief points.  Looks at each belief
	point in the list and the value of the two value functions.  Thus,
	it does not compare these over the whole belief simplex.
  */
  BeliefList b_ptr;
  double max_diff, prev_val, cur_val, diff;
  AlphaList dummy;

  max_diff = -1.0 * HUGE_VAL;

  b_ptr = gFiniteGrid;
  while( b_ptr != NULL )
    {

	 prev_val = bestVectorValue( prev_list, b_ptr->b,
						    &dummy, param->alpha_epsilon );
	 cur_val = bestVectorValue( cur_list, b_ptr->b,
						   &dummy, param->alpha_epsilon );

	 diff = fabs( prev_val - cur_val );
	 
	 if ( diff > max_diff )
	   max_diff = diff;

	 b_ptr = b_ptr->next;
    } /* while b_ptr != NULL */

  return max_diff;

} /* FG_computeError */

/**********************************************************************/
/* A breadth first search of belief space using a gievn list and
   marking those that have been searched. */
BeliefList 
searchAndAddBeliefStates( BeliefList belief_list, 
					 int size_limit,
					 double belief_epsilon )
{
  int action, obs;
  BeliefList walk_ptr;
  int added_belief_state = 1;
  int cur_size;

  if ( belief_list == NULL )
    return NULL;

  cur_size = sizeBeliefList( belief_list );

  while (( cur_size <= size_limit )
	    && ( added_belief_state )) {

    walk_ptr = belief_list;
    added_belief_state = 0;

    /* Breadth first search, so we first will generate a bunch of
	  belief states, add then to the list (unless they are duplicates),
	  then at the end, recurse on any new ones.
    */
    while( walk_ptr != NULL ) {
	 
	 /* Skip belief sttaes that have already been transformed. */
	 if ( walk_ptr->mark ) {
	   walk_ptr = walk_ptr->next;
	   continue;
	 }
	 
	 /* Mark the belief state indicating it has been transformed. */
	 walk_ptr->mark = 1;
	 
	 /* For each action */
	 for ( action = 0; action < gNumActions; action++ ) {
	   
	   /* For each observation */
	   for ( obs = 0; obs < gNumObservations; obs++ ) {
		
		/* Transform the belief state.  If the observation is
		   impossible for this belief state, then we just move
		   on. */
		if ( ! transformBeliefState( walk_ptr->b,
							    gFGTempBelief,
							    action,
							    obs )){
		  continue;
		}

		/* If it is not in list, add it and then recurse */
		if ( findBeliefState( belief_list, gFGTempBelief, 
						  belief_epsilon ) == NULL ) {
		  
		  belief_list = appendBeliefList( belief_list, gFGTempBelief );
		  added_belief_state = 1;
		  cur_size++;
		}
		
		/* else if it is in the list, do not add and  do not recurse. */
		
	   } /* for obs */
	 } /* for action */

	 /* Inside this loop, we are constantly adding belief points to
	    the end of the list, but since this is the list we are
	    walking down, the end can keep getting further and further
	    away, thereby never reaching this inner loop termination, and
	    thus not seeing the outer loops criteria for stopping when
	    the size gets too big, Thus, we insert this short-circuit. 
	 */
	 if ( cur_size > size_limit )
	   break;

	 walk_ptr = walk_ptr->next;
	 
    } /* while walk_ptr */
  } /* while cur_size < size_list and added_belief_state */

  return belief_list;

} /* searchAndAddBeliefStates */

/**********************************************************************/
BeliefList 
genAllBeliefStates( PomdpSolveParams param )
{
  /* Initial belief states are derived by grouping states into
	equivalence classes based on observations.
  */
  int action, obs, state, num_found;
  double prob;
  BeliefList belief_list = NULL;

  /* FIXME: Decide if adding this initial uniform belief state should
	stay or go. 
  */ 
  for( state = 0; state < gNumStates; state++ )
    gFGTempBelief[state] = 1.0 / gNumStates;
  
  belief_list = appendBeliefList( belief_list, gFGTempBelief );

  /* For each action-observation combination */
  for ( action = 0; action < gNumActions; action++ ) {
    for ( obs = 0; obs < gNumObservations; obs++ ) {

	 /* Zero out the belief state */
	 for( state = 0; state < gNumStates; state++ )
	   gFGTempBelief[state] = 0.0;
	 
	 num_found = 0;
	 
	 /* Find all states that have this observation */
	 for( state = 0; state < gNumStates; state++ ) {
	   
	   prob = getEntryMatrix( R[action], state, obs );
	   
	   if ( Equal( prob, 0.0, param->opts->fg_epsilon ))
		continue;

	   /* otherwise we found a state that can observe 'z' */
	   
	   gFGTempBelief[state] = 1.0;
	   num_found++;

	 } /* for state */

	 /* Create a belief state uniform over this set of states */
	 for( state = 0; state < gNumStates; state++ )
	   gFGTempBelief[state] = gFGTempBelief[state] / num_found;
 
	 /* Add the belief state to the list */
	 if ( findBeliefState( belief_list, gFGTempBelief, 
					   param->opts->fg_epsilon ) == NULL ) {

	   belief_list = appendBeliefList( belief_list, gFGTempBelief );
	 }

    } /* for z */
  } /* for action */

  belief_list = searchAndAddBeliefStates( belief_list, 
								  param->opts->finite_grid_points,
								  param->opts->fg_epsilon );
  
  return belief_list;

} /* genAllBeliefStates */

/**********************************************************************/
BeliefList 
genInitialDerivedBeliefStates( PomdpSolveParams param )
{
  /* Belief states are derived by starting with file-specifed initial
	belief sttae and searching from there (breadth first).
  */
  int action, obs, state, num_found;
  double prob;
  BeliefList belief_list = NULL;

  /* If file has no initial belief state, then assume the uniform
	distribution. */
  if ( gInitialBelief == NULL ) {
    for( state = 0; state < gNumStates; state++ )
	 gFGTempBelief[state] = 1.0 / gNumStates;
  
    belief_list = appendBeliefList( belief_list, gFGTempBelief );
  }
  
  else {
     belief_list = appendBeliefList( belief_list, gInitialBelief );
   
  } /* if have file initial belief */

  belief_list = searchAndAddBeliefStates( belief_list, 
								  param->opts->finite_grid_points,
								  param->opts->fg_epsilon );
  
  return belief_list;

} /* genInitialDerivedBeliefStates */

/**********************************************************************/
/* Adds the simplex corners to the finite grid of belief points.
*/
void 
FG_addSimplexCorners( )
{
  int i, j;

  for ( i = 0; i < gNumStates; i++ ) {

    for ( j = 0; j < gNumStates; j++ ) {
	 
	 if ( i == j )
	   gFGTempBelief[j] = 1.0; 
	 else
	   gFGTempBelief[j] = 0.0; 

    } /* for j */
    
    gFiniteGrid = appendBeliefList( gFiniteGrid,
							 gFGTempBelief );

  } /* for i */

} /* FG_addSimplexCorners */
/**********************************************************************/
/* Adds belief states for all combinations of equilikely being in two
   states.  Assumes that belief points all start as zeroes, so we only
   have to fill in the non-zeroes.  It adds these relative to the
   current size of the grid (so they can be added anywhere.)  
*/
void FG_addPairwise( ) 
{
  int i, j, k;

  /* These two looks generate all the beliefs we need to add. */
  for ( i = 0; i < gNumStates; i++ ) {
    for ( j = 0; j < i; j++ ) {

	 /* The inner loop here just fills in the values for this
	    particular combination opf two states. */
	 for ( k = 0; k < gNumStates; k++ ) {
	   
	   if (( k == i ) || ( k == j ))
		gFGTempBelief[k] = 0.5; 
	   else
		gFGTempBelief[k] = 0.0;
	 } /* for k */

	 gFiniteGrid = appendBeliefList( gFiniteGrid,
							   gFGTempBelief );
	 
    } /* for j */
  } /* for i */

} /* FG_addPairwise */
/**********************************************************************/
void initFiniteGrid( PomdpSolveParams param )
{
  int i;
  char belief_filename[MAX_MSG_LENGTH];

  Assert( param != NULL, "Param object is NULL." );

  gFGTempBelief = (double *) XCALLOC( gNumStates,
							  sizeof(double));

  fprintf( param->report_file, "[Finite Grid Method:]\n"  );
  fprintf( param->report_file, "    [Creating grid ... "  );
  fflush( param->report_file );

  // PBVI:
  gFiniteGrid = readBeliefList( "facet-test-00.belief", 0 );
  goto FG_END;

  switch( param->opts->finite_grid_type ) {
    
  case POMDP_SOLVE_OPTS_Fg_Type_initial:
    gFiniteGrid = genInitialDerivedBeliefStates( param );

    /*    gFiniteGrid = genAllBeliefStates(); */
    break;

  case POMDP_SOLVE_OPTS_Fg_Type_search:
    gFiniteGrid = genAllBeliefStates( param );
    break;

  case POMDP_SOLVE_OPTS_Fg_Type_pairwise:
    FG_addSimplexCorners();
    FG_addPairwise();

    break;
    
  case POMDP_SOLVE_OPTS_Fg_Type_simplex:
  default:
    FG_addSimplexCorners();
    break;
    
  } /* switch param->opts->finite_grid_type */

  // PBVI:
 FG_END:


  fprintf( param->report_file, "done.]\n"  );

  fprintf( param->report_file, 
		 "    [Grid has %d points.]\n",
		 sizeBeliefList(gFiniteGrid)  );
  
  if ( param->opts->finite_grid_save ) {

    sprintf( belief_filename, "%s.belief", param->opts->prefix_str );
    saveBeliefList( gFiniteGrid, belief_filename );
    fprintf( param->report_file, 
		   "    Grid saved to %s.\n", belief_filename );
    
  } /* if saving belief list */

} /* initFiniteGrid */
/**********************************************************************/
void 
cleanUpFiniteGrid( )
{
  
  XFREE( gFGTempBelief );

  destroyBeliefList( gFiniteGrid );
  gFiniteGrid = NULL;

} /* cleanUpFiniteGrid */
/**********************************************************************/
AlphaList 
improveFiniteGrid( AlphaList **projection, PomdpSolveParams param )
{
  AlphaList new_alpha_list;
  AlphaList alpha_node = NULL;
  BeliefList belief_list;

  Assert ( projection != NULL, "Projection sets are NULL." );
  
  new_alpha_list = newAlphaList();

  belief_list = gFiniteGrid;
  while( belief_list != NULL ) {

    /* Find the best vector at this point and add it to the list.
	  Note that this will check to make sure the vector is not
	  already in the list and return NULL if it is. */
    alpha_node = makeAlphaVector( new_alpha_list,
						    projection,
						    belief_list->b,
						    param->alpha_epsilon );

   /* We may find that this vector is already in the list. */

    belief_list = belief_list->next;
    

  } /* while belief_list */

  if ( gVerbose[V_FINITE_GRID] )
    fprintf( param->report_file, 
             "\t  FiniteGrid improve (pre-purging): %d vectors.\n",
             new_alpha_list->length );

  purgeAlphaList( new_alpha_list, param->fg_purge_option, param );

  if ( gVerbose[V_FINITE_GRID] )
    fprintf( param->report_file, 
             "\t  FiniteGrid improve (final): %d vectors.\n",
             new_alpha_list->length );
    
  return ( new_alpha_list );

} /* improveFiniteGrid */
/**********************************************************************/

