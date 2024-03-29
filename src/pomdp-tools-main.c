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
 * This is a program that has all the pomdp-solve code, but which
 * houses various utilities for examining and manipulating the files
 * related to this program (input and outputs. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mdp/mdp.h"

#include "global.h"
#include "params.h"
#include "common.h"
#include "pomdp.h"
#include "utils.h"
#include "pg-eval.h"
#include "lp-interface.h"

#include "pomdp-tools-options.h"

/**********************************************************************/
int main( int argc, char **argv )
{
  PomdpToolsProgOptions tool_options;
  PomdpSolveParams solve_params;

  tool_options = POMDP_TOOLS_OPTS_create( argc, argv );

  gStdErrFile = stderr;

  solve_params = newPomdpSolveParams();

  /* Manually set these. */
  gNumStates = tool_options->states;
  gNumObservations = tool_options->obs;

  /*
    CF_printf( POMDP_TOOLS_OPTS_toConfigFile( tool_options ) );
  */

  switch( tool_options->tool )
    {
    case POMDP_TOOLS_OPTS_Tool_compare_alphas:
	 if (( tool_options->states < 1 )
		|| ( strcmp( tool_options->alpha1, "" ) == 0 )
		|| ( strcmp( tool_options->alpha2, "" ) == 0 )
		|| ( strcmp( tool_options->result_filename, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s, %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_ALPHA1_STR,
			    POMDP_TOOLS_OPTS_ARG_ALPHA2_STR,
			    POMDP_TOOLS_OPTS_ARG_O_STR,
			    POMDP_TOOLS_OPTS_ARG_STATES_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }

	   
	 UTIL_compareAlphaFiles( tool_options->alpha1, 
						tool_options->alpha2, 
						tool_options->epsilon, 
						tool_options->result_filename );
	 break;

    case POMDP_TOOLS_OPTS_Tool_belief_alpha_compare:
	 if (( tool_options->states < 1 )
		|| ( strcmp( tool_options->alpha1, "" ) == 0 )
		|| ( strcmp( tool_options->alpha2, "" ) == 0 )
		 || ( strcmp( tool_options->belief, "" ) == 0 )
		|| ( strcmp( tool_options->result_filename, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s, %s, %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_ALPHA1_STR,
			    POMDP_TOOLS_OPTS_ARG_ALPHA2_STR,
			    POMDP_TOOLS_OPTS_ARG_BELIEF_STR,
			    POMDP_TOOLS_OPTS_ARG_O_STR,
			    POMDP_TOOLS_OPTS_ARG_STATES_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }

	   
	 UTIL_compareAlphaFilesUsingBeliefs( tool_options->alpha1, 
								  tool_options->alpha2, 
								  tool_options->belief, 
								  tool_options->epsilon, 
								  tool_options->result_filename );
	 break;

    case POMDP_TOOLS_OPTS_Tool_pg_eval:
	 if (( strcmp( tool_options->pomdp, "" ) == 0 )
		|| ( strcmp( tool_options->pg1, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_PG1_STR,
			    POMDP_TOOLS_OPTS_ARG_POMDP_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }

	 /* See the pg-search-source/pg-eval-main.c file */

	 initializePomdp( tool_options->pomdp, 
				   1e-5 );

	 PGE_main( tool_options->pg1 );
	   
	 cleanUpPomdp();

	 break;

    case POMDP_TOOLS_OPTS_Tool_pg_relink:
	 if (( strcmp( tool_options->pomdp, "" ) == 0 )
		|| ( strcmp( tool_options->alpha1, "" ) == 0 )
		|| ( strcmp( tool_options->alpha2, "" ) == 0 )
		|| ( strcmp( tool_options->pg1, "" ) == 0 )
		|| ( strcmp( tool_options->result_filename, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s, %s, %s and %s..\n",
			    POMDP_TOOLS_OPTS_ARG_ALPHA1_STR,
			    POMDP_TOOLS_OPTS_ARG_ALPHA2_STR,
			    POMDP_TOOLS_OPTS_ARG_POMDP_STR,
			    POMDP_TOOLS_OPTS_ARG_O_STR,
			    POMDP_TOOLS_OPTS_ARG_PG1_STR  );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }
	 
	 initializePomdp( tool_options->pomdp, 
				   1e-5 );
	 
	 UTIL_relinkPolicyGraph( tool_options->alpha1,
						tool_options->pg1,
						tool_options->alpha2,
						tool_options->result_filename );
	 
	 cleanUpPomdp();

	 break;

    case POMDP_TOOLS_OPTS_Tool_map_beliefs:
	 if ( (tool_options->states < 1 )
		 || ( strcmp( tool_options->belief, "" ) == 0 )
		 || ( strcmp( tool_options->alpha1, "" ) == 0 )
		 || ( strcmp( tool_options->result_filename, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s, %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_BELIEF_STR,
			    POMDP_TOOLS_OPTS_ARG_ALPHA1_STR,
			    POMDP_TOOLS_OPTS_ARG_O_STR,
			    POMDP_TOOLS_OPTS_ARG_STATES_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }
	   
	 UTIL_mapBeliefList( tool_options->belief, 
					 tool_options->alpha1, 
					 tool_options->epsilon, 
					 tool_options->result_filename );
	 break;

    case POMDP_TOOLS_OPTS_Tool_pg_isomorphism:
	 if ( (tool_options->obs < 1 )
		 || ( strcmp( tool_options->pg1, "" ) == 0 )
		 || ( strcmp( tool_options->pg2, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_PG1_STR,
			    POMDP_TOOLS_OPTS_ARG_PG2_STR,
			    POMDP_TOOLS_OPTS_ARG_OBS_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }
	   
	 fprintf( stderr, "This feature not yet implemented\n" );
	 exit( -1 );
	 break;

    case POMDP_TOOLS_OPTS_Tool_purge_alphas:
	 if ( (tool_options->states < 1 )
		 || ( strcmp( tool_options->alpha1, "" ) == 0 )
		 || ( strcmp( tool_options->result_filename, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_ALPHA1_STR,
			    POMDP_TOOLS_OPTS_ARG_O_STR,
			    POMDP_TOOLS_OPTS_ARG_STATES_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }
	   
	 initCommon();
	 initGlobal();
	 initLpInterface( solve_params );
	 
	 UTIL_purgeAlphaFile( tool_options->alpha1, 
					  tool_options->result_filename, 
					  solve_params );
	 break;

    case POMDP_TOOLS_OPTS_Tool_sort_alphas:
	 if ( (tool_options->states < 1 )
		 || ( strcmp( tool_options->alpha1, "" ) == 0 )
		 || ( strcmp( tool_options->result_filename, "" ) == 0 ))
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s, %s and %s.\n",
			    POMDP_TOOLS_OPTS_ARG_ALPHA1_STR,
			    POMDP_TOOLS_OPTS_ARG_O_STR,
			    POMDP_TOOLS_OPTS_ARG_STATES_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }

	 UTIL_sortAlphaFile( tool_options->alpha1,
					 tool_options->result_filename );

	 break;

    case POMDP_TOOLS_OPTS_Tool_update_beliefs:
	 if ( strcmp( tool_options->pomdp, "" ) == 0 )
	   {
		fprintf( stderr, "Missing required arguments(s) for '%s'\n",
			    POMDP_TOOLS_OPTS_Tool_Str[tool_options->tool] );
		fprintf( stderr, "Need: %s.\n",
			    POMDP_TOOLS_OPTS_ARG_POMDP_STR );
		POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
		exit( -1 );
	   }

	 initializePomdp( tool_options->pomdp, 
				   1e-5 );

	 UTIL_doBeliefUpdates();

	 if ( gInitialBelief == NULL )
	   {
		fprintf( stderr, 
			    "POMDP file '%s' must specify a starting belief state\n",
			    tool_options->pomdp );
		fprintf( stderr, "Aborted.\n" );
		exit( -1 );
	   }

	 cleanUpPomdp();
	 break;

    default:
	 fprintf( stderr, "Unreckognized tool option\n" );
	 POMDP_TOOLS_OPTS_showUsageBrief( stderr, argv[0] );
	 
	 exit( -1 );

    } /* switch param->tool */

  /* This will also take care of deallocating the 'opts' structure. */
  destroyPomdpSolveParams( solve_params );

  return 0;
} /* main */
