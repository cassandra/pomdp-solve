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

/*  imm-reward.c

For a sparse representation, the main thing we are trying to avoid is
keeping a matrix that is NxN where N is the number of rows and columns
(states in the MDP case).  This is easily accomplished with the
transition matrices and observation matrices.  However, the most
general form for the specification of an immediate rewards in a POMDP
conditions the reward upon the actions, current state, next state and
observation.  This requires a matrix for each combination of action
and state and the size of these matrices will be NxO where N is the
number of states and O the number of observations.  Although we can
keep the individual matrices in a sparse representation where we have
a row for each state, we will require as many of these as there are
states, effectively requiring a matrix that is at least NxN.  We
choose not to limit the file format's reward specification as
previously defined, yet want to keep a short internal repressntation
for the rewards.

Since the general rewards are used for computing the immediate rewards
for each state-action pair at the start of the program, we do not need
to be very concerned with efficiency.  We will keep the shorthand
representation around, in case individual elements are needed (e.g.,
in a simulation).  However, getting the immediate reward from this
representation will be slower than getting the other problem
parameters.

Note that for MDPs we do not have to worry about this issue.  We
cannot condition the immediate rewards upon the observation (there
is not a notion of observation in MDPs) so we merely need a sparse NxN
matrix for each action.  This will require us to keep track of the
type of problem we have loaded.

However, even for the MDP case we would like to take advantage of any 
wildcard character shortcuts, so we use this module for both the MDP 
and POMDP case.  However, things will be slightly different depending 
upon the type of problem being parsed (in gProblemType).

Here's how this module interacts with the parser: When the Parser sees
a line that begins with R: it will call the newImmReward() routine.
This will create an Imm_Reward_List node and fill in the proper
information.  If necessary, it will then initialize the intermediate
sparse matrix representation (i.e., next_state and obs not specified
for a POMDP or cur_state and next_state not specified for an MDP).
The node is not added to the list at this time.  The parser will then
deliver each value for this 'R:' entry individually through the
routine enterImmReward(). As the actual values are parsed, one of
three things could happen: It could require only a single value, it
could require a vector of values or it could be a matrix of values.
With a single value we just set it in the node.  With the vector we
set the proper entry each time a value is passed in.  Finally, if it
is an entire matrix, we enter it into the sparse representation.  When
the current 'R:' line is finished, the parser will call
doneImmReward() which will first, if necessary, transform the
intermediate sparse matrix in to a sparse matrix.  Then it will put
this into the node and add the node to the list.

Note that the semantics of the file is such that sequentially later
values override earlier valus.  This means that for a particular
combination of action, cur_state, next state and obs could have more
than one value.  The last value in the file is the one that is
correct.  Therefore we need to keep the linked list in order from
oldest to newest.  Then when a particular value is desired, we must
run through the entire list, setting the value each time we see a
specification for it.  In this way we will be left with the last value
that was specified in the file.  */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "mdp-common.h"
#include "mdp.h"
#include "sparse-matrix.h"
#include "imm-reward.h"
#include "decision-tree.h"

/* Set USE_DECISION_TREE to 1 to use the decision-tree library to speed
   up getImmediateReward() queries. */
#define USE_DECISION_TREE (1)

/* Additionally set CHECK_DECISION_TREE to 1 to run the decision-tree
   library and the original pattern matching approach in parallel, and
   check that all return values match. (Of course, running both is
   slower than running either alone.  This mode is only for validation.) */
#define CHECK_DECISION_TREE (0)

/* As we parse the file, we will encounter only one R : * : *.... line
   at a time, so we will keep the intermediate matrix as a global
   variable.  When we start to enter a line we will initial it and
   when we are finished we will convert it and store the sparse matrix
   into the node of the linked list.  */ 
I_Matrix gCurIMatrix = NULL;

/* We will have most of the information we need when we first start to
  parse the line, so we will create the node and put that information
  there.  After we have read all of the values, we will put it into
  the linked list.  */
Imm_Reward_List gCurImmRewardNode = NULL;

/* This is the actual list of immediate reward lines */
Imm_Reward_List gImmRewardList = NULL;

/**********************************************************************/
void 
destroyImmRewards() {
  Imm_Reward_List temp;

  while( gImmRewardList != NULL ) {

    temp = gImmRewardList;
    gImmRewardList = gImmRewardList->next;

    switch( temp->type ) {
    case ir_vector:
      XFREE( temp->rep.vector );
      break;

    case ir_matrix:
      destroyMatrix( temp->rep.matrix );
      break;

    case ir_value:
    default:
      break;
    }  /* switch */

    XFREE( temp );

  }  /* while */

#if USE_DECISION_TREE
  dtDeallocate();
#endif

}  /* destroyImmRewardList */
/**********************************************************************/
Imm_Reward_List 
appendImmRewardList( Imm_Reward_List list, Imm_Reward_List node ) {
  Imm_Reward_List temp = list;

  if( temp == NULL )
    return( node );

  while( temp->next != NULL ) 
    temp = temp->next;

  temp->next = node;

  return( list );

}  /* appendImmRewardList */
/**********************************************************************/
void 
newImmReward( int action, int cur_state, int next_state, int obs ) {
  
  /* First we will allocate a new node for this entry */
  gCurImmRewardNode = (Imm_Reward_List) XMALLOC( sizeof(*gCurImmRewardNode ));
  
  gCurImmRewardNode->action = action;
  gCurImmRewardNode->cur_state = cur_state;
  gCurImmRewardNode->next_state = next_state;
  gCurImmRewardNode->obs = obs;
  gCurImmRewardNode->next = NULL;

  switch( gProblemType ) {

  case POMDP_problem_type:
    if( obs == NOT_PRESENT) {
      
      if( next_state == NOT_PRESENT ) {
       
	/* This is the situation where we will need to keep a sparse 
	   matrix, so let us initialize the global I_Matrix variable */
	
       gCurIMatrix = newIMatrix( gNumStates );
       gCurImmRewardNode->rep.matrix = NULL;
       gCurImmRewardNode->type = ir_matrix;
       
     } /* next_state == NOT_PRESENT */
      
      else { /* we will need a vector of numbers, not a matrix */
	
	gCurImmRewardNode->rep.vector = (double *) XCALLOC( gNumObservations,
							  sizeof(double));
	gCurImmRewardNode->type = ir_vector;
	
      }  /* else need vector, not matrix */
      
    }  /* obs == NOT_PRESENT */
    
    else {  /* We only need a single value, so let us just initialize it */
      /* to zero */
      
      gCurImmRewardNode->rep.value = 0.0;
      gCurImmRewardNode->type = ir_value;
    }
    break;

  case MDP_problem_type:
    /* for this case we completely ignor 'obs' parameters */
      
    if( next_state == NOT_PRESENT ) {
       
      if( cur_state == NOT_PRESENT ) {
	/* This is the situation where we will need to keep a sparse 
	   matrix, so let us initialize the global I_Matrix variable.
	   */
	
	gCurIMatrix = newIMatrix( gNumStates );
	gCurImmRewardNode->rep.matrix = NULL;
	gCurImmRewardNode->type = ir_matrix;
	
      } /* cur_state == NOT_PRESENT */
      
      else { /* we will need a vector of numbers, not a matrix */
	
	gCurImmRewardNode->rep.vector = (double *) XCALLOC( gNumStates,
							  sizeof(double));
	gCurImmRewardNode->type = ir_vector;
	
      }  /* else need vector, not matrix */
      
    }  /* next_state == NOT_PRESENT */
    
    else {  /* We only need a single value, so let us just initialize it */
      /* to zero */
      
      gCurImmRewardNode->rep.value = 0.0;
      gCurImmRewardNode->type = ir_value;
    }
    break;
    
  default:
    fprintf( stderr, "**ERR** newImmReward: Unreckognised problem type.\n");
    exit( -1 );
    break;

  }  /* switch */

}  /* newImmReward */
/**********************************************************************/
void 
enterImmReward( int cur_state, int next_state, int obs, 
		double value ) {

/* cur_state is ignored for a POMDP, and obs is ignored for an MDP */

  assert( gCurImmRewardNode != NULL );

  switch( gCurImmRewardNode->type ) {
  case ir_value:
    gCurImmRewardNode->rep.value = value;
    break;

  case ir_vector:
    if( gProblemType == POMDP_problem_type )
      gCurImmRewardNode->rep.vector[obs] = value;
    else
      gCurImmRewardNode->rep.vector[next_state] = value;
    break;

  case ir_matrix:
    if( gProblemType == POMDP_problem_type )
      addEntryToIMatrix( gCurIMatrix, next_state, obs, value );
    else
      addEntryToIMatrix( gCurIMatrix, cur_state, next_state, value );
    break;

  default:
    fprintf( stderr, "** ERR ** Unreckognized IR_Type in enterImmReward().\n");
    exit( -1 );
    break;
  }  /* switch */

}  /* enterImmReward */
/**********************************************************************/
void irAddToDecisionTree(Imm_Reward_List node)
{
  int i, j, k;
  Matrix m;

  assert( node != NULL );

  /* ensure the decision tree is initialized (ok to call dtInit() more than once) */
  dtInit(gNumActions, gNumStates, gNumObservations);

  switch( node->type ) {
  case ir_value:
    if ( gProblemType == POMDP_problem_type ) { /* pomdp */
      dtAdd(node->action, node->cur_state, node->next_state, node->obs, node->rep.value);
    } else { /* mdp */
      dtAdd(node->action, node->cur_state, node->next_state, WILDCARD_SPEC, node->rep.value);
    }
    break;
    
  case ir_vector:
    if ( gProblemType == POMDP_problem_type ) { /* pomdp */
      for (i=0; i < gNumObservations; i++) {
	dtAdd(node->action, node->cur_state, node->next_state, i, node->rep.vector[i]);
      }
    } else { /* mdp */
      for (i=0; i < gNumStates; i++) {
	dtAdd(node->action, node->cur_state, i, WILDCARD_SPEC, node->rep.vector[i]);
      }
    }
    break;
    
  case ir_matrix:
    m = node->rep.matrix;
    for (i=0; i < m->num_rows; i++) {
      for (j=0; j < m->row_length[i]; j++) {
	k = m->row_start[i] + j;
	if( gProblemType == POMDP_problem_type )  { /* pomdp */
	  dtAdd(node->action, node->cur_state, i, m->col[k], m->mat_val[k]);
	} else { /* mdp */
	  dtAdd(node->action, i, m->col[k], WILDCARD_SPEC, m->mat_val[k]);
	}
      }
    }
    break;
    
  default:
    assert(0 /* never reach this point */);
  }  /* switch */
}
/**********************************************************************/
void 
doneImmReward() {
  
  if( gCurImmRewardNode == NULL )
    return;

  switch( gCurImmRewardNode->type ) {
  case ir_value:
  case ir_vector:
    /* Do nothing for these cases */
    break;
    
  case ir_matrix:
    gCurImmRewardNode->rep.matrix = transformIMatrix( gCurIMatrix );
    destroyIMatrix( gCurIMatrix );
    gCurIMatrix = NULL;
    break;

  default:
    fprintf( stderr, "** ERR ** Unreckognized IR_Type in doneImmReward().\n");
    exit( -1 );
    break;
  }  /* switch */

#if USE_DECISION_TREE
  irAddToDecisionTree(gCurImmRewardNode);
#endif

  gImmRewardList = appendImmRewardList( gImmRewardList,
				       gCurImmRewardNode );
  gCurImmRewardNode = NULL;

}  /* doneImmReward */
/**********************************************************************/
double 
getImmediateReward( int action, int cur_state, int next_state,
		    int obs ) {
#if USE_DECISION_TREE
  double dt_return_value;
#endif
#if !USE_DECISION_TREE || CHECK_DECISION_TREE
  Imm_Reward_List temp = gImmRewardList;
  double return_value = 0.0;

  assert(( action >= 0) && (action < gNumActions)
	 && (cur_state >= 0) && (cur_state < gNumStates)
	 && (next_state >= 0) && (next_state < gNumStates));

  while( temp != NULL ) {
    
    if((( temp->action == WILDCARD_SPEC )
	|| ( temp->action == action ))) {

      switch( temp->type ) {
      case ir_value:

	if( gProblemType == POMDP_problem_type ) {
	  if((( temp->next_state == WILDCARD_SPEC )
	      || ( temp->next_state == next_state))
	     && ((temp->obs == WILDCARD_SPEC)
		 || (temp->obs == obs ))
	     && ((temp->cur_state == WILDCARD_SPEC)
		 || (temp->cur_state == cur_state ))) {

	    
	    return_value = temp->rep.value;
	    
	  }  /* if we have a match */
	}  /* if POMDP */

	else {  /* then it is an MDP */
	  if((( temp->cur_state == WILDCARD_SPEC )
	      || ( temp->cur_state == cur_state))
	     && ((temp->next_state == WILDCARD_SPEC)
		 || (temp->next_state == next_state ))) {
	    
	    return_value = temp->rep.value;
	    
	  }  /* if we have a match */
	}
	     break;
    
      case ir_vector:

	if( gProblemType == POMDP_problem_type ) {
	  if((( temp->next_state == WILDCARD_SPEC )
	      || ( temp->next_state == next_state))
	     && ((temp->cur_state == WILDCARD_SPEC)
		 || (temp->cur_state == cur_state ))) {
	    
	    return_value = temp->rep.vector[obs];
	  }
	}  /* if POMDP */

	else {  /* it is an MDP */
	  if(( temp->cur_state == WILDCARD_SPEC )
	     || ( temp->cur_state == cur_state)) {
	    
	    return_value = temp->rep.vector[next_state];
	  }
	}

	break;
    
      case ir_matrix:
	if( gProblemType == POMDP_problem_type )  {
	  if(( temp->cur_state == WILDCARD_SPEC )
	     || (temp->cur_state == cur_state ))
	    return_value = getEntryMatrix( temp->rep.matrix, next_state,
					obs );
	}
	else
	  return_value = getEntryMatrix( temp->rep.matrix, cur_state,
					next_state );

	break;

      default:
	fprintf( stderr, 
		"** ERR ** Unreckognized IR_Type in getImmediateReward().\n");
	exit( -1 );
	break;
      }  /* switch */

    
    }  /* If we have a partially matching node */

    temp = temp->next;
  }  /* while */
#endif /* if !USE_DECISION_TREE || CHECK_DECISION_TREE */


#if USE_DECISION_TREE
  dt_return_value = dtGet(action, cur_state, next_state, obs);
#if CHECK_DECISION_TREE
  if (return_value != dt_return_value) {
    fprintf(stderr,
	    "ERROR: getImmediateReward: decision-tree value and pattern match values disagree\n"
	    "  action=%d cur_state=%d next_state=%d obs=%d)\n"
	    "  decision-tree value=%g pattern match value=%g difference=%g\n",
	    action, cur_state, next_state, obs,
	    dt_return_value, return_value, fabs(dt_return_value - return_value));
    exit(EXIT_FAILURE);
  }
#endif
  return dt_return_value;
#else /* if USE_DECISION_TREE / else */
  return( return_value );
#endif /* if USE_DECISION_TREE / else */
  
}  /* getImmediateReward */
/**********************************************************************/


