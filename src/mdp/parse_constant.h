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

#ifndef MDP_PARSE_CONSTANT_H
#define MDP_PARSE_CONSTANT_H 1

typedef enum { CONST_INT, CONST_STRING, CONST_FLOAT } Const_Type;

typedef struct cNode
      {
	Const_Type	theTag;		/* Type of constant it is */
	union {
		int	theInt;		
		char	*theString;	
		double	theFloat;	
	      } theValue;
      } Constant_Block;

#endif

