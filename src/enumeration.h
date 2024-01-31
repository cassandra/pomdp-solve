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

#ifndef ENUMERATION_H
#define ENUMERATION_H

#include "params.h"

/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

#define CMD_ARG_ENUM_PURGE_OPTION                "-enum_purge"

/* How to purge the set of vectors that are enumerated. */
#define DEFAULT_ENUM_PURGE_OPTION                purge_prune

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

/* For keeping some statistics while doing the enumeration. */
extern int gNumVectorsEnum;

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/

extern void initEnumeration();

extern void cleanUpEnumeration( );
      
/* This is the routine to call to do the enumeration exact POMDP
  algorithm given a set of this epochs projection vectors.  */
extern AlphaList improveEnumeration( AlphaList **projection,
                                     PomdpSolveParams param );

#endif
