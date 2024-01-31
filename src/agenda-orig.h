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
 *   Used in witness algorithm to keep track of what alpha vectors
 *   we have worked on.
 *   Needs alpha.h also.
 * 
 */


typedef struct Agenda_Type *Agenda;
struct Agenda_Type {

   union agenda_u1_tag {
      
      double *alpha;
      /* A vector of length gNumStates representing the actual vector */

      Agenda head;
      /* for list headers */
   } u1;

   union agenda_u2_tag{

      int id;
      /* The ID of the vector in the Q-list */

      int length;
      /* for header elements to keep length of list */
   } u2;

   union agenda_u3_tag {

      Agenda next;
      /* for list elements */

      Agenda tail;
      /* for list headers */
   } u3;

   int last_obs;
   /*  Indicates where last search stopped */

   Alpha_List last_k; 
   /* for agenda list items, to indicate where last search stopped.  It
      is a pointer into the V_{t-1} alpha list */
  
   Alpha_List *choice;
   /* For agenda elements to remember the previous alpha vectors that
      were used to construct it.  It is an array of pointers into
      the V_{t-1} list and are needed when setting up LPs */

};

/***** External Functions *****/

extern void initAgenda( Agenda list );
extern void destroyAgendaItem( Agenda item );
extern void enqueueAgendaItem( Agenda agenda, Agenda item );
extern Agenda dequeueAgendaItem( Agenda agenda );
extern void addAgenda( Agenda agenda, double *alpha, 
                      Alpha_List *choice, int id );
extern void displayAgenda( FILE *file, Agenda agenda );
extern void showAgenda( Agenda agenda );
