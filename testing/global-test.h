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

#define StartUnitTest(NAME) printf( "----- Unit test for: %s\n", NAME )
  
#define EndUnitTest(NAME) { \
  if ( error_count < 1 ) \
     printf( "  PASSED\n" ); \
  else \
     printf( "  FAILED with %d errors.\n", error_count ); \
  }

#define Error(MSG) { \
  printf( "  ** ERROR ** File: %s function: %s, Line: %d\n     ", \
          __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
  fputs( MSG, stdout );							   \
  fputs( "\n", stdout );								   \
  error_count++; }



