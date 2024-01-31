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

#ifndef RANDOM_H
#define RANDOM_H

/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

/* How big the array of short int's should be for the random seed */
#define SEED_SIZE                  3

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

/* Seeds the psuedo-random number generated if it has not already been
  seeded.  */
extern void randomize();

/* Returns a uniform psuedo-random number between 0 and 1 */
extern double fran();

/* Returns the current random seed.  Useful if you want to reproduce 
   the sequence (e.g., debugging) */
extern void getRandomSeed( unsigned short seed[SEED_SIZE] );

/* Allows you to reproduce a psuedo-random sequence by explicitly 
   setting the seed value for the random number generator */
extern void setRandomSeed( unsigned short seed[SEED_SIZE] );

/* Sets seed from a string specification. */
extern void setRandomSeedFromString ( char *str );

/* Displays the current random seed to file stream. */
extern void displayRandomSeed( FILE *file );

/* Displays the current random seed on stdout. */
extern void showRandomSeed( );

/* Returns a uniform psuedo-random number between min and max in the 
 form of a double precision number */
extern double getRandomDouble( double min, 
                               double max );

/* Returns a uniform psuedo-random number between min and max in the 
 form of an integer */
extern int getRandomInt( int min, 
                         int max );

/* This routine sets the discrete probability distribution so that
   each distribution is equally likely.  */
extern void setRandomDistribution( double *x, 
                                   int num_probs );

/* Sets the given vector of doubles with random values betwen the min
  and max values given.  'num' is the number of elements in the
  vector.  */
extern void setRandomDoubleVector( double *vect, 
                                   int num, 
                                   double min, 
                                   double max );
 
#endif


