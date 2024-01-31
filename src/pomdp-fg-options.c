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
 * This code was automatically generated on February 2005 by the program:
 *
 *     gen-program-opts.py
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "pomdp-fg-options.h"

/*
 * Strings arrays for parameters.
 */

char* POMDP_FG_OPTS_Grid_Type_Str[] = POMDP_FG_OPTS_OPT_GRID_TYPE_STRINGS;

/*******************************************************/
PomdpFgProgOptions
POMDP_FG_OPTS_new( )
{

  PomdpFgProgOptions options;

  options = (PomdpFgProgOptions) XMALLOC( sizeof( *options ));

  strcpy( options->__exec_name__, "pomdp-fg" );

  strcpy( options->__version__, "0.1" );

  options->__error__ = 0;

  options->max_secs = 0;
  options->finite_grid_points = POMDP_FG_OPTS_OPT_MAX_GRID_POINTS_DEFAULT;
  options->initial_values_filename[0] = '\0';
  options->save_penultimate = POMDP_FG_OPTS_OPT_SAVE_PENULTIMATE_DEFAULT;
  options->memory_limit = 0;
  options->pomdp_filename[0] = '\0';
  options->True[0] = '\0';
  options->rand_seed[0] = '\0';
  options->save_all = POMDP_FG_OPTS_OPT_SAVE_ALL_DEFAULT;
  options->initial_grid_filename[0] = '\0';
  strcpy( options->prefix_str, POMDP_FG_OPTS_OPT_O_DEFAULT );
  options->horizon = 0;
  options->finite_grid_save = POMDP_FG_OPTS_OPT_GRID_SAVE_DEFAULT;
  options->finite_grid_type = POMDP_FG_OPTS_OPT_GRID_TYPE_DEFAULT;

  return( options );

}  /* POMDP_FG_OPTS_new */

/*******************************************************/
void
POMDP_FG_OPTS_delete( PomdpFgProgOptions options )
{

  XFREE( options );

}  /* POMDP_FG_OPTS_delete */

/*******************************************************/
ConfigFile
POMDP_FG_OPTS_toConfigFile( PomdpFgProgOptions options )
{
  ConfigFile cfg;
  char str[1096];

  cfg = CF_new();

  sprintf( str, "%d", options->max_secs );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_TIME_LIMIT_STR, str );

  sprintf( str, "%d", options->finite_grid_points );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_MAX_GRID_POINTS_STR, str );

  sprintf( str, "%s", options->initial_values_filename );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_INITIAL_VALUES_STR, str );

  sprintf( str, "%s", Boolean_Str[options->save_penultimate] );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_SAVE_PENULTIMATE_STR, str );

  sprintf( str, "%d", options->memory_limit );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_MEMORY_LIMIT_STR, str );

  sprintf( str, "%s", options->pomdp_filename );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_POMDP_STR, str );

  sprintf( str, "%s", options->True );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_F_STR, str );

  sprintf( str, "%s", options->rand_seed );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_RAND_SEED_STR, str );

  sprintf( str, "%s", Boolean_Str[options->save_all] );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_SAVE_ALL_STR, str );

  sprintf( str, "%s", options->initial_grid_filename );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_INITIAL_GRID_STR, str );

  sprintf( str, "%s", options->prefix_str );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_O_STR, str );

  sprintf( str, "%d", options->horizon );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_HORIZON_STR, str );

  sprintf( str, "%s", Boolean_Str[options->finite_grid_save] );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_GRID_SAVE_STR, str );

  sprintf( str, "%s", POMDP_FG_OPTS_Grid_Type_Str[options->finite_grid_type] );
  CF_addParam( cfg, POMDP_FG_OPTS_CFG_GRID_TYPE_STR, str );

  return cfg;

} /* POMDP_FG_OPTS_toConfigFile */

/*******************************************************/
void 
POMDP_FG_OPTS_showUsageBrief( FILE* file, char* exec_name )
{
  fprintf( file, "Usage: %s [opts...] [args...]\n", exec_name );
  fprintf( file, "Use '-h' for help.\n");

}  /* POMDP_FG_OPTS_showUsageBrief */

/*******************************************************/
void 
POMDP_FG_OPTS_showUsage( FILE* file, char* exec_name )
{
  fprintf( file, "Usage: %s [opts...] [args...]\n", exec_name );

  /*******************************/
  /* __MAIN__ parameters  */
  /*******************************/
  fprintf( file, "General options:\n" );

  PO_showUsageEnumType( file,
                     POMDP_FG_OPTS_ARG_SAVE_PENULTIMATE_STR,
                     Boolean_Str );
  fprintf( file, "\t%s <string>\n", POMDP_FG_OPTS_ARG_F_STR );
  fprintf( file, "\t%s <string>\n", POMDP_FG_OPTS_ARG_RAND_SEED_STR );
  PO_showUsageEnumType( file,
                     POMDP_FG_OPTS_ARG_SAVE_ALL_STR,
                     Boolean_Str );
  fprintf( file, "\t%s <string>\n", POMDP_FG_OPTS_ARG_O_STR );

  /*******************************/
  /* Domain parameters  */
  /*******************************/
  fprintf( file, "Domain options:\n" );

  fprintf( file, "\t%s <string>\n", POMDP_FG_OPTS_ARG_POMDP_STR );
  fprintf( file, "\t%s <int>\n", POMDP_FG_OPTS_ARG_HORIZON_STR );

  /*******************************/
  /* Resource Limits parameters  */
  /*******************************/
  fprintf( file, "Resource Limits options:\n" );

  fprintf( file, "\t%s <int>\n", POMDP_FG_OPTS_ARG_TIME_LIMIT_STR );
  fprintf( file, "\t%s <int>\n", POMDP_FG_OPTS_ARG_MEMORY_LIMIT_STR );

  /*******************************/
  /* Value Function parameters  */
  /*******************************/
  fprintf( file, "Value Function options:\n" );

  fprintf( file, "\t%s <string>\n", POMDP_FG_OPTS_ARG_INITIAL_VALUES_STR );

  /*******************************/
  /* Belief Grid parameters  */
  /*******************************/
  fprintf( file, "Belief Grid options:\n" );

  fprintf( file, "\t%s <int>\n", POMDP_FG_OPTS_ARG_MAX_GRID_POINTS_STR );
  fprintf( file, "\t%s <string>\n", POMDP_FG_OPTS_ARG_INITIAL_GRID_STR );
  PO_showUsageEnumType( file,
                     POMDP_FG_OPTS_ARG_GRID_SAVE_STR,
                     Boolean_Str );
  PO_showUsageEnumType( file,
                     POMDP_FG_OPTS_ARG_GRID_TYPE_STR,
                     POMDP_FG_OPTS_Grid_Type_Str );

}  /* POMDP_FG_OPTS_showUsage */

/*******************************************************/
PomdpFgProgOptions
POMDP_FG_OPTS_parse( ProgramOptions opts )
{
  PomdpFgProgOptions options;
  int enum_idx;
  int ret_value;

  options = POMDP_FG_OPTS_new();

  PO_startValidate( opts );

  ret_value = PO_getIntegerOption( opts,
                           POMDP_FG_OPTS_ARG_TIME_LIMIT_STR,
                            &(options->max_secs),
                           1,
                           INT_MAX );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'max_secs' has invalid value." );

  ret_value = PO_getIntegerOption( opts,
                           POMDP_FG_OPTS_ARG_MAX_GRID_POINTS_STR,
                            &(options->finite_grid_points),
                           1,
                           INT_MAX );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'finite_grid_points' has invalid value." );

  ret_value = PO_getStringOption( opts,
                         POMDP_FG_OPTS_ARG_INITIAL_VALUES_STR,
                         options->initial_values_filename,
                         NULL,
                         NULL );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'initial_values_filename' has invalid value." );

  ret_value = PO_getEnumOption( opts,
                         POMDP_FG_OPTS_ARG_SAVE_PENULTIMATE_STR,
                         &(enum_idx),
                         Boolean_Str );
  options->save_penultimate = enum_idx;
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'save_penultimate' has invalid value." );

  ret_value = PO_getIntegerOption( opts,
                           POMDP_FG_OPTS_ARG_MEMORY_LIMIT_STR,
                            &(options->memory_limit),
                           1,
                           INT_MAX );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'memory_limit' has invalid value." );

  ret_value = PO_getStringOption( opts,
                         POMDP_FG_OPTS_ARG_POMDP_STR,
                         options->pomdp_filename,
                         NULL,
                         NULL );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'pomdp_filename' has invalid value." );

  ret_value = PO_getStringOption( opts,
                         POMDP_FG_OPTS_ARG_F_STR,
                         options->True,
                         NULL,
                         NULL );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'true' has invalid value." );

  ret_value = PO_getStringOption( opts,
                         POMDP_FG_OPTS_ARG_RAND_SEED_STR,
                         options->rand_seed,
                         NULL,
                         NULL );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'rand_seed' has invalid value." );

  ret_value = PO_getEnumOption( opts,
                         POMDP_FG_OPTS_ARG_SAVE_ALL_STR,
                         &(enum_idx),
                         Boolean_Str );
  options->save_all = enum_idx;
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'save_all' has invalid value." );

  ret_value = PO_getStringOption( opts,
                         POMDP_FG_OPTS_ARG_INITIAL_GRID_STR,
                         options->initial_grid_filename,
                         NULL,
                         NULL );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'initial_grid_filename' has invalid value." );

  ret_value = PO_getStringOption( opts,
                         POMDP_FG_OPTS_ARG_O_STR,
                         options->prefix_str,
                         NULL,
                         NULL );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'prefix_str' has invalid value." );

  ret_value = PO_getIntegerOption( opts,
                           POMDP_FG_OPTS_ARG_HORIZON_STR,
                            &(options->horizon),
                           1,
                           INT_MAX );
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'horizon' has invalid value." );

  ret_value = PO_getEnumOption( opts,
                         POMDP_FG_OPTS_ARG_GRID_SAVE_STR,
                         &(enum_idx),
                         Boolean_Str );
  options->finite_grid_save = enum_idx;
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'finite_grid_save' has invalid value." );

  ret_value = PO_getEnumOption( opts,
                         POMDP_FG_OPTS_ARG_GRID_TYPE_STR,
                         &(enum_idx),
                         POMDP_FG_OPTS_Grid_Type_Str );
  if ( ret_value == PO_OPT_PRESENT_VALID )
    options->finite_grid_type = enum_idx;
  if ( ret_value == PO_OPT_PRESENT_ERROR )
    PO_handleError( opts, "Option 'finite_grid_type' has invalid value." );

  PO_endValidate( opts );

  if( ! PO_isValid( opts ))
    options->__error__ = 1;
  return options;

}  /* POMDP_FG_OPTS_parse */

/*******************************************************/
PomdpFgProgOptions
POMDP_FG_OPTS_create( int argc, char** argv )
{
  PomdpFgProgOptions options;
  ProgramOptions opts;
  ConfigFile cfg;

  opts = PO_create( argc, argv );

  if ( opts->usage )
    {
      POMDP_FG_OPTS_showUsage( stdout,
          opts->cmd_line->exec_name );
      PO_delete( opts );
      exit( 1 );
    }

  if ( ! PO_isValid( opts ))
    {
      POMDP_FG_OPTS_showUsageBrief( stdout, 
          opts->cmd_line->exec_name );
      PO_delete( opts );
      exit( 1 );
    }

  options = POMDP_FG_OPTS_parse( opts );

  if ( options->__error__ )
    {
      POMDP_FG_OPTS_showUsageBrief( stdout, 
          opts->cmd_line->exec_name );
      PO_delete( opts );

      POMDP_FG_OPTS_delete( options );
      exit( 1 );
    }

  PO_delete( opts );

  return options;

} /* POMDP_FG_OPTS_create */

/* end C code */
