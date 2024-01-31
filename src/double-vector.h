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
 *  Representations for vectors of doubles (used in value functions
 *  and belief points) and dynamically allocated lists of these.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef DOUBLE_VECTOR_H
#define DOUBLE_VECTOR_H

/*******************************************************************/
/**************      USEFUL MNENOMIC CONSTANTS      ****************/
/*******************************************************************/

/*******************************************************************/
/**************             TYPEDEFS                ****************/
/*******************************************************************/

/* A vector is just an array of doubles. */

typedef double* DoubleVector;

/* A list of vectors is a linked list of DoubleVectors with a
   place for generic attributes for each vector. */

typedef struct DoubleVectorNodeStruct *DoubleVectorNode;
struct DoubleVectorNodeStruct {

  int id;                    /* unique ID (usually the position in list) */

  DoubleVector v;

  DoubleVectorNode next;

  /* Generic pointer to store specific attributes for a given sematics
	of the vector points. */ 
  void *attr;

};

/* This defines the list header. */

typedef struct DoubleVectorListStruct *DoubleVectorList;
struct DoubleVectorListStruct {

  int length;

  DoubleVectorNode head;
  DoubleVectorNode tail;

  /* Generic pointer to store specific attributes for a given sematics
	of the list. */ 
  void *attr;

};

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

extern DoubleVector
DV_new(  );

extern DoubleVector
DV_duplicate( DoubleVector v );

extern void 
DV_copy( DoubleVector dest, DoubleVector src );

extern void 
DV_destroy( DoubleVector v );

extern int 
DV_isSame( DoubleVector first, DoubleVector second, double epsilon );

extern int 
DV_isZero( DoubleVector v, double epsilon );

extern void 
DV_display( FILE *file, DoubleVector v );

extern void 
DV_show( DoubleVector v );

extern int 
DV_isLexicographicallyBetter( DoubleVector first,
						DoubleVector second,
						double epsilon );

extern int 
DV_isDominated( DoubleVector first, DoubleVector second );

extern void 
DV_destroyNode( DoubleVectorNode temp );

extern void 
DV_appendNodeToList( DoubleVectorList list, DoubleVectorNode node );

extern void 
DV_prependNodeToList( DoubleVectorList list, DoubleVectorNode node );

extern DoubleVectorNode 
DV_dequeueNode( DoubleVectorList list );

extern void 
DV_enqueueNode( DoubleVectorList list, DoubleVectorNode node );

extern DoubleVectorNode 
DV_duplicateNode( DoubleVectorNode node );

extern DoubleVectorNode
DV_appendDuplicateNodeToList( DoubleVectorList list, 
						DoubleVectorNode orig_node );

extern DoubleVectorList 
DV_newList( void* attr );

extern void 
DV_renumberList( DoubleVectorList list );

extern DoubleVectorNode
DV_prependList( DoubleVectorList list,
			 DoubleVector v,
			 void* attr );

extern DoubleVectorNode
DV_appendList( DoubleVectorList list,
			DoubleVector v,
			void* attr );

extern void 
DV_clearList( DoubleVectorList orig_list );

extern void 
DV_destroyList( DoubleVectorList list );

extern DoubleVectorNode 
DV_findNode( DoubleVectorList list, 
		   DoubleVector target,
		   double epsilon );

extern int 
DV_queryList( DoubleVectorList list, 
		    DoubleVector target,
		    double epsilon );

extern int 
DV_sizeOfList( DoubleVectorList list );

extern void 
DV_copyList( DoubleVectorList dest_list,
		   DoubleVectorList src_list );

extern DoubleVectorList 
DV_duplicateList( DoubleVectorList src_list );

extern int 
DV_isSameList( DoubleVectorList list1,
			DoubleVectorList list2,
			double epsilon );

extern int 
DV_isSimilaraList( DoubleVectorList list1, 
			    DoubleVectorList list2,
			    double epsilon );

extern void 
DV_unionTwoLists( DoubleVectorList list, DoubleVectorList other_list );

extern void 
DV_displayList( FILE *file, DoubleVectorList list );

extern void 
DV_showList( DoubleVectorList list );



#endif
