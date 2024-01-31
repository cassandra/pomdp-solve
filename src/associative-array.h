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
  Module: AA

  A very simple associative array that maps string keys to string
  values. 
 */

#ifndef ASSOCIATIVE_ARRAY_H
#define ASSOCIATIVE_ARRAY_H

/*******************************************************/
/* Private Data */
/*******************************************************/

typedef struct AssocArrayStruct* AssocArray;
struct AssocArrayStruct {

  int max_size;     /* Size of memory allocated */
  int cur_size;     /* Number of used locations */

  char** keys;
  char** values;
};

/*******************************************************/
/* Public Data */
/*******************************************************/

extern AssocArray  
AA_new( int, int );

extern void  
AA_delete( AssocArray );

extern int 
AA_size( AssocArray );

extern int 
AA_put( AssocArray, char*, char* );

extern char* 
AA_get( AssocArray, char* );

extern void 
AA_fprintf( AssocArray, FILE* );

extern void 
AA_printf( AssocArray );

#endif
