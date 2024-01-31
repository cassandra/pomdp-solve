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

#ifndef PROJECTION_H
#define PROJECTION_H

/*******************************************************************/
/**************       EXTERNAL VARIABLES            ****************/
/*******************************************************************/

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

/* Just creates and returns the storage to hold all the projection
  sets.  They will initially all be NULL.  */
extern AlphaList **allocateAllProjections(  );

/* Makes all the projected alpha vector lists, which amounts to a
   projected list for each action-observation pair. Stores this as a
   two dimensional array of lists where the first index is the action
   and the other is the observation.
   
   The 'impossible_obs_epsilon' specifies the tolerance to use when
   trying to determine whether or not a particulat observation is at
   all feabile.
*/
extern void setAllProjections( AlphaList **projection,
						 AlphaList prev_alpha_list );

/* Just removes the memory for the list themselves and not for the
  2D array which is holding them.
*/
extern void clearAllProjections( AlphaList **projection );
 
/* Discards all the projection lists and memory associated with them.  */
extern void freeAllProjections( AlphaList **projection );
 
/* Makes all the projected alpha vector lists, which amounts to a
   projected list for each action-observation pair. Stores this as a
   two dimensional array of lists where the first index is the action
   and the other is the observation.  This allocates the space for the
   projections first.  The 'impossible_obs_epsilon' specifies the
   tolerance to use when trying to determine whether or not a
   particulat observation is at all feabile.  */
extern AlphaList **makeAllProjections( AlphaList prev_alpha_list );

/* Displays all projections to file stream.  */
extern void displayProjections( FILE *file, AlphaList **projection );

/* Displays all projections to stdout.  */
extern void showProjections( AlphaList **projection );
        
#endif
