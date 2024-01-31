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
 *   Routines for handling CPU times.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* This has: 'CLK_TCK'  */
#include <time.h>
#include <unistd.h>

/* This has: 'struct tms', times() */
#include <sys/times.h>

#include "timing.h"

/**********************************************************************/
void 
getSecsDetail( double *user_time, double *system_time ) 
{
  /* Get total CPU time in seconds breaking it down by user
     and system time. */
  struct tms time;

  times( &time );
  
  *user_time = (double) time.tms_utime / (double) sysconf(_SC_CLK_TCK);
  *system_time = (double) time.tms_stime / (double) sysconf(_SC_CLK_TCK);

}  /* getSecsDetail */
/**********************************************************************/
double 
getSecs( ) 
{
  /* Get total CPU time in seconds including user and system time. */
  double user_time, system_time;

  getSecsDetail( &user_time, &system_time );

  return ( user_time + system_time );

}  /* getSecs */
/**********************************************************************/
void 
reportTimes( FILE *file, double tot_secs, char *str ) 
{
  /* 
     Report the total secons time in a nicer hr, min sec format with a
     string to label what the time is for. 
  */

   int hrs, mins;
   double remaining_secs;

   hrs = (int)(tot_secs / 3600.0);
   remaining_secs = tot_secs - 3600*hrs;

   mins = (int)(remaining_secs / 60.0);
   remaining_secs = remaining_secs - 60*mins;

   fprintf( file,
            "%s %d hrs., %d mins, %.2lf secs. (= %.2lf secs)\n", 
            str, hrs, mins, remaining_secs, tot_secs );
   
}  /* reportTimes */
/**********************************************************************/
