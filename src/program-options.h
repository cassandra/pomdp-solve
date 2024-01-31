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
  Module: PO

  Handles all the details for managing a C program that you need to
  control through a combination of configuration file and command line
  options.  Command line options will override the configuration file
  options.  

  General usagage is to do something like this:

    ProgramOptions opts = PO_create( argc, argv );

    if ( ! PO_isValid( opts ))
       abort( "Aaargh! Something's wrong." );

    myarg_str = PO_getArgument( opts, 2 )

    myvalue_str = PO_getOption( opts, "myoption" )

    if ( PO_hasFlag( opts, "myflag" ))
       doSomething();

*/

#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include "command-line.h"
#include "config-file.h"

/*******************************************************/
/* Private Data */
/*******************************************************/

#define MAX_OPT_FLAGS               256
#define MAX_OPT_STRING_LEN         1024
#define MAX_OPT_ARGS                256

/* Various return condition codce when looking for options in data
   structure. 
*/
#define PO_OPT_INTERNAL_ERROR       -2
#define PO_OPT_PRESENT_ERROR        -1
#define PO_OPT_NOT_PRESENT           0
#define PO_OPT_PRESENT_VALID         1

#define MAX_ENUM_USAGE_WIDTH        70

/*
 * We convert all command line flags into program options that have a
 * true or false value.
 */
#define PO_FLAG_TRUE_VALUE "true"
#define PO_FLAG_FALSE_VALUE "false"

typedef struct ProgramOptionsStruct* ProgramOptions;
struct ProgramOptionsStruct {

  /* Anytime and operation fails on a mutator method of this data
	structure, this count is incremented.
  */
  int error_count;

  /* Indicate that we saw the '-h' flag on the command line so that we
	only need to display the usage information.
  */
  int usage;

  /* This really contains all the data we care about. We really want
	ProgramOptions to inherit from CommandLine, but that would
	require using C++. This is the lesser of the two evils. */
  CommandLine cmd_line;

  /* For tracking which have been seen for validation purposes so we
	can know when there were unreckognized arguments.
  */
  int* flag_seen;     
  int* option_seen;
  int* arg_seen;

};

/*******************************************************/
/* Public Data */
/*******************************************************/

typedef enum {
  Boolean_false,
  Boolean_true,
  Boolean__END__
} Boolean_Type;

#define OPT_BOOLEAN_STRINGS { \
    "false", \
    "true", \
    "" \
  }

extern char* Boolean_Str[];

extern ProgramOptions 
PO_new( );

extern void 
PO_delete( ProgramOptions );

extern void 
PO_handleError( ProgramOptions, char* );

extern int
PO_isValid( ProgramOptions );

extern int
PO_getNumArguments( ProgramOptions );

extern char*
PO_getArgument( ProgramOptions, int );

extern char*
PO_getOption( ProgramOptions, char* );

extern int
PO_hasFlag( ProgramOptions, char* );

extern int
PO_validate( ProgramOptions );

extern ProgramOptions
PO_create( int, char ** );

extern void 					 
PO_fprintf( ProgramOptions, FILE* );

extern void 				
PO_printf( ProgramOptions );

extern void 				
PO_startValidate( ProgramOptions opts );

extern void 				
PO_endValidate( ProgramOptions opts );

extern int
PO_getFlag( ProgramOptions, char* );

extern int
PO_getStringOption( ProgramOptions, char*, char*, char*, char*  );

extern int
PO_getEnumOption( ProgramOptions, char*, int*, char* foo[] );

extern int
PO_getIntegerOption( ProgramOptions, char*, int*, int, int  );

extern int
PO_getDoubleOption( ProgramOptions, char*, double*, double, double );

extern void 
PO_showUsageEnumType( FILE*, char*, char** );

#endif
