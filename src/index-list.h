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
 *   Data structure and routines for maintaining a dynamic list of
 *   indices.
 *
 *   A list of indices is an ordered list of non-negative integers.
 * 
 */

#ifndef INDEX_LIST_H
#define INDEX_LIST_H

#define INITIAL_CAPACITY   10

#include <stdio.h>

/**********************************************************************/
/********************       CONSTANTS       ***************************/
/**********************************************************************/

/**********************************************************************/
/********************   DEFAULT VALUES       **************************/
/**********************************************************************/

/**********************************************************************/
/********************    TYPEDEFS            **************************/
/**********************************************************************/

typedef struct IndexListStruct *IndexList;
struct IndexListStruct {

  int size;
  int capacity;

  int* element;

};

/**********************************************************************/
/********************   EXTERNAL VARIABLES   **************************/
/**********************************************************************/

/**********************************************************************/
/********************   EXTERNAL FUNCTIONS    *************************/
/**********************************************************************/


IndexList IL_new();

void IL_dispose( IndexList list );

void IL_resize( IndexList list );

void IL_append( IndexList list, int value );

int IL_sizeOf( IndexList list );
       
#endif

