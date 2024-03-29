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

#ifndef FG_PARAMS_H
#define FG_PARAMS_H

#include "global.h"

#include "pomdp-fg-options.h"
#include "value-function.h"
#include "belief-state.h"

/*******************************************************************/
/**************      USEFUL MNENOMIC CONSTANTS      ****************/
/*******************************************************************/

/* The suffix to use for alpha vector files. */
#define VALUE_FILE_SUFFIX                    ".value"

/* The suffix to use for policy graph files. */
#define POLICY_FILE_SUFFIX                   ".policy"

/* The suffix to use for grid files. */
#define GRID_FILE_SUFFIX                   ".grid"

/* The suffix to use for penultimate alpha vector files. */
#define PENULTIMATE_SUFFIX                    ".penultimate"

/* We will save the solution after each iteration in a temporary file,
   so that if the program terminates abnormally, we can recover the
   latest solution. We will use the PID of the file to make sure the
   filename is unique so multiple copies can run at the same time. */
#define SAVE_FILE_NAME_FORMAT                "##pomdp-fg-%d##"

/*******************************************************************/
/**************             TYPEDEFS                ****************/
/*******************************************************************/

/* We just want to encapsulate all the parameters used in the
   pomdp-solve program into a single structure for convenience. These
   do not include any parameters that are specific to a particular
   algorithm. */ 
typedef struct FiniteGridParamStruct *FiniteGridParams;
struct FiniteGridParamStruct {

  /* This is the structure that holds all the things that can be
	configured through the command line or configuration file. */
  PomdpFgProgOptions opts;

  /* We'll keep track of the epoch (iteration) as value iteration
     progresses. */
  int cur_epoch;

  /* All messages will be output to the same file handle, which
     defaults to stdout if no filename is given on the command
     line. */
  FILE *report_file;

  /* Names of files to write out solutions */
  char values_filename[MAX_FILENAME_LENGTH];       /* alpha vectors */
  char policy_filename[MAX_FILENAME_LENGTH];      /* policy graph  */
  char grid_filename[MAX_FILENAME_LENGTH];        /* grid points   */

  ValueFunction initial_values;

  BeliefGrid initial_grid;

  /* We can set a timer to interrupt the program after too many
     seconds have elapsed.  It is done from the command line and is
     optional. Zero or negative values turns time limits off.  */
  int max_secs;

  /* The nature of these problems is such that they could require all
     the memory in the universe.  Therefore, to prevent them from
     going out and actually searching for all this memory, we put a
     ceiling on how much memory it can consume.  This can also be set
     via the command line if more or less is desired. Zero or negative
     values turns memory limits off. */
  int memory_limit;

  /* Whether or not to save each epoch's solution in a separate
     file. */
  int save_all;

  /* The filename to use as a backup file for each iteration's answer. */
  char backup_file[MAX_FILENAME_LENGTH];

  /* The filename to use for previous epoch solution. */
  char penultimate_filename[MAX_FILENAME_LENGTH];

};

/*******************************************************************/
/**************         DEFAULT VALUES              ****************/
/*******************************************************************/

#define DEFAULT_PREFIX                  "fg-solution"

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

/* Creates the memory for the structure to hold the parameters used in
   solving a POMDP.  Also sets the fields to the default values.  */
extern FiniteGridParams FGP_new(  );

/* Frees the memory for pointers in the params and the param structure
   itself.
*/
extern void FGP_destroy( FiniteGridParams param );

/*
  Main routine for parsing config file and command line. 
*/
extern FiniteGridParams FGP_parse( int argc, char **argv );

/* Display to stdout the current program parameters. */
extern void FGP_show( FiniteGridParams params );

#endif
