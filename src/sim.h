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

#ifndef SIM_H
#define SIM_H

#include "alpha.h"

/*******************************************************************/
/**************             TYPEDEFS                ****************/
/*******************************************************************/

typedef struct TrajectoryStruct *Trajectory;
struct TrajectoryStruct {

  double *belief;

  int action;
  int obs;

  Trajectory next;

};

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

extern void freeTrajectory( Trajectory trajectory );

extern Trajectory newTrajectory( int length );

extern void generateTrajectory( Trajectory trajectory, 
                                double *init_b,
                                AlphaList for_policy );

#endif
