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

#ifndef CROSS_SUM_H
#define CROSS_SUM_H

/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

/**********************************************************************/
/********************   DEFAULT VALUES       **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

/* Takes the cross sum of two sets of vectors and returns the
  resulting set. If either A or B is null, then NULL is returned.  If
  either list is empty, then an empty list is returned. The
  save_obs_sources argument deterines whther we do the bookkeeping
  required to develop a policy graph or not.  */
extern AlphaList crossSum( AlphaList A, 
                           AlphaList B, 
                           int save_obs_sources );
 
#endif
