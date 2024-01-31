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

#ifndef MDP_PARSE_HASH_H
#define MDP_PARSE_HASH_H 1

#define HASH_TABLE_SIZE      255

typedef enum { nt_state, nt_action, 
               nt_observation, nt_unknown } Mnemonic_Type;

typedef struct Node_Struct *Node;
struct Node_Struct {
   Mnemonic_Type type;
   int number;
   char *str;
   Node next;
};

extern void H_create();
extern void H_destroy();
extern int H_enter( char *str, Mnemonic_Type type );
extern int H_lookup( char *str, Mnemonic_Type type );

#endif
