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

#ifndef INC_PRUNE_H
#define INC_PRUNE_H

#include "params.h"

/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

/* We will use the 'length' field of an AlphaList as a counter.  To
   make this explicit, this macros is used. */
#define COUNT(X)    ((X)->length)

/* Use the 'mark' field of the header node of a list to indicate
   whether or not it will need to be destroyed later. The use of this
   is made explicit with this macro. */
#define SHOULD_DESTROY(X) ((X)->mark)

/**********************************************************************/
/********************   DEFAULT VALUES       **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

extern void initIncPrune( );

extern void cleanUpIncPrune(  );
  
/* The main incremental pruning algorithm routine for finding the
  Q-function represention for value iteration with POMDPs.  */
extern AlphaList improveIncPrune( AlphaList *projection,
                                  PomdpSolveParams param );
  
#endif
