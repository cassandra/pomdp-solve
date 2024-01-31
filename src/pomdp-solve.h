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

#ifndef POMDP_SOLVE_H
#define POMDP_SOLVE_H

/*******************************************************************/
/**************             CONSTANTS               ****************/
/*******************************************************************/

/*******************************************************************/
/**************             TYPEDEFS                ****************/
/*******************************************************************/

/*******************************************************************/
/**************       EXTERNAL VARIABLES            ****************/
/*******************************************************************/

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

extern void showUsagePomdpSolve( FILE *file );

extern void initPomdpSolve( PomdpSolveParams param );

extern void cleanUpPomdpSolve( PomdpSolveParams param );

extern void solvePomdp( PomdpSolveParams param );

/* For now our default policy is just all zeroes.  */
extern AlphaList getDefaultInitialPolicy( );

/* Some algorithms will solve one iteration of POMDP value iteration
   by breaking the problem into a separate one for each action.  This
   routine will implement the basic structure needed and call the
   appropriate routines depending on the specific algorithm being
   used.  Current algorithms that do it this way: TwoPass, Witness and
   IncrementalPruning */
extern AlphaList improveByQ( AlphaList **projection,
                             PomdpSolveParams param );
/* This does a single DP step of value iteration for a POMDP.  It
   takes in the previous value function and parameters for solving and
   returns the next or improved solution.  */
extern AlphaList improveV( AlphaList prev_alpha_list,
                           PomdpSolveParams param );


#endif
