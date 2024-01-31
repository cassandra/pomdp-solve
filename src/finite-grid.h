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

#ifndef FINITE_GRID_H
#define FINITE_GRID_H

#include "params.h"

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

extern double FG_computeError( AlphaList prev_list, 
						 AlphaList cur_list,
						 PomdpSolveParams param );

extern void initFiniteGrid( PomdpSolveParams param );
extern void cleanUpFiniteGrid( );

#ifdef ORIGINAL_FINITE_GRID
extern AlphaList improveFiniteGrid( AlphaList *projection, 
					    PomdpSolveParams param );
#else
extern AlphaList improveFiniteGrid( AlphaList **projection, 
							 PomdpSolveParams param );
#endif


#endif
