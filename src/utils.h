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

#ifndef UTILS_H
#define UTILS_H

/* Rather than worry about dynamically alocating arrays for tracking
   ties in best comparisons, we will make an assumption that there are
   at most a fixed number of ties.  This variable sets this amount and
   the code that uses it should check to see if this is exceeding and
   spit out a warning/error message if it has so that the user can
   increase this value for their problem.
*/
#define MAX_BEST_COUNT   100

extern void UTIL_mapBeliefList( char *belief_filename,
						  char *alpha_filename,
						  double epsilon,
						  char *map_filename );
  
extern void UTIL_sortAlphaFile( char *in_alpha_filename,
						  char *out_alpha_filename );

extern void UTIL_purgeAlphaFile( char *in_alpha_filename,
						   char *out_alpha_filename,
						   PomdpSolveParams param );

extern void UTIL_compareAlphaFiles( char *alpha1_filename,
							 char *alpha2_filename,
							 double epsilon,
							 char *out_filename );

extern void UTIL_relinkPolicyGraph( char *alpha_filename,
							 char *pg_filename,
							 char *prev_alpha_filename,
							 char *out_pg_filename );

extern void UTIL_doBeliefUpdates();

extern void UTIL_compareAlphaFilesUsingBeliefs( char *alpha1_filename,
									   char *alpha2_filename,
									   char *belief_filename,
									   double epsilon,
									   char *out_filename  );

#endif
