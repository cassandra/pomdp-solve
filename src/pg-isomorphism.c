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
 * Takes two policy graph files and sees if the smaller is isomorphic
 * to some subgraph of the larger.
 */

#include <stdio.h>
#include <stdlib.h>

#include "mdp/mdp.h"

#include "global.h"
#include "params.h"
#include "pomdp.h"
#include "utils.h"
#include "pg.h"

char pgiPGFile1[MAX_FILENAME_LENGTH];
char pgiPGFile2[MAX_FILENAME_LENGTH];
double pgiEpsilon;

/**********************************************************************/
int 
PGI_allMarked( int *iso_fwd_map, int num_nodes )
{
  /* Returns TRUE if all the nodes in the isomorphic mapping have been
	used.  */
  int n;

  for ( n = 0; n < num_nodes; n++ )
    if ( iso_fwd_map[n] == INVALID_NODE )
	 return FALSE;

  return TRUE;
} /* PGI_allMarked */

/**********************************************************************/
void
PGI_foundIsomorphism( PG small_pg, int *iso_fwd_map, int *iso_bwd_map)
{
  int n;

  printf( "\n** Isomorphism found **\n" );

  printf( "\n\tsmall    large\n" );
  printf( "\t==============\n" );

  for ( n = 0; n < small_pg->num_nodes; n++ )
    {
	 printf( "\t%5d -> %5d\n", n, iso_fwd_map[n] );
    }

} /* PGI_foundIsomorphism */
/**********************************************************************/
int
PGI_isIsomorphic(  PG small_pg, 
			    PG large_pg,
			    int *iso_fwd_map )
{
  /* With nodes assigned, need to check that 'next' mappings match.
	This gets tricky trying to keep the indices straight.
  */
  int n, z, large_n, next_small, next_large;

  for ( n = 0; n < small_pg->num_nodes; n++ ) 
    {

	 large_n = iso_fwd_map[n];

	 /* Check that actions match (this has already been checked, but
	    it doesn't hurt to do it again. */
	 if (  small_pg->action[n] != large_pg->action[large_n] )
	   return FALSE;
	 
	 /* now check all transitions */
	 for( z = 0; z < small_pg->num_obs; z++ )
	   {
		next_small = small_pg->next[n][z];

		next_large = large_pg->next[large_n][z];

		if ( iso_fwd_map[next_small] != next_large )
		  return FALSE;
		
	   } /* for z */
	 
    } /* for n */

  return TRUE;

} /* PGI_isIsomorphic */
/**********************************************************************/
int
PGI_searchForSubgraphIsomorphism( PG small_pg, 
						    PG large_pg,
						    int *iso_fwd_map, 
						    int *iso_bwd_map )
{
  int iso_count = 0;
  int small_n = INVALID_NODE;
  int large_n;
  int n;

  /* First find the first node not being used (in smaller list) */

  for ( n = 0; n < small_pg->num_nodes; n++ )
    if ( iso_fwd_map[n] == INVALID_NODE )
	 {
	   small_n = n;
	   break;
	 }

  /* Recursion base case: everything is used. */
  if ( small_n == INVALID_NODE )
    {
	 if ( PGI_isIsomorphic( small_pg, large_pg, iso_fwd_map ))
	   {
		PGI_foundIsomorphism( small_pg, iso_fwd_map, iso_bwd_map);
		return 1;
	   }
	 else
	   return 0;
    }

  /* Try all combinations of the first unused node with node sin the
	larger list. */
  for ( large_n = 0; large_n < large_pg->num_nodes; large_n++ )
    {
	 
	 /* Skip those that are already being used. */
	 if ( iso_bwd_map[large_n] != INVALID_NODE )
	   continue;
	 
	 /* Skip if the actions do not match. */
	 if ( small_pg->action[small_n]
		 != large_pg->action[large_n] )
	   continue;
	 
	 /* If we knew the alpha vector values, we could add another
	    check here that does an epsilon comparison of the
	    components to make sure they are close.
	 */
	 
	 /* Add a mapping to try. */
	 iso_fwd_map[small_n] = large_n;
	 iso_bwd_map[large_n] = small_n;
	 
	 iso_count += PGI_searchForSubgraphIsomorphism( small_pg, 
										   large_pg,
										   iso_fwd_map, 
										   iso_bwd_map );
	 
	 /* Remove mapping so we can try again for some other
	    match. */ 
	 iso_fwd_map[small_n] = INVALID_NODE;
	 iso_bwd_map[large_n] = INVALID_NODE;
	 
    } /* for large_n */
  
  return iso_count;

} /* PGI_searchForSubgraphIsomorphism */

/**********************************************************************/
void
PGI_checkPGIsomorphism( char *pg_file1, char *pg_file2 )
{
  PG tmp_pg1, tmp_pg2, smaller_pg, larger_pg;
  int *iso_map_smaller_to_larger;
  int *iso_map_larger_to_smaller;
  int n, iso_count;

  /* First we read in the two policy graphs. */
  tmp_pg1 = PG_read( pg_file1, TRUE );
  tmp_pg2 = PG_read( pg_file2, TRUE );

  /* We will test the smaller to see if it is a subgraph of the
	larger. */
  if ( tmp_pg1->num_nodes <= tmp_pg2->num_nodes )
    {
	 smaller_pg = tmp_pg1;
	 larger_pg = tmp_pg2;
    }
  else
    {
	 smaller_pg = tmp_pg2;
	 larger_pg = tmp_pg1;
    }
	
  iso_map_smaller_to_larger
    = (int *) malloc( smaller_pg->num_nodes * sizeof(int) );

  for ( n = 0; n < smaller_pg->num_nodes; n++ )
    iso_map_smaller_to_larger[n] = INVALID_NODE;

  iso_map_larger_to_smaller
    = (int *) malloc( larger_pg->num_nodes * sizeof(int) );

  for ( n = 0; n < larger_pg->num_nodes; n++ )
    iso_map_larger_to_smaller[n] = INVALID_NODE;
  
  iso_count = PGI_searchForSubgraphIsomorphism( smaller_pg,
									   larger_pg,
									   iso_map_smaller_to_larger,
									   iso_map_larger_to_smaller );

  printf( "\nDone. There were %d isomorphisms found.\n", iso_count );

} /* checkPGIsomorphism */

/**********************************************************************/
PomdpSolveParams
PGI_parseCmdLine( int argc, char **argv )
{
  PomdpSolveParams param;
  char str[MAX_MSG_LENGTH];
  int *mark_args;
  int cmd_line_valid = TRUE;
  int i;

  param = newPomdpSolveParams();

  pgiPGFile1[0] = '\0';
  pgiPGFile2[0] = '\0';

  gStdErrFile = stderr;
  strcpy( gExecutableName, argv[0] );

  mark_args = (int *) XCALLOC( argc, sizeof( int ));

  if( getStringParam( argc, argv, 
				  "-obs",
				  mark_args, str )) 
    gNumObservations = atoi( str );

  if( getStringParam( argc, argv, 
				  "-pg1",
				  mark_args, str )) 
    strcpy( pgiPGFile1, str );

  if( getStringParam( argc, argv, 
				  "-pg2",
				  mark_args, str )) 
    strcpy( pgiPGFile2, str );

   /* Now we look for arguments that weren't parsed or which
      were parsed more than once. The first argument is just
      the program name, so start the loop at 1. */

   for ( i = 1; i < argc; i++ )
     if ( mark_args[i] != 1 ) {
       
       fprintf( param->report_file, 
                "** Error: cmd line option problem for '%s'\n",
                argv[i] );
       cmd_line_valid = FALSE;
     }
   
   free( mark_args );

   if (( pgiPGFile1[0] == '\0' )
	  || ( pgiPGFile2[0] == '\0' ))
	cmd_line_valid = FALSE;

     /* If we had a problem parsing the command line, then show the
      help message and bag out. */
   if ( ! cmd_line_valid ) {

    fprintf( stderr, 
		  "Usage: %s -obs n -pg1 pg_file1 -pg2 pg_file2\n", 
		  gExecutableName  );

     exit( -1 );

   } /* if bad command line */  

   return ( param );

} /*PGI_parseCmdLine */
/**********************************************************************/

int 
main( int argc, char **argv )
{
  PomdpSolveParams param;

  param = PGI_parseCmdLine( argc, argv );

  PGI_checkPGIsomorphism( pgiPGFile1, pgiPGFile2 );
  
  return 0;
} /* main */
