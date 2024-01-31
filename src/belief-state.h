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

/*
 *  Representations and methods for a grid of belief points.
 */

#ifndef BELIEF_GRID_H
#define BELIEF_GRID_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "double-vector.h"

/*******************************************************************/
/**************      USEFUL MNENOMIC CONSTANTS      ****************/
/*******************************************************************/

/*******************************************************************/
/**************             TYPEDEFS                ****************/
/*******************************************************************/

/* Sometimes we just need a belief state, which we represent simply
   as a DoubleVector. */  
typedef DoubleVector BeliefState;

typedef DoubleVectorList BeliefStateList;

/* A belief grid is a wrapper around the double vector list structure
   that adds its own attributes. */

typedef DoubleVectorList BeliefGrid;

/* The attributes for individual belief points (nodes in the list) */
typedef struct BeliefGridNodeAttrStruct*  BeliefGridNodeAttr;
struct BeliefGridNodeAttrStruct 
{
  DoubleVectorList facets;

};

/* The attributes for the lists themselves. */

typedef struct BeliefGridAttrStruct*  BeliefGridAttr;
struct BeliefGridAttrStruct
{
  int id;
};


/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

extern BeliefGrid BS_newGrid( );
extern void BS_destroyGrid( BeliefGrid bg );

#endif
