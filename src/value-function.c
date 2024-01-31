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
 *  Representing value functions (was alpha.[ch]).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mdp/mdp.h>

#include "global.h"
#include "value-function.h"

/**********************************************************************/
ValueFunction 
VF_new(  ) 
{
  /* Allocates memory for a new BeliefGrid which will be initially
	empty. */
  FacetList f;
  ValueFunction vf;
  ValueFunctionAttr attr;

  attr = (ValueFunctionAttr) XMALLOC( sizeof( *attr ));

  attr->obs_source = NULL;
  attr->obs = INVALID_OBS;
  attr->prev_source = NULL;

  vf = DV_newList( attr );

  return vf;

}  /* VF_new */

/**********************************************************************/
void 
VF_destroy( ValueFunction vf ) 
{
  /* Deallocates memory for a BeliefGrid. The grid cannot be NULL. */

  DoubleVectorNode node;

  Assert( vf != NULL, "Cannot destroy NULL value function." );
    
  /* First we need to free the attributes for each node (facet) in the 
	value function. */ 
  node = vf->head;
  while( node != NULL )
    {
	 if ( node->attr != NULL )
	   {

		/* FIXME: Decide if these need to also be deallocated here 

		   node->attr->witness = NULL;
		   node->attr->belief_list = NULL;
		*/

		XFREE( node->attr );
		node->attr = NULL;
	   }

	 node = node->next;
    } /* while node != NULL */

  if ( vf->attr != NULL )
    {

	 /* FIXME: Decide if these need to also be deallocated here 

	    vf->attr->obs_source = NULL;
	    vf->attr->prev_source = NULL;
	 */

	 XFREE( vf->attr );
	 vf->attr = NULL;
    }

  DV_destroyList( vf );




}  /* VF_destroy */
