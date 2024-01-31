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

#ifndef TIMING_H
#define TIMING_H

/*******************************************************************/
/**************       EXTERNAL VARIABLES            ****************/
/*******************************************************************/

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

/* Get total CPU time in seconds breaking it down by user and system
   time. */
extern void getSecsDetail( double *user_time, 
                           double *system_time );

/* Get total CPU time in seconds including user and system time. */
extern double getSecs( );

/* Report the total secons time in a nicer hr, min sec format with a
   string to label what the time is for.  */
extern void reportTimes( FILE *file,
                         double tot_secs, 
                         char *str );
   
#endif
