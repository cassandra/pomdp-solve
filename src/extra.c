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
 
/**********************************************************************/
parseCmdLineEpsilons
/**********************************************************************/



  /* This may need to change.  The relationship between these epsilons
     and the global epsilon might not be so straightforward. Also
     might need to add the enforceSmallestPrecision to be called on
     each of these if they are changed. */
  /* param->alpha_epsilon = param->epsilon; */
  param->vertex_epsilon = param->epsilon;
  param->double_equality_precision = param->epsilon;

  /* Because the impossible_obs_epsilon epsilon is related to
     probabilities in the model, an epsilon here near or greater than
     1 will mean that no observations will look possible.  For this
     reason, we have to take this to be some minimal value using
     epsilon nd default. */
  param->impossible_obs_epsilon = Min( param->epsilon, 
                                       DEFAULT_IMPOSSIBLE_OBS_EPSILON );
  
  /* Adjust the weak bound criteria, if none specified.  This
     requires us to look for the weak bound delta *after* the
     -epsilon argument so that we can override this if need
     be.  Added 8/17/95. */ 
  param->stop_delta = param->epsilon;

    if (( opts->stop_delta zzzDoesNotExistOnCommandLine )
	   && ( opts->stop_criteria == stop_weak ))
      Warning
        ( "Weak bound delta on command line ignored.");


/**********************************************************************/
parseCmdLineViVariations
/**********************************************************************/



  /****************/
  /* When using the fixed solution size value iteration variation,
     this sets the maximum number of vectors to allow as the solution
     on any epoch. Only parse this if that variation of VI was
     chosen. */
  if ( opts->vi_variation == FixedSolnSizeVi ) {
    
    getIntParam( argc, argv, 
                 CMD_ARG_SOLN_SIZE,
                 mark_args, 
                 &(param->max_soln_size), 0, 0 );
    
    /* Make sure max solution size is not negative. */
    if ( param->max_soln_size < 1 ) {
      
      Warning( "max_soln is not positive.  Using normal VI." );
      opts->max_soln_size = 0;
      opts->vi_variation = NormalVi;
    } /* if max solution size makes no sense. */
    
  } /* if param->vi_variation == FixedSolnSizeVi */

  /* Only parse the adjustable epsilon command line stuff if this or
     the fixed solution size option is chosen. The command line things
     that are relevant are:
     
     starting epsilon
     stopping epsilon
     epsilon increment
     history length
     size history delta 
  */
  if (( param->vi_variation == AdjustableEpsilonVi )
      || ( param->vi_variation == FixedSolnSizeVi )) {
    
    /****************/
    /* When using the adjustable epsilon value iteration variation, this
       sets the epsilon for the first epoch of value iteration. Also
       used for the fixed size solution variation. */
    getDoubleParam( argc, argv,  
                    CMD_ARG_STARTING_EPSILON,
                    mark_args, 
                    &(param->starting_epsilon), 0.0, 0.0 );
    
    /****************/
    /* When using the adjustable epsilon value iteration variation, this
       sets the epsilon for the final or most precise value. Also used
       for the fixed size variation of value iteration. */
    getDoubleParam( argc, argv, 
                    CMD_ARG_ENDING_EPSILON,
                    mark_args, 
                    &(param->ending_epsilon), 0.0, 0.0 );
    
    /****************/
    /* When using the adjustable epsilon value iteration variation, this
       sets the increment for the epsilon if it need to adjust. */
    getDoubleParam( argc, argv, 
                    CMD_ARG_EPSILON_ADJUST_FACTOR,
                    mark_args, 
                    &(param->epsilon_adjust_factor), 0.0, 0.0 );
    
    /****************/
    /* When using the adjustable epsilon value iteration variation, this
       helps define the criteria for when the epsilon should be
       adjusted.  It looks at the sizes of the resulting sets over the
       last few epochs and determines if they are all of approaximately
       the same size. This parameter defines how many epochs in the past
       it should look. */
    getIntParam( argc, argv, 
                 CMD_ARG_HISTORY_WINDOW_LENGTH,
                 mark_args, 
                 &(param->epoch_history_window_length), 0, 0 );
    
    /****************/
    /* When using the adjustable epsilon value iteration variation, this
       helps define the criteria for when the epsilon should be
       adjusted. It looks at the sizes of the resulting sets over the
       last few epochs and determines if they are all of approximately
       the same size. This parameter defines what it means for the
       previous solution sizes to be approximately the same size.  They
       must all be within this many vectors of each other. */
    getIntParam( argc, argv,  
                 CMD_ARG_HISTORY_WINDO_DELTA,
                 mark_args, 
                 &(param->epoch_history_window_delta), 0, 0 );
    
  } /* if either adjustable epsilon or fixed soln size variation */

  /****************/
  /* Must enforce these things if the zlz variation is chosen. */
  if ( param->vi_variation == ZlzSpeedup ) {
    
    Warning ( "ZLZ speedup enforcing non-negative rewards." );
    
    gRequireNonNegativeRewards = TRUE;
    
    if ( param->use_witness_points )
      Warning( "ZLZ speedup requires witness points. Overriding.");
    
    param->use_witness_points = TRUE;
    
    if ( param->stop_criteria != stop_bellman )
      Warning( "ZLZ speedup requires Bellman stop criteria. Overriding.");
    
    param->stop_criteria = stop_bellman;
    
  } /* if using the ZLZ speedup */
