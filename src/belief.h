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

#ifndef BELIEF_H
#define BELIEF_H

typedef struct BeliefListStruct *BeliefList;
struct BeliefListStruct {

  double *b;

  int mark;  /* can be used to mark elements of the list as used or
                unused. */

  BeliefList next;
};


/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

/* How many decimal places to show when write grid to file/screen */
#define BELIEF_FILE_DECIMAL_PRECISION          25

/**********************************************************************/
/********************   DEFAULT VALUES       **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

extern double *newBelief( );
extern double *duplicateBelief( double *b );
extern void copyBelief( double *dest, double *src );
extern void destroyBelief( double *b );
extern int sameBelief( double *b1, double *b2, double epsilon );
extern BeliefList readBeliefList( char *filename, int max_beliefs ); 
extern void writeBeliefList( FILE *file, BeliefList belief_list );
extern void saveBeliefList( BeliefList list, char *filename );
extern void showBeliefList( BeliefList list );
extern BeliefList readBeliefList( char *filename, int max );
extern BeliefList newBeliefNode( double *b );
extern void destroyBeliefNode( BeliefList temp );
extern BeliefList prependBeliefList( BeliefList list,
							  double *b );
extern BeliefList appendBeliefList( BeliefList list, double *b );
extern void destroyBeliefList( BeliefList list );
extern int sizeBeliefList( BeliefList list );
extern BeliefList findBeliefState( BeliefList list, 
							double *b,
							double epsilon );

  
#endif
