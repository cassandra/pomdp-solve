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
 *   This file contains the main routine for the pomdp-solve program.
 *   
 *   The command line arguments are shown by running:
 * 
 *         pomdp-solve -h
 */

#define MAIN_C

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "mdp/mdp.h"

#include "global.h"
#include "signal-handler.h"
#include "cmd-line.h"
#include "pomdp.h"
#include "alpha.h"
#include "belief.h"
#include "stats.h"
#include "random.h"
#include "pg.h"
#include "projection.h"
#include "enumeration.h"
#include "linear-support.h"
#include "two-pass.h"
#include "witness.h"
#include "inc-prune.h"
#include "pomdp-solve.h"

/**********************************************************************/
int 
main( int argc, char **argv ) {
  PomdpSolveParams param;
  
  /* Handles configuration file and command line options. */
  param = parseCmdLineAndCfgFile( argc, argv );

  /* To document what is actually being executed we dump out
     all the parameters of the execution. */
  showPomdpSolveParams( param );

  /* Do any file reading and memory allocations that are required. */
  initPomdpSolve( param );
  
  /* And away we go... */
  solvePomdp( param );

  /* Let's make all nice afterwards for all other things. Note that
     this deallocates the space for 'param' and its 'opts'
     component. */
  cleanUpPomdpSolve( param );

  return( 0 );

} /* main */
/**********************************************************************/

