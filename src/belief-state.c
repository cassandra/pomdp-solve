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
 *  Representing a grid of belief points.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "belief-state.h"

/**********************************************************************/
BeliefGrid
BS_newGrid(  ) 
{
  /* Allocates memory for a new BeliefGrid which will be initially
	empty. */

  BeliefGrid grid;
  BeliefGridAttr attr;

  attr = (BeliefGridAttr ) XMALLOC( sizeof( *attr ));

  /* Always zero for now */
  attr->id = 0;

  grid = DV_newList( attr );

  return grid;

}  /* BG_newGrid */

/**********************************************************************/
void
BS_destroyGrid( BeliefGrid grid ) 
{
  /* Deallocates memory for a BeliefGrid. The grid cannot be NULL. */

  DoubleVectorNode node;

  Assert( grid != NULL, "Cannot destroy NULL belief grid." );
    
  /* First we need to free the attributes for each node in the belief
	grid. */ 
  node = grid->head;
  while( node != NULL )
    {
	 if ( node->attr != NULL )
	   {
		XFREE( node->attr );
		node->attr = NULL;
	   }

	 node = node->next;
    } /* while node != NULL */

  if ( grid->attr != NULL )
    {
	 XFREE( grid->attr );
	 grid->attr = NULL;
    }

  DV_destroyList( grid );

}  /* BG_destroyGrid */
