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

#ifndef POMDP_H
#define POMDP_H

#include "global.h"
#include "mdp/mdp.h"

/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

extern int **gObservationPossible;
extern int *gNumPossibleObservations;
extern int gRequireNonNegativeRewards;

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

/* Often we would like to do some max or min procedure and require
   initialization to the most extreme value.  Since the extreme value
   depends on whether or not we are using rewards or costs, we have
   encapsulated this in this routine.  */
extern double worstPossibleValue();

/* Often we would like to do some max or min procedure and require
   initialization to the most extreme value.  Since the extreme value
   depends on whether or not we are using rewards or costs, we have
   encapsulated this in this routine.  */
extern double bestPossibleValue();

/* Does the necessary things to read in and set-up a POMDP file.  Also
 precomputes which observations are possible and which are not.  */
extern void initializePomdp( char *filename, 
                             double obs_possible_epsilon );
    
/* Deallocates the POMDP read in by initializePomdp().  */
extern void cleanUpPomdp(  );

/* Although the getEntryMatrix() routine is normally used to extract
  matrix entries, we provide this routine for the immediate reward
  (utilities) matrix for two purposes: First, the pomdp-solve code
  itself no longer deals with with utilities that are specified in
  terms of costs.  This routine will mask this fact by multiplying all
  immediate rewards by -1.  Second, it is often desirable to have
  non-negative rewards. Any problem can be converted to one of these
  by adding the appropriate offset.  The routine can do this as well.
  Note that the actual value functions will be skewed and require some
  form of rescaling to make sense.

  To force only non-negative rewards, set the global flag
  gRequireNonNegativeRewards before calling this routine. 

  If you want cost utilities and do not mind negative values,
  the you should access the immediate rewards directly with:

     getEntryMatrix( Q, a, state ) */
extern double getAdjustedReward( int action, int state );

/* Returns a boolean value as to whether or not the immediate rewards
  have to be scaled (and the value functions that are calculated as
  well.)  This adjustment is either due to a non-negativity constraint
  or a COST utility which was converted to REWARD.  */
extern int valuesRequireScaling(  );

/* Because we use the getAdjustedReward() routine to access the
   rewards, the value function at a given time might not be for the
   underlying POMDP since the values are skewed.  To bring the values
   back to the solution to the POMDP requires undoing the scaling that
   getAdjustedReward() did.  This scaling is based upon the number of
   value function updates, since this is how many times the adjusted
   reward was incorporated into the values.  To scale, you should add
   the value returned.  */
extern double getValueScaleFactor( int num_updates );

/* As per the getImmediateReward() rotuine, there are two manners in
  which the POMDP immediate rewards could have been adjusted.  First
  they could have been adjusted to convert COSTS to REWARDS.  Next
  they might have been adjusted to be non-negative.  Because of these
  two conversions, when you get a value function, it is not really the
  value function of the underlying POMDP.  However, we can convert it
  to an equivalent value function by re-adjusting the rewards. Because
  there can be discounting involved, and rewards are accumulated, how
  much the values need to be adjusted depend upon how many value
  iteration epochs have passed, which is why you need to specify this.  */
extern double scaleValue( double value, int num_updates );
   
#endif
