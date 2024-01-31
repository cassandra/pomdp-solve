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

#ifndef ZLZ_SPEEDUP_H
#define ZLZ_SPEEDUP_H

/**********************************************************************/
/********************   DEFAULT VALUES       **************************/
/**********************************************************************/

/* This is the value cited in the ppaer and used as part of the
   stopping criteria for the improve() routine. */
#define DEFAULT_EPSILON1_VALUE             0.001

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

/* This is the routine that implements the portion of the VI1
  algorithm *after* the regular DP update.  The U_n set is the last
  computed value funciton (output of the DP update).  The 'delta'
  parametre is the current Bellman residual of the new value function
  and the previous value funciton as computed by the bellmanError()
  routine.  */
extern void ZLZ_ViSpeedup( AlphaList U_n,
                           double delta,
                           PomdpSolveParams param );

#endif
