
/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    agenda-orig.h
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
 *    $RCSfile: agenda-orig.h,v $
 *    $Source: /u/cvs/proj/pomdp-solve/src/agenda-orig.h,v $
 *    $Revision: 1.1 $
 *    $Date: 2003/05/13 21:46:39 $
 *  </RCS_KEYWORD>
 *
 *  <COPYRIGHT>
 *
 *
 *    All Rights Reserved
 *                          
 *    Permission to use, copy, modify, and distribute this software and its
 *    documentation for any purpose other than its incorporation into a
 *    commercial product is hereby granted without fee, provided that the
 *    above copyright notice appear in all copies and that both that
 *    copyright notice and this permission notice appear in supporting
 *    documentation.
 * 
 *    ANTHONY CASSANDRA DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 *    INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY
 *    PARTICULAR PURPOSE.  IN NO EVENT SHALL ANTHONY CASSANDRA BE LIABLE FOR
 *    ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *  </COPYRIGHT>
 *
 *</SOURCE_HEADER>
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
