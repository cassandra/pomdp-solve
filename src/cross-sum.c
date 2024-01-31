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
 *  Routines for taking the cross-sum of two vector sets.
 * 
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "mdp/mdp.h"

#include "global.h"
#include "pomdp.h"
#include "alpha.h"
#include "cross-sum.h"

/**********************************************************************/
void 
setAlphaListSources( AlphaList new_node, 
		     AlphaList first_node, 
		     AlphaList second_node ) 
{
  /*
    Sets the first_source, second_source and obs_source fields of the
    new_node to be consistent with the fact that new_node was
    constructed from the sum of first_node and second_node, where those
    two nodes are either vectors in a projection set or a partial
    cross-sum. 
  */
  int z;

  /* Do nothing unless all arguments are valid. Note that part of our
     assumptions is that new_node was just created from first and
     second nodes, so it better not already have its obs_sources
     set. */
  Assert ( new_node != NULL
           && first_node != NULL
           && second_node != NULL,
          "Bad (NULL) parameter(s)." );

  /* We now want to indicate exactly how this vector was
     created. There are two uses for this: 1) In the generalize
     incremental pruning, we will want to know which vectors were
     used to create new vectors.  2) When developing the policy
     graph, we will want to know which previous iteration vectors
     this was formed from, since it defines the decision tree
     which can lead to a policy graph. 
     
     These two uses are reflected by setting two different sets of
     fields in the AlphaList node. */
  
  /* To maintain the two vectors immediately responsible for this
     vector, we simply set the two fields. below. */
  new_node->first_source = first_node;
  new_node->second_source = second_node;
  
  /* We assume that the absence of an obs_source array pointer in the
     node means that we just are not concerned with setting its
     values. */
  if ( new_node->obs_source == NULL )
    return;

  /* The next thing is a little bit tricky because for the sets A and
     B, they may either be a projection set, or they may be a partial
     cross-sum.  Keeping track of the overall sources is a little
     tricky, since we may have a partial cross-sum which would need
     its obs_source array copied over to the new partial cross-sum's
     array.  Here are the cases:

       projection + projection

       projection + partial cross-sum
         
       partial cross-sum + partial cross-sum

     Because this array is built up during a cross-sum, we use the
     fact that some are NULL pointers to indicate that they have not
     yet had their values set. Thus, we will copy both the first and
     second node's obs_source arrays, but only the non-NULL elements.
     We assume that it is a mistake for both vectors to have had their
     obs_source pointer set for the same observation.
     
     Note that we can differentiate a projection set from a partial
     cross-sum by the absence or presence of the obs_source array.  */

  /* We need both the obs_source array to exist and for it to not be
     pointer to NULL for us to copy it. */
  if ( first_node->obs_source != NULL ) {
    for ( z = 0; z < gNumObservations; z++ )
      if ( first_node->obs_source[z] != NULL )
        new_node->obs_source[z] = first_node->obs_source[z];
  } /* if have first node obs_source array */
  
  /* Otherwise it is a projection set, so set its observation's source
     pointer to it. */
  else
    new_node->obs_source[first_node->obs] = first_node;
  
  /* Repeat the above, but now for the second node. */
  if ( second_node->obs_source != NULL ) {
    for ( z = 0; z < gNumObservations; z++ )
      if ( second_node->obs_source[z] != NULL )
        new_node->obs_source[z] = second_node->obs_source[z];
  } /* if have second node obs_source array */
  else
    new_node->obs_source[second_node->obs] = second_node;
  
}  /* setAlphaListSources */
/**********************************************************************/
AlphaList 
crossSum( AlphaList A, AlphaList B, int save_obs_sources ) 
{
  /*
    Takes the cross sum of two sets of vectors and returns the resulting
    set. If either A or B is null, then NULL is returned.  If either
    list is empty, then an empty list is returned.  The save_obs_sources
    argument deterines whther we do the bookkeeping required to develop
    a policy graph or not. 
  */
  AlphaList V, a, b, temp;
  double *alpha;
  int i;
  
  Assert ( A != NULL &&  B != NULL,
           "NULL set(s) specified." );

  /* Allocate memory for new list and initialize it. */
  V = newAlphaList();

  /* We should never be cross-summing two sets that have different
     actions, so selecting either lists' action should be equally as
     valid. */
  V->action = A->action;

  /* Just a doubly nested loop over both sets. */
  a = A->head;
  while ( a != NULL ) {

    b = B->head;
    while ( b != NULL ) {

      /* This is where all the action is: */
      alpha = newAlpha();
      for ( i = 0; i < gNumStates; i++ )
        alpha[i] = a->alpha[i] + b->alpha[i];

      /* By creating this node with an obs_source array, we are
         indicating that we want the array set.  This is done just
         below in the setAlphaListSources routine. */
      if ( save_obs_sources )
        temp = newAlphaNodeObsSource( alpha, V->action );
      else
        temp = newAlphaNode( alpha, V->action );
     
      appendNodeToAlphaList( V, temp );

      /* Set the source pointers to the right things. */
      setAlphaListSources( temp, a, b );

      b = b->next;
    }  /* while b */

    a = a->next;
  }  /* while a */

  return ( V );
}  /* crossSum */
/**********************************************************************/


