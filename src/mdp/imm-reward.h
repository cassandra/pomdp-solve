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

#ifndef MDP_IMM_REWARD_H
#define MDP_IMM_REWARD_H 1

#include "sparse-matrix.h"

/*
   We will represent the general immediate reward structure as a
linked list, where each node of the list will correspond to a single
R: * : ... entry.  The entry from the file could specify a single
value, a row of values, or an entire matrix.  Thus we need three
different representations depending on the situation.  Additionally,
all of the components could have a wildcard character indicating 
that it is a specification for a family of values.  This is indicated
with special characters.

*/

/* Each of the action, states and obs could have a state index number,
  or one of these two values.  Since states cannot be negative we use
  negative values for the special characters.  The observation cannot
  be present when the next_state is present, but this should be
  enforced by the parser.  When both the next state and obs are not
  present, we will use a sparse matrix representation.  When only the
  obs is not present we will use a single dimensional, non-sparse
  matrix.  When both are specified we use a single value.  Note that
  it does not matter if the indivdual elements are specific indices or
  a wildcard, either way we will store a single value.

*/

#define WILDCARD_SPEC                -1
#define NOT_PRESENT                  -99

/* This allows us to easily check what type of entry it is, since */
/* there are three possibilities. */
typedef enum { ir_none, ir_value, ir_vector, ir_matrix } IR_Type;

typedef struct Imm_Reward_Node_Struct *Imm_Reward_List;
struct Imm_Reward_Node_Struct {

  IR_Type type;

  int action;
  int cur_state;
  int next_state;
  int obs;

  union rep_tag {
     double value;
     double *vector;
     Matrix matrix;
  } rep;

  Imm_Reward_List next;
};

extern void destroyImmRewards();
extern void newImmReward( int action, int cur_state, int next_state, int obs );
extern void enterImmReward( int cur_state, int next_state, int obs, 
			   double value );
extern void doneImmReward();
extern double getImmediateReward( int action, int cur_state, int
				 next_state, int obs );
				 
#endif
