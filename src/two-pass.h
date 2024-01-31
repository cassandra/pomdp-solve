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

#ifndef TWO_PASS_H
#define TWO_PASS_H

#include "params.h"

/*******************************************************************/
/**************      USEFUL MNENOMIC CONSTANTS      ****************/
/*******************************************************************/

/* These are the command line arguments that are valid.  To add one
   you need to increment NUM_WITNESS_ARGS, add a line to the
   WITNESS_ARGS list of strings. and finally add a constant of the
   form CMD_ARG_XXX with the new string. */
#define NUM_TWO_PASS_ARGS       0

#define UNMARKED              0
#define MARKED                1

/*******************************************************************/
/**************       EXTERNAL VARIABLES            ****************/
/*******************************************************************/


/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

extern void initTwoPass( );

extern void cleanUpTwoPass( );
  
/* The main two-pass algorithm routine for finding the Q-function
  represention for value iteration with POMDPs.  */
extern AlphaList improveTwoPass( AlphaList *projection,
                                 PomdpSolveParams param );
 
#endif


