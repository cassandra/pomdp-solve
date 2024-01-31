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

#ifndef NEIGHBOR_H
#define NEIGHBOR_H

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

/* Adds a copy of the given neighbor to the list, but only if: 1)
   The neighbor isn't already in the list.  2) The neigbhbor isn't
   dominated by an existing neighbor.  Returns TRUE if copy of
   neighbor is added and false otherwise.  Other params: 'source' is
   the vector that generated this neighbor.  'z' is the observation
   for which this neighbor differs from the source.  'proj_z' is the
   vector that is different.  */
extern int addNeighbor( AlphaList list, 
                        double *neighbor,
                        AlphaList source, 
                        int z, 
                        AlphaList proj_z,
                        int domination_check,
                        double epsilon );

/* Adds all neighbors of the vector in the node into the list.  */
extern int addAllNeighbors( AlphaList list, 
                            AlphaList node, 
                            AlphaList *projection,
                            int domination_check,
                            double epsilon );

/* Adds all the "neighbors" of all the vectors in the list sent in
  into the list. Checks to make sure the neighbor isn't already
  in the list.  Returns the number of neighbors added.  */
extern int addAllNeighborsFromList( AlphaList dest_list, 
                                    AlphaList list,
                                    AlphaList *projection,
                                    int domination_check,
                                    double epsilon );
       
#endif
