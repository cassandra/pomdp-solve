/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20220114

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"
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
#include <stdlib.h>
#include <stdio.h>

#include "mdp-common.h"
#include "parse_err.h"
#include "mdp.h"
#include "parse_hash.h"
#include "parse_constant.h"
#include "sparse-matrix.h"
#include "imm-reward.h"

#define YACCtrace(X)       /*   printf(X);fflush(stdout)    */ 

/* When reading in matrices we need to know what type we are reading
   and also we need to keep track of where in the matrix we are
   including how to update the row and col after each entry is read. */
typedef enum { mc_none, mc_trans_single, mc_trans_row, mc_trans_all,
               mc_obs_single, mc_obs_row, mc_obs_all,
               mc_reward_single, mc_reward_row, 
               mc_reward_all, mc_reward_mdp_only,
               mc_start_belief, mc_mdp_start, 
               mc_start_include, mc_start_exclude } Matrix_Context;

extern int yylex();

/* Forward declaration for action routines which appear at end of file */
void yyerror(char *string);
void checkMatrix();
void enterString( Constant_Block *block );
void enterUniformMatrix( );
void enterIdentityMatrix( );
void enterResetMatrix( );
void enterMatrix( double value );
void setMatrixContext( Matrix_Context context, 
                      int a, int i, int j, int obs );
void enterStartState( int i );
void setStartStateUniform();
void endStartStates();
void verifyPreamble();
void checkProbs();

/*  Helps to give more meaningful error messages */
long currentLineNumber = 1;

/* This sets the context needed when names are given the the states, 
   actions and/or observations */
Mnemonic_Type curMnemonic = nt_unknown;

Matrix_Context curMatrixContext = mc_none;

/* These variable are used to keep track what type of matrix is being entered and
   which element is currently being processed.  They are initialized by the
   setMatrixContext() routine and updated by the enterMatrix() routine. */
int curRow;
int curCol;
int minA, maxA;
int minI, maxI;
int minJ, maxJ;
int minObs, maxObs;

extern I_Matrix *IP;   /* For transition matrices. */
extern I_Matrix *IR;   /* For observation matrices. */

/* These variables are used by the parser only, to keep some state
   information. 
*/
/* These are set when the appropriate preamble line is encountered.  This will
   allow us to check to make sure each is specified.  If observations are not
   defined then we will assume it is a regular MDP, and otherwise assume it 
   is a POMDP
   */
int discountDefined = 0;
int valuesDefined = 0;
int statesDefined = 0;
int actionsDefined = 0;
int observationsDefined = 0;

/* We only want to check when observation probs. are specified, but
   there was no observations in preamble. */
int observationSpecDefined = 0;

/* When we encounter a matrix with too many entries.  We would like
   to only generate one error message, instead of one for each entry.
   This variable is cleared at the start of reading  a matrix and
   set when there are too many entries. */
int gTooManyEntries = 0;

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 111 "parser.y"
typedef union YYSTYPE {
  Constant_Block *constBlk;
  int i_num;
  double f_num;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 136 "parser.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define INTTOK 1
#define FLOATTOK 2
#define COLONTOK 3
#define MINUSTOK 4
#define PLUSTOK 5
#define STRINGTOK 6
#define ASTERICKTOK 7
#define DISCOUNTTOK 8
#define VALUESTOK 9
#define STATETOK 10
#define ACTIONTOK 11
#define OBSTOK 12
#define TTOK 13
#define OTOK 14
#define RTOK 15
#define UNIFORMTOK 16
#define IDENTITYTOK 17
#define REWARDTOK 18
#define COSTTOK 19
#define RESETTOK 20
#define STARTTOK 21
#define INCLUDETOK 22
#define EXCLUDETOK 23
#define EOFTOK 258
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    9,   11,    0,    7,    7,   12,   12,   12,   12,   12,
   13,   14,   18,   18,   20,   15,   19,   19,   23,   16,
   22,   22,   25,   17,   24,   24,   27,    8,    8,   29,
    8,   30,    8,    8,   28,   28,   10,   10,   31,   31,
   31,   32,   36,   35,   37,   35,   39,   35,   33,   41,
   40,   42,   40,   43,   40,   34,   45,   44,   47,   44,
   48,   44,   49,   44,   38,   38,   38,   26,   26,   26,
   50,   50,   46,   46,    2,    2,    2,    1,    1,    1,
    3,    3,    3,   21,   21,    6,    6,    5,    5,    4,
    4,    4,
};
static const YYINT yylen[] = {                            2,
    0,    0,    5,    2,    0,    1,    1,    1,    1,    1,
    3,    3,    1,    1,    0,    4,    1,    1,    0,    4,
    1,    1,    0,    4,    1,    1,    0,    4,    3,    0,
    5,    0,    5,    0,    2,    1,    2,    0,    1,    1,
    1,    3,    0,    7,    0,    5,    0,    3,    3,    0,
    7,    0,    5,    0,    3,    3,    0,    9,    0,    7,
    0,    5,    0,    3,    1,    1,    1,    1,    1,    1,
    2,    1,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    1,    1,    1,    2,    2,    1,
    1,    0,
};
static const YYINT yydefred[] = {                         5,
    0,    0,    0,    0,    0,    0,    0,    0,    4,    6,
    7,    8,    9,   10,    0,    0,   15,   19,   23,    0,
    2,   91,   90,    0,   11,   13,   14,   12,    0,    0,
    0,    0,    0,    0,   38,   88,   89,   17,   85,   16,
    0,   21,    0,   20,   25,    0,   24,   29,    0,   30,
   32,    0,   84,   86,   87,   68,   69,   72,   28,    0,
    0,    0,    0,    0,    0,   37,   39,   40,   41,   71,
   75,   76,   77,   36,    0,    0,    0,    0,    0,   35,
   78,   79,   80,    0,   42,    0,   49,    0,   56,    0,
    0,    0,    0,    0,    0,    0,   65,   66,   48,    0,
    0,   55,    0,   74,    0,    0,    0,    0,    0,    0,
    0,   73,   43,   46,   81,   82,   83,   50,   53,    0,
    0,    0,    0,    0,    0,   44,   51,   57,    0,    0,
   58,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  260,  267,    8,    9,   10,   11,   12,  269,  272,  273,
  274,  275,  276,  277,    3,    3,    3,    3,    3,   21,
  268,    4,    5,  264,  265,   18,   19,  278,  280,  283,
  285,    3,   22,   23,  271,    1,    2,    1,    6,  279,
  281,    1,  281,  282,    1,  281,  284,    6,  287,    3,
    3,  270,    6,    1,    2,   16,   20,  266,  286,  310,
  289,  290,   13,   14,   15,  291,  292,  293,  294,  266,
    1,    6,    7,  262,  288,  288,    3,    3,    3,  262,
    1,    6,    7,  261,  295,  261,  300,  261,  304,    3,
  299,    3,  303,    3,  309,  262,   16,   17,  298,  310,
  262,  286,  262,  265,  306,    3,  297,    3,  302,    3,
  308,  265,  262,  286,    1,    6,    7,  263,  286,  262,
  306,  296,  301,    3,  307,  266,  266,  263,  306,  305,
  265,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          1,
   84,   74,  118,   24,  104,   58,    2,   21,    8,   52,
   35,    9,   10,   11,   12,   13,   14,   28,   40,   29,
   41,   44,   30,   47,   31,   59,   49,   75,   61,   62,
   66,   67,   68,   69,   85,  122,  107,   99,   91,   87,
  123,  109,   93,   89,  130,  105,  125,  111,   95,   60,
};
static const YYINT yysindex[] = {                         0,
    0,  156,   29,   51,   53,   55,   84,   41,    0,    0,
    0,    0,    0,    0,   35,  164,    0,    0,    0,    8,
    0,    0,    0,  183,    0,    0,    0,    0,  137,  152,
  153,   83,   94,   96,    0,    0,    0,    0,    0,    0,
   95,    0,   95,    0,    0,   95,    0,    0,   59,    0,
    0,  166,    0,    0,    0,    0,    0,    0,    0,  185,
    6,    6,  104,  114,  134,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    6,    6,    9,    9,    9,    0,
    0,    0,    0,  141,    0,  149,    0,  157,    0,    6,
   92,    6,   59,    6,   35,  191,    0,    0,    0,  185,
  192,    0,  193,    0,   35,    6,   59,   79,   59,    6,
   35,    0,    0,    0,    0,    0,    0,    0,    0,  194,
   35,  185,  185,   79,   35,    0,    0,    0,   35,   35,
    0,
};
static const YYINT yyrindex[] = {                         0,
    0,   38,    0,    0,    0,    0,    0,  116,    0,    0,
    0,    0,    0,    0,  187,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   72,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,   34,    0,    0,   57,    0,    0,    0,    0,
    0,  105,    0,    0,    0,    0,    0,    0,    0,  132,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  136,  142,    0,    0,    0,    0,
    0,    0,    0,  102,    0,   75,    0,    4,    0,    0,
    0,    0,    0,    0,  187,   80,    0,    0,    0,  148,
   82,    0,  168,    0,  110,    0,    0,    0,    0,    0,
  187,    0,    0,    0,    0,    0,    0,    0,    0,  173,
  120,    0,    0,    0,  187,    0,    0,    0,  126,  187,
    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#endif
static const YYINT yygindex[] = {                         0,
  112,  -73,  -11,    0,  -15,  -59,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  162,    0,    0,    0,    0,  -89,    0,  109,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -75,    0,    0,    0,   85,
};
#define YYTABLESIZE 197
static const YYINT yytable[] = {                         25,
   70,   80,   80,  102,   63,   63,   71,   63,   63,   81,
   32,   72,   73,   18,   82,   83,   96,  114,  101,  119,
  103,   18,   18,   18,   18,   18,   18,   18,   18,   33,
   34,   15,  113,   22,   18,  121,  120,    1,   22,   23,
   70,   22,   22,   22,   22,   22,   22,   22,   22,  129,
    1,    1,    1,   16,   22,   17,   26,   18,    1,   54,
   55,   20,  126,  127,   26,   26,   26,   26,   26,   26,
   26,   26,   27,   27,   56,   54,   54,   26,   57,  115,
   45,   45,   52,   52,  116,  117,   19,   27,   48,  112,
   54,   27,   54,   55,   54,   45,   50,   52,   51,   45,
   53,   52,   47,   47,    3,  112,   77,   97,   98,   64,
   92,   92,  128,  112,  131,   34,   78,   47,   47,   62,
   92,   92,   64,   64,   64,   60,   92,   92,   34,   34,
   34,   70,   62,   62,   62,   31,   79,   38,   60,   60,
   60,   33,   39,   90,   70,   70,   70,   67,   31,   31,
   31,   92,   42,   45,   33,   33,   33,   39,   39,   94,
   67,   67,   67,    3,    4,    5,    6,    7,   61,   61,
   76,   61,   61,   59,   59,  100,   59,   59,   63,   64,
   65,   26,   27,   36,   37,   54,   55,   92,   92,   86,
   88,   43,   46,  106,  108,  110,  124,
};
static const YYINT yycheck[] = {                         15,
   60,   75,   76,   93,    1,    2,    1,    4,    5,    1,
    3,    6,    7,    0,    6,    7,   90,  107,   92,  109,
   94,    8,    9,   10,   11,   12,   13,   14,   15,   22,
   23,    3,  106,    0,   21,  111,  110,    0,    4,    5,
  100,    8,    9,   10,   11,   12,   13,   14,   15,  125,
   13,   14,   15,    3,   21,    3,    0,    3,   21,    1,
    2,   21,  122,  123,    8,    9,   10,   11,   12,   13,
   14,   15,    1,    2,   16,    1,    2,   21,   20,    1,
    1,    2,    1,    2,    6,    7,    3,   16,    6,  105,
   16,   20,    1,    2,   20,   16,    3,   16,    3,   20,
    6,   20,    1,    2,    0,  121,    3,   16,   17,    0,
    1,    2,  124,  129,  130,    0,    3,   16,   17,    0,
    1,    2,   13,   14,   15,    0,    1,    2,   13,   14,
   15,    0,   13,   14,   15,    0,    3,    1,   13,   14,
   15,    0,    6,    3,   13,   14,   15,    0,   13,   14,
   15,    3,    1,    1,   13,   14,   15,    6,    6,    3,
   13,   14,   15,    8,    9,   10,   11,   12,    1,    2,
   62,    4,    5,    1,    2,   91,    4,    5,   13,   14,
   15,   18,   19,    1,    2,    1,    2,    1,    2,   78,
   79,   30,   31,    3,    3,    3,    3,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 258
#define YYUNDFTOKEN 311
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end","INTTOK","FLOATTOK","COLONTOK","MINUSTOK","PLUSTOK","STRINGTOK",
"ASTERICKTOK","DISCOUNTTOK","VALUESTOK","STATETOK","ACTIONTOK","OBSTOK","TTOK",
"OTOK","RTOK","UNIFORMTOK","IDENTITYTOK","REWARDTOK","COSTTOK","RESETTOK",
"STARTTOK","INCLUDETOK","EXCLUDETOK",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,"error",0,"EOFTOK","$accept","pomdp_file","action",
"state","obs","optional_sign","number","prob","preamble","start_state","$$1",
"param_list","$$2","param_type","discount_param","value_param","state_param",
"action_param","obs_param","value_tail","state_tail","$$3","ident_list",
"action_tail","$$4","obs_param_tail","$$5","u_matrix","$$6","start_state_list",
"$$7","$$8","param_spec","trans_prob_spec","obs_prob_spec","reward_spec",
"trans_spec_tail","$$9","$$10","ui_matrix","$$11","obs_spec_tail","$$12","$$13",
"$$14","reward_spec_tail","$$15","num_matrix","$$16","$$17","$$18",
"prob_matrix","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : pomdp_file",
"$$1 :",
"$$2 :",
"pomdp_file : preamble $$1 start_state $$2 param_list",
"preamble : preamble param_type",
"preamble :",
"param_type : discount_param",
"param_type : value_param",
"param_type : state_param",
"param_type : action_param",
"param_type : obs_param",
"discount_param : DISCOUNTTOK COLONTOK number",
"value_param : VALUESTOK COLONTOK value_tail",
"value_tail : REWARDTOK",
"value_tail : COSTTOK",
"$$3 :",
"state_param : STATETOK COLONTOK $$3 state_tail",
"state_tail : INTTOK",
"state_tail : ident_list",
"$$4 :",
"action_param : ACTIONTOK COLONTOK $$4 action_tail",
"action_tail : INTTOK",
"action_tail : ident_list",
"$$5 :",
"obs_param : OBSTOK COLONTOK $$5 obs_param_tail",
"obs_param_tail : INTTOK",
"obs_param_tail : ident_list",
"$$6 :",
"start_state : STARTTOK COLONTOK $$6 u_matrix",
"start_state : STARTTOK COLONTOK STRINGTOK",
"$$7 :",
"start_state : STARTTOK INCLUDETOK COLONTOK $$7 start_state_list",
"$$8 :",
"start_state : STARTTOK EXCLUDETOK COLONTOK $$8 start_state_list",
"start_state :",
"start_state_list : start_state_list state",
"start_state_list : state",
"param_list : param_list param_spec",
"param_list :",
"param_spec : trans_prob_spec",
"param_spec : obs_prob_spec",
"param_spec : reward_spec",
"trans_prob_spec : TTOK COLONTOK trans_spec_tail",
"$$9 :",
"trans_spec_tail : action COLONTOK state COLONTOK state $$9 prob",
"$$10 :",
"trans_spec_tail : action COLONTOK state $$10 u_matrix",
"$$11 :",
"trans_spec_tail : action $$11 ui_matrix",
"obs_prob_spec : OTOK COLONTOK obs_spec_tail",
"$$12 :",
"obs_spec_tail : action COLONTOK state COLONTOK obs $$12 prob",
"$$13 :",
"obs_spec_tail : action COLONTOK state $$13 u_matrix",
"$$14 :",
"obs_spec_tail : action $$14 u_matrix",
"reward_spec : RTOK COLONTOK reward_spec_tail",
"$$15 :",
"reward_spec_tail : action COLONTOK state COLONTOK state COLONTOK obs $$15 number",
"$$16 :",
"reward_spec_tail : action COLONTOK state COLONTOK state $$16 num_matrix",
"$$17 :",
"reward_spec_tail : action COLONTOK state $$17 num_matrix",
"$$18 :",
"reward_spec_tail : action $$18 num_matrix",
"ui_matrix : UNIFORMTOK",
"ui_matrix : IDENTITYTOK",
"ui_matrix : prob_matrix",
"u_matrix : UNIFORMTOK",
"u_matrix : RESETTOK",
"u_matrix : prob_matrix",
"prob_matrix : prob_matrix prob",
"prob_matrix : prob",
"num_matrix : num_matrix number",
"num_matrix : number",
"state : INTTOK",
"state : STRINGTOK",
"state : ASTERICKTOK",
"action : INTTOK",
"action : STRINGTOK",
"action : ASTERICKTOK",
"obs : INTTOK",
"obs : STRINGTOK",
"obs : ASTERICKTOK",
"ident_list : ident_list STRINGTOK",
"ident_list : STRINGTOK",
"prob : INTTOK",
"prob : FLOATTOK",
"number : optional_sign INTTOK",
"number : optional_sign FLOATTOK",
"optional_sign : PLUSTOK",
"optional_sign : MINUSTOK",
"optional_sign :",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 739 "parser.y"

/********************************************************************/
/*              External Routines                                   */
/********************************************************************/

#define EPSILON  0.00001  /* tolerance for sum of probs == 1 */

Constant_Block *aConst;

/******************************************************************************/
void 
yyerror(char *string)
{
   ERR_enter("Parser<yyparse>", currentLineNumber, PARSE_ERR,"");
}  /* yyerror */
/******************************************************************************/
void 
checkMatrix() {
/* When a matrix is finished being read for the exactly correct number of
   values, curRow should be 0 and curCol should be -1.  For the cases
   where we are only interested in a row of entries curCol should be -1.
   If we get too many entries, then we will catch this as we parse the 
   extra entries.  Therefore, here we only need to check for too few 
   entries.
   */

   switch( curMatrixContext ) {
   case mc_trans_row:
      if( curCol < gNumStates )
         ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
                   TOO_FEW_ENTRIES, "");
      break;
   case mc_trans_all:
      if((curRow < (gNumStates-1) )
	 || ((curRow == (gNumStates-1))
	     && ( curCol < gNumStates ))) 
	ERR_enter("Parser<checkMatrix>:", currentLineNumber,  
                   TOO_FEW_ENTRIES, "" );
      break;
   case mc_obs_row:
      if( curCol < gNumObservations )
         ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
                   TOO_FEW_ENTRIES, "");
      break;
   case mc_obs_all:
      if((curRow < (gNumStates-1) )
	 || ((curRow == (gNumStates-1))
	     && ( curCol < gNumObservations ))) 
         ERR_enter("Parser<checkMatrix>:", currentLineNumber,  
                   TOO_FEW_ENTRIES, "" );
      break;
   case mc_start_belief:
      if( curCol < gNumStates )
	ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
		  TOO_FEW_ENTRIES, "");
      break;

    case mc_mdp_start:
      /* We will check for invalid multiple entries for MDP in 
	 enterMatrix() */
      break;

    case mc_reward_row:
      if( gProblemType == POMDP_problem_type )
	if( curCol < gNumObservations )
	  ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
		    TOO_FEW_ENTRIES, "");
      break;

    case mc_reward_all:
      if( gProblemType == POMDP_problem_type ) {
	if((curRow < (gNumStates-1) )
	   || ((curRow == (gNumStates-1))
	       && ( curCol < gNumObservations ))) 
	  ERR_enter("Parser<checkMatrix>:", currentLineNumber,  
		    TOO_FEW_ENTRIES, "" );
      }
      else
	if( curCol < gNumStates )
	  ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
		    TOO_FEW_ENTRIES, "");
      
      break;
    case mc_reward_single:
      /* Don't need to do anything */
      break;

    case mc_reward_mdp_only:
      if((curRow < (gNumStates-1) )
	 || ((curRow == (gNumStates-1))
	     && ( curCol < gNumStates ))) 
	ERR_enter("Parser<checkMatrix>:", currentLineNumber,  
		  TOO_FEW_ENTRIES, "" );
      break;

   default:
      ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
                BAD_MATRIX_CONTEXT, "" );
      break;
   }  /* switch */

   if( gTooManyEntries )
     ERR_enter("Parser<checkMatrix>:", currentLineNumber, 
	       TOO_MANY_ENTRIES, "" );

   /* After reading a line for immediate rewards for a pomdp, we must tell
      the data structures for the special representation that we are done */
   switch( curMatrixContext ) {
   case mc_reward_row:
   case mc_reward_all:
   case mc_reward_mdp_only:
     doneImmReward();
     break;

     /* This case is only valid for POMDPs, so if we have an MDP, we
	never would have started a new immediate reward, so calling 
	the doneImmReward will be in error.  */
   case mc_reward_single:
     if( gProblemType == POMDP_problem_type )
       doneImmReward();
     break;
   default:
     break;
   }  /* switch */
   

   curMatrixContext = mc_none;  /* reset this as a safety precaution */
}  /* checkMatrix */
/******************************************************************************/
void 
enterString( Constant_Block *block ) {
   
   if( H_enter( block->theValue.theString, curMnemonic ) == 0 )
      ERR_enter("Parser<enterString>:", currentLineNumber, 
                DUPLICATE_STRING, block->theValue.theString );

   XFREE( block->theValue.theString );
   XFREE( block );
}  /* enterString */
/******************************************************************************/
void 
enterUniformMatrix( ) {
   int a, i, j, obs;
   double prob;

   switch( curMatrixContext ) {
   case mc_trans_row:
      prob = 1.0/gNumStates;
      for( a = minA; a <= maxA; a++ )
         for( i = minI; i <= maxI; i++ )
            for( j = 0; j < gNumStates; j++ )
	       addEntryToIMatrix( IP[a], i, j, prob );
      break;
   case mc_trans_all:
      prob = 1.0/gNumStates;
      for( a = minA; a <= maxA; a++ )
         for( i = 0; i < gNumStates; i++ )
            for( j = 0; j < gNumStates; j++ )
 	       addEntryToIMatrix( IP[a], i, j, prob );
      break;
   case mc_obs_row:
      prob = 1.0/gNumObservations;
      for( a = minA; a <= maxA; a++ )
         for( j = minJ; j <= maxJ; j++ )
            for( obs = 0; obs < gNumObservations; obs++ )
 	       addEntryToIMatrix( IR[a], j, obs, prob );
      break;
   case mc_obs_all:
      prob = 1.0/gNumObservations;
      for( a = minA; a <= maxA; a++ )
         for( j = 0; j < gNumStates; j++ )
            for( obs = 0; obs < gNumObservations; obs++ )
 	       addEntryToIMatrix( IR[a], j, obs, prob );
      break;
   case mc_start_belief:
      setStartStateUniform();
      break;
   case mc_mdp_start:
      /* This is meaning less for an MDP */
      ERR_enter("Parser<enterUniformMatrix>:", currentLineNumber, 
                BAD_START_STATE_TYPE, "" );
      break;
   default:
      ERR_enter("Parser<enterUniformMatrix>:", currentLineNumber, 
                BAD_MATRIX_CONTEXT, "" );
      break;
   }  /* switch */
}  /* enterUniformMatrix */
/******************************************************************************/
void 
enterIdentityMatrix( ) {
   int a, i,j;

   switch( curMatrixContext ) {
   case mc_trans_all:
      for( a = minA; a <= maxA; a++ )
         for( i = 0; i < gNumStates; i++ )
            for( j = 0; j < gNumStates; j++ )
               if( i == j )
		 addEntryToIMatrix( IP[a], i, j, 1.0 );
               else
		 addEntryToIMatrix( IP[a], i, j, 0.0 );
      break;
   default:
      ERR_enter("Parser<enterIdentityMatrix>:", currentLineNumber, 
                BAD_MATRIX_CONTEXT, "" );
      break;
   }  /* switch */
}  /* enterIdentityMatrix */
/******************************************************************************/
void 
enterResetMatrix( ) {
  int a, i, j;

  if( curMatrixContext != mc_trans_row ) {
    ERR_enter("Parser<enterMatrix>:", currentLineNumber, 
	      BAD_RESET_USAGE, "" );
    return;
  }

  if( gProblemType == POMDP_problem_type )
    for( a = minA; a <= maxA; a++ )
      for( i = minI; i <= maxI; i++ )
	for( j = 0; j < gNumStates; j++ )
	  addEntryToIMatrix( IP[a], i, j, gInitialBelief[j] );
  
  else  /* It is an MDP */
    for( a = minA; a <= maxA; a++ )
      for( i = minI; i <= maxI; i++ )
	addEntryToIMatrix( IP[a], i, gInitialState, 1.0 );
  

}  /* enterResetMatrix */
/******************************************************************************/
void 
enterMatrix( double value ) {
/*
  For the '_single' context types we never have to worry about setting or 
  checking the bounds on the current row or col.  For all other we do and
  how this is done depends on the context.  Notice that we are filling in the 
  elements in reverse order due to the left-recursive grammar.  Thus
  we need to update the col and row backwards 
  */
   int a, i, j, obs;

   switch( curMatrixContext ) {
   case mc_trans_single:
      for( a = minA; a <= maxA; a++ )
         for( i = minI; i <= maxI; i++ )
            for( j = minJ; j <= maxJ; j++ )
	      addEntryToIMatrix( IP[a], i, j, value );
      break;
   case mc_trans_row:
      if( curCol < gNumStates ) {
         for( a = minA; a <= maxA; a++ )
            for( i = minI; i <= maxI; i++ )
	      addEntryToIMatrix( IP[a], i, curCol, value );
         curCol++;
      }
      else
	gTooManyEntries = 1;

      break;
   case mc_trans_all:
      if( curCol >= gNumStates ) {
         curRow++;
         curCol = 0;;
      }

      if( curRow < gNumStates ) {
         for( a = minA; a <= maxA; a++ )
	   addEntryToIMatrix( IP[a], curRow, curCol, value );
         curCol++;
      }
      else
	gTooManyEntries = 1;

      break;

   case mc_obs_single:

      if( gProblemType == POMDP_problem_type )
	/* We ignore this if it is an MDP */

	for( a = minA; a <= maxA; a++ )
	  for( j = minJ; j <= maxJ; j++ )
            for( obs = minObs; obs <= maxObs; obs++ )
	      addEntryToIMatrix( IR[a], j, obs, value );
      break;

   case mc_obs_row:
	if( gProblemType == POMDP_problem_type ) {
	  /* We ignore this if it is an MDP */
	  
	  if( curCol < gNumObservations ) {
	    
	    for( a = minA; a <= maxA; a++ )
		 for( j = minJ; j <= maxJ; j++ )
		   addEntryToIMatrix( IR[a], j, curCol, value );
	    
	    curCol++;
	  }
	  else {
	    gTooManyEntries = 1;
	  }
	}
	break;

   case mc_obs_all:
      if( curCol >= gNumObservations ) {
         curRow++;
         curCol = 0;
      }

      if( gProblemType == POMDP_problem_type ) {
	   /* We ignore this if it is an MDP */

	   if( curRow < gNumStates ) {
		for( a = minA; a <= maxA; a++ )
		  addEntryToIMatrix( IR[a], curRow, curCol, value );
		
		curCol++;
	   }
	   else {
		gTooManyEntries = 1;
	   }
	 }
      break;

/* This is a special case for POMDPs, since we need a special 
   representation for immediate rewards for POMDP's.  Note that this 
   is not valid syntax for an MDP, but we flag this error when we set 
   the matrix context, so we ignore the MDP case here.
   */
   case mc_reward_single:
      if( gProblemType == POMDP_problem_type ) {

	if( curCol == 0 ) {
	  enterImmReward( 0, 0, 0, value );
	  curCol++;
	}
	else
	  gTooManyEntries = 1;

      }
     break;

    case mc_reward_row:
      if( gProblemType == POMDP_problem_type ) {

	/* This is a special case for POMDPs, since we need a special 
	   representation for immediate rewards for POMDP's */
   
	if( curCol < gNumObservations ) {
	  enterImmReward( 0, 0, curCol, value );
	  curCol++;
	}
	else
	  gTooManyEntries = 1;

      }  /* if POMDP problem */

      else /* we are dealing with an MDP, so there should only be 
	      a single entry */
	if( curCol == 0 ) {
	  enterImmReward( 0, 0, 0, value );
	  curCol++;
	}
	else
	  gTooManyEntries = 1;


     break;

   case mc_reward_all:

      /* This is a special case for POMDPs, since we need a special 
	 representation for immediate rewards for POMDP's */

      if( gProblemType == POMDP_problem_type ) {
	if( curCol >= gNumObservations ) {
	  curRow++;
	  curCol = 0;
	}
	if( curRow < gNumStates ) {
	  enterImmReward( 0, curRow, curCol, value );
	  curCol++;
	}
	else
	  gTooManyEntries = 1;

      }  /* If POMDP problem */

      /* Otherwise it is an MDP and we should be expecting an entire
	 row of rewards. */

      else  /* MDP */
	if( curCol < gNumStates ) {
	  enterImmReward( 0, curCol, 0, value );
	  curCol++;
	}
	else
	  gTooManyEntries = 1;

      break;

      /* This is a special case for an MDP only where we specify
	 the entire matrix of rewards. If we are erroneously 
	 definining a POMDP, this error will be flagged in the 
	 setMatrixContext() routine.
	 */

    case mc_reward_mdp_only:
      if( gProblemType == MDP_problem_type ) {
	if( curCol >= gNumStates ) {
	  curRow++;
	  curCol = 0;
	}
	if( curRow < gNumStates ) {
	  enterImmReward( curRow, curCol, 0, value );
	  curCol++;
	}
	else
	  gTooManyEntries = 1;

      }
      break;

    case mc_mdp_start:

      /* For an MDP we only want to see a single value and */
      /* we want it to correspond to a valid state number. */

      if( curCol > 0 )
	gTooManyEntries = 1;

      else {
	gInitialState = value;
	curCol++;
      }
      break;
	  
   case mc_start_belief:

      /* This will process the individual entries when a starting */
      /* belief state is fully specified.  When it is a POMDP, we need */
      /* an entry for each state, so we keep the curCol variable */
      /* updated.  */

      if( curCol < gNumStates ) {
	gInitialBelief[curCol] = value;
	curCol++;
      }
      else
	gTooManyEntries = 1;

      break;

   default:
      ERR_enter("Parser<enterMatrix>:", currentLineNumber, 
                BAD_MATRIX_CONTEXT, "");
      break;
   }  /* switch */

}  /* enterMatrix */
/******************************************************************************/
void 
setMatrixContext( Matrix_Context context, 
		  int a, int i, int j, int obs ) {
/* 
   Note that we must enter the matrix entries in reverse order because
   the matrices are defined with left-recursive rules.  Set the a, i,
   and j parameters to be less than zero when you want to define it
   for all possible values.  

   Rewards for MDPs and POMDPs differ since in the former, rewards are not
   based upon an observations.  This complicates things since not only is one 
   of the reward syntax options not valid, but the semantics of all the
   rewards change as well.  I have chosen to handle this in this routine.  
   I will check for the appropriate type and set the context to handle the
   proper amount of entries.
*/
  int state;

   curMatrixContext = context;
   gTooManyEntries = 0;  /* Clear this out before reading any */

   curRow = 0;  /* This is ignored for some contexts */
   curCol = 0;

   switch( curMatrixContext ) {

   mc_start_belief:
     
     break;

   case mc_start_include:

     /* When we specify the starting belief state as a list of states */
     /* to include, we initialize all state to 0.0, since as we read */
     /* the states we will set that particular value to 1.0.  After it */
     /* is all done we can then just normalize the belief state */

     if( gProblemType == POMDP_problem_type )
       for( state = 0; state < gNumStates; state++ )
	 gInitialBelief[state] = 0.0;

     else  /* It is an MDP which is not valid */
       ERR_enter("Parser<setMatrixContext>:", currentLineNumber, 
		 BAD_START_STATE_TYPE, "");
      
     break;

   case mc_start_exclude:

     /* When we are specifying the starting belief state as a a list */
     /* of states, we initialize all states to 1.0 and as we read each */
     /* in the list we clear it out to be zero.  fter it */
     /* is all done we can then just normalize the belief state */

     if( gProblemType == POMDP_problem_type )
       for( state = 0; state < gNumStates; state++ )
	 gInitialBelief[state] = 1.0;

     else  /* It is an MDP which is not valid */
       ERR_enter("Parser<setMatrixContext>:", currentLineNumber, 
		 BAD_START_STATE_TYPE, "");

     break;

  /* We need a special representation for the immediate rewards.
     These four cases initialize the data structure that will be
     needed for immediate rewards by calling newImmReward.  Note that
     the arguments will differe depending upon whether it is an
     MDP or POMDP.
     */
  case mc_reward_mdp_only:
    if( gProblemType == POMDP_problem_type )  {
       ERR_enter("Parser<setMatrixContext>:", currentLineNumber, 
		 BAD_REWARD_SYNTAX, "");
    }
    else {
      newImmReward( a, NOT_PRESENT, NOT_PRESENT, 0 );
    } 
    break;
 
  case mc_reward_all:	
    if( gProblemType == POMDP_problem_type ) 
      newImmReward( a, i, NOT_PRESENT, NOT_PRESENT );

    else {
      newImmReward( a, i, NOT_PRESENT, 0 );
    }
    break;
  case mc_reward_row:
    if( gProblemType == POMDP_problem_type ) 
      newImmReward( a, i, j, NOT_PRESENT );
    
    else {
      newImmReward( a, i, j, 0 );
    } 
    break;
  case mc_reward_single:

    if( gProblemType == MDP_problem_type ) {
       ERR_enter("Parser<setMatrixContext>:", currentLineNumber, 
		 BAD_REWARD_SYNTAX, "");
    }
    else {
       newImmReward( a, i, j, obs );
     }
    break;

   default:
     break;
   }

  /* These variable settings will define the range over which the current 
     matrix context will have effect.  This accounts for wildcards by
     setting the range to include everything.  When a single entry was
     specified, the range is that single number.  When we actually 
     start to read the matrix, each entry we see will apply for the
     entire range specified, though for specific entries the range 
     will be a single number.
     */
   if( a < 0 ) {
      minA = 0;
      maxA = gNumActions - 1;
   }
   else
      minA = maxA = a;

   if( i < 0 ) {
      minI = 0;
      maxI = gNumStates - 1;
   }
   else
      minI = maxI = i;

   if( j < 0 ) {
      minJ = 0;
      maxJ = gNumStates - 1;
   }
   else
      minJ = maxJ = j;

   if( obs < 0 ) {
      minObs = 0;
      maxObs = gNumObservations - 1;
   }
   else
      minObs = maxObs = obs;

}  /* setMatrixContext */
/******************************************************************************/
void 
enterStartState( int i ) {
/*
   This is not valid for an MDP, but the error has already been flagged
   in the setMatrixContext() routine.  Therefore, if just igore this if 
   it is an MDP.
*/

  if( gProblemType == MDP_problem_type )
    return;

  switch( curMatrixContext ) {
  case mc_start_include:
    gInitialBelief[i] = 1.0;
    break;
  case mc_start_exclude:
    gInitialBelief[i] = 0.0;
    break;
  default:
    ERR_enter("Parser<enterStartState>:", currentLineNumber, 
	      BAD_MATRIX_CONTEXT, "");
      break;
  } /* switch */
}  /* enterStartState */
/******************************************************************************/
void 
setStartStateUniform() {
  int i;
  double prob;

  if( gProblemType != POMDP_problem_type )
    return;

  prob = 1.0/gNumStates;
  for( i = 0; i < gNumStates; i++ )
    gInitialBelief[i] = prob;

}  /*  setStartStateUniform*/
/******************************************************************************/
void 
endStartStates() {
/*
   There are a few cases where the matrix context will not be
   set at this point.  When there is a list of probabilities
   or if it is an MDP the context will have been cleared.
   */
  int i;
  double prob;

  if( gProblemType == MDP_problem_type ) {
    curMatrixContext = mc_none;  /* just to be sure */
    return;
  }
    
  switch( curMatrixContext ) {
  case mc_start_include:
  case mc_start_exclude:
    /* At this point gInitialBelief should be a vector of 1.0's and 0.0's
       being set as each is either included or excluded.  Now we need to
       normalized them to make it a true probability distribution */
    prob = 0.0;
    for( i = 0; i < gNumStates; i++ )
      prob += gInitialBelief[i];
    if( prob <= 0.0 ) {
      ERR_enter("Parser<endStartStates>:", currentLineNumber, 
                BAD_START_PROB_SUM, "" );
      return;
    }
    for( i = 0; i < gNumStates; i++ )
      gInitialBelief[i] /= prob;
    break;

  default:  /* Make sure we have a valid prob. distribution */
    prob = 0.0;
    for( i = 0; i < gNumStates; i++ ) 
      prob += gInitialBelief[i];
    if((prob < ( 1.0 - EPSILON)) || (prob > (1.0 + EPSILON))) {
      ERR_enter("Parser<endStartStates>:", NO_LINE, 
		BAD_START_PROB_SUM, "" );
    }
    break;
  }  /* switch */

  curMatrixContext = mc_none;

}  /* endStartStates */
/******************************************************************************/
void 
verifyPreamble() {
/* 
   When a param is not defined, set these to non-zero so parsing can
   proceed even in the absence of specifying these values.  When an
   out of range value is encountered the parser will flag the error,
   but return 0 so that more errors can be detected 
   */

   if( discountDefined == 0 )
      ERR_enter("Parser<verifyPreamble>:", currentLineNumber, 
                MISSING_DISCOUNT, "" );
   if( valuesDefined == 0 )
      ERR_enter("Parser<verifyPreamble>:", currentLineNumber,
                MISSING_VALUES, "" );
   if( statesDefined == 0 ) {
      ERR_enter("Parser<verifyPreamble>:", currentLineNumber, 
                MISSING_STATES, "" );
      gNumStates = 1;
   }
   if( actionsDefined == 0 ) {
      ERR_enter("Parser<verifyPreamble>:", currentLineNumber, 
                MISSING_ACTIONS, "" );
      gNumActions = 1;
   }

   /* If we do not see this, them we must be parsing an MDP */
   if( observationsDefined == 0 ) {
     gNumObservations = 0;
     gProblemType = MDP_problem_type;
   }

   else
     gProblemType = POMDP_problem_type;

}  /* verifyPreamble */
/******************************************************************************/
void 
checkProbs() {
   int a,i,j,obs;
   double sum;
   char str[512];

   
   for( a = 0; a < gNumActions; a++ )
      for( i = 0; i < gNumStates; i++ ) {
	 sum = sumIMatrixRowValues( IP[a], i );
         if((sum < ( 1.0 - EPSILON)) || (sum > (1.0 + EPSILON))) {
            sprintf( str, "action=%d, state=%d (%.5lf)", a, i, sum );
            ERR_enter("Parser<checkProbs>:", NO_LINE, 
                      BAD_TRANS_PROB_SUM, str );
         }
      } /* for i */

   if( gProblemType == POMDP_problem_type )
     for( a = 0; a < gNumActions; a++ )
       for( j = 0; j < gNumStates; j++ ) {
	 sum = sumIMatrixRowValues( IR[a], j );
         if((sum < ( 1.0 - EPSILON)) || (sum > (1.0 + EPSILON))) {
	   sprintf( str, "action=%d, state=%d (%.5lf)", a, j, sum );
	   ERR_enter("Parser<checkProbs>:", NO_LINE, 
		     BAD_OBS_PROB_SUM, str );
         } /* if sum not == 1 */
       }  /* for j */

   /* Now see if we had observation specs defined in an MDP */

   if( observationSpecDefined && (gProblemType == MDP_problem_type))
     ERR_enter("Parser<checkProbs>:", NO_LINE, 
	       OBS_IN_MDP_PROBLEM, "" );

}  /* checkProbs */
/************************************************************************/
void 
initParser() {
/*
   This routine will reset all the state variables used by the parser
   in case it will parse multiple files.
*/
   observationSpecDefined = 0;
   discountDefined = 0;
   valuesDefined = 0;
   statesDefined = 0;
   actionsDefined = 0;
   observationsDefined = 0;
   observationSpecDefined = 0;
   currentLineNumber = 1;
   curMnemonic = nt_unknown;
   curMatrixContext = mc_none;

}  /* initParser */
/************************************************************************/
int 
readMDPFile( FILE *file ) {
   int returnValue, dump_status;
   extern FILE *yyin;

   initParser();

   ERR_initialize();
   H_create();
   yyin = file;

   returnValue = yyparse();

   /* If there are syntax errors, then we have to do something if we 
      want to parse another file without restarting.  It seems that
      a syntax error bombs the code out, but leaves the file pointer
      at the place it bombed.  Thus, another call to yyparse() will
      pick up where it left off and not necessarily at the start of a 
      new file.

      Unfortunately, I do not know how to do this yet.
      */
   if (returnValue != 0) {
      printf("\nParameter file contains syntax errors!\n");
    }

   dump_status = ERR_dump();

   ERR_cleanUp();
   H_destroy();

   if (dump_status || returnValue ) 
      return( 0 );

   /* This is where intermediate matrix representation are
      converted into their final representation */
   convertMatrices();

   return( 1 );
}  /* readPomdpFile */
/************************************************************************/
int 
yywrap()
{
   return 1;
}
/************************************************************************/
#line 1486 "parser.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 124 "parser.y"
	{ 
		    /* The preamble is a section of the file which */
		    /* must come first and whcih contains some global */
		    /* properties of the MDP that the file */
		    /* specifies. (e.g., number of states).  The */
		    /* observations are optional and its presence or */
		    /* absence is what first tells the parser whether */
		    /* it is parsing an MDP or a POMDP. */

		    verifyPreamble();  /* make sure all things are */
				       /* defined */

		    /* While we parse we use an intermediate */
		    /* representation which will be converted to the */
		    /* sparse representation when we are finished */
		    /* parsing.  After the preamble we are ready to */
		    /* start filling in values and we know how big the */
		    /* problem is, so we allocate the space for the */
		    /* intermediate forms */

		    allocateIntermediateMDP();  
		  }
#line 2180 "parser.c"
break;
case 2:
#line 148 "parser.y"
	{ 
		    /* Some type of algorithms want a place to start */
		    /* off the problem, especially when doing */
		    /* simulation type experiments.  This is an */
		    /* optional argument that allows specification of */
		    /* this.   In a POMDP this is a belief state, but */
		    /* in an MDP this is a single state.  If none is */
		    /* specified for a POMDP, then the uniform */
		    /* distribution over all states is used.  If none */
		    /* is specified for an MDP, then random states */
		    /* will be assumed. */

		    endStartStates(); 
		  }
#line 2198 "parser.c"
break;
case 3:
#line 167 "parser.y"
	{
		    /* This is the very last thing we do while */
		    /* parsing.  Even though the file may conform to */
		    /* the syntax, the semantics of the problem */
		    /* specification requires probability */
		    /* distributions.  This routine will make sure */
		    /* that the appropriate things sum to 1.0 to make */
		    /* a valid probability distribution. This will */
		    /* also generate the error message when */
		    /* observation probabilities are specified in an */
		    /* MDP problem, since this is illegal. */

                     checkProbs();
		     YACCtrace("pomdp_file -> preamble params\n");
                  }
#line 2217 "parser.c"
break;
case 4:
#line 184 "parser.y"
	{
		   YACCtrace("preamble -> preamble param_type\n");
		}
#line 2224 "parser.c"
break;
case 11:
#line 196 "parser.y"
	{
		  /* The discount factor only makes sense when in the */
		  /* range 0 to 1, so it is an error to specify */
		  /* anything outside this range. */

                   gDiscount = yystack.l_mark[0].f_num;
                   if(( gDiscount < 0.0 ) || ( gDiscount > 1.0 ))
                      ERR_enter("Parser<ytab>:", currentLineNumber,
                                BAD_DISCOUNT_VAL, "");
                   discountDefined = 1;
		   YACCtrace("discount_param -> DISCOUNTTOK COLONTOK number\n");
	        }
#line 2240 "parser.c"
break;
case 12:
#line 210 "parser.y"
	{
                   valuesDefined = 1;
		   YACCtrace("value_param -> VALUESTOK COLONTOK value_tail\n");
	        }
#line 2248 "parser.c"
break;
case 13:
#line 223 "parser.y"
	{
                   gValueType = REWARD_value_type;
		}
#line 2255 "parser.c"
break;
case 14:
#line 227 "parser.y"
	{
                   gValueType = COST_value_type;
		}
#line 2262 "parser.c"
break;
case 15:
#line 232 "parser.y"
	{ 
		  /* Since are able to enumerate the states and refer */
		  /* to them by identifiers, we will need to set the */
		  /* current state to indicate that we are parsing */
		  /* states.  This is important, since we will parse */
		  /* observatons and actions in exactly the same */
		  /* manner with the same code.  */
 
		  curMnemonic = nt_state; 

		}
#line 2277 "parser.c"
break;
case 16:
#line 244 "parser.y"
	{
                   statesDefined = 1;
                   curMnemonic = nt_unknown;
		   YACCtrace("state_param -> STATETOK COLONTOK state_tail\n");
		}
#line 2286 "parser.c"
break;
case 17:
#line 251 "parser.y"
	{

		  /*  For the number of states, we can just have a */
		  /*  number indicating how many there are, or ... */

                   gNumStates = yystack.l_mark[0].constBlk->theValue.theInt;
                   if( gNumStates < 1 ) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_NUM_STATES, "");
                      gNumStates = 1;
                   }

 		   /* Since we use some temporary storage to hold the
		      integer as we parse, we free the memory when we
		      are done with the value */

                   XFREE( yystack.l_mark[0].constBlk );
		}
#line 2308 "parser.c"
break;
case 19:
#line 273 "parser.y"
	{
		  /* See state_param for explanation of this */

		  curMnemonic = nt_action;  
		}
#line 2317 "parser.c"
break;
case 20:
#line 279 "parser.y"
	{
                   actionsDefined = 1;
                   curMnemonic = nt_unknown;
		   YACCtrace("action_param -> ACTIONTOK COLONTOK action_tail\n");
		}
#line 2326 "parser.c"
break;
case 21:
#line 286 "parser.y"
	{

		  /*  For the number of actions, we can just have a */
		  /*  number indicating how many there are, or ... */

                   gNumActions = yystack.l_mark[0].constBlk->theValue.theInt;
                   if( gNumActions < 1 ) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_NUM_ACTIONS, "" );
                      gNumActions = 1;
                   }
		   
		   /* Since we use some temporary storage to hold the
		      integer as we parse, we free the memory when we
		      are done with the value */

                   XFREE( yystack.l_mark[0].constBlk );
		}
#line 2348 "parser.c"
break;
case 23:
#line 308 "parser.y"
	{ 
		  /* See state_param for explanation of this */

		  curMnemonic = nt_observation; 
		}
#line 2357 "parser.c"
break;
case 24:
#line 314 "parser.y"
	{
                   observationsDefined = 1;
                   curMnemonic = nt_unknown;
		   YACCtrace("obs_param -> OBSTOK COLONTOK obs_param_tail\n");
		}
#line 2366 "parser.c"
break;
case 25:
#line 321 "parser.y"
	{

		  /*  For the number of observation, we can just have a */
		  /*  number indicating how many there are, or ... */

                   gNumObservations = yystack.l_mark[0].constBlk->theValue.theInt;
                   if( gNumObservations < 1 ) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_NUM_OBS, "" );
                      gNumObservations = 1;
                   }

		   /* Since we use some temporary storage to hold the
		      integer as we parse, we free the memory when we
		      are done with the value */

                   XFREE( yystack.l_mark[0].constBlk );
		}
#line 2388 "parser.c"
break;
case 27:
#line 343 "parser.y"
	{ 
		  /* There are a number of different formats for the */
		  /* start state.  This one is valid for either a */
		  /* POMDP or an MDP.  With a POMDP it will expect a */
		  /* list of probabilities, one for each state, */
		  /* representing the initial belief state.  For an */
		  /* MDP there can be only a single integer */
		  /* representing the starting state. */

		  if( gProblemType == POMDP_problem_type )
		    setMatrixContext(mc_start_belief, 0, 0, 0, 0); 
		  else
		    setMatrixContext(mc_mdp_start, 0, 0, 0, 0); 
		}
#line 2406 "parser.c"
break;
case 29:
#line 376 "parser.y"
	{
                   int num;

		   num = H_lookup( yystack.l_mark[0].constBlk->theValue.theString, nt_state );
		   if(( num < 0 ) || (num >= gNumStates )) {
		     ERR_enter("Parser<ytab>:", currentLineNumber, 
					BAD_STATE_STR, yystack.l_mark[0].constBlk->theValue.theString );
		   }
		   else {
		     if( gProblemType == MDP_problem_type )
		       gInitialState = num;
		     else
		       gInitialBelief[num] = 1.0;
		   }

		   XFREE( yystack.l_mark[0].constBlk->theValue.theString );
		   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2428 "parser.c"
break;
case 30:
#line 396 "parser.y"
	{ 
		  setMatrixContext(mc_start_include, 0, 0, 0, 0); 
		}
#line 2435 "parser.c"
break;
case 32:
#line 402 "parser.y"
	{ 
		  setMatrixContext(mc_start_exclude, 0, 0, 0, 0); 
		}
#line 2442 "parser.c"
break;
case 34:
#line 409 "parser.y"
	{ 
		  setStartStateUniform(); 
		}
#line 2449 "parser.c"
break;
case 35:
#line 414 "parser.y"
	{
		  enterStartState( yystack.l_mark[0].i_num );
                }
#line 2456 "parser.c"
break;
case 36:
#line 418 "parser.y"
	{
		  enterStartState( yystack.l_mark[0].i_num );
                }
#line 2463 "parser.c"
break;
case 40:
#line 427 "parser.y"
	{
		    /* If there are observation specifications defined,
		       but no observations listed in the preamble, then
		       this is an error, since regular MDPs don't have
		       the concept of observations.  However, it could 
		       be a POMDP that was just missing the preamble 
		       part.  The way we handle this is to go ahead 
		       and parse the observation specifications, but
		       always check before we actually enter values in
		       a matrix (see the enterMatrix() routine.)  This
		       way we can determine if there are any problems 
		       with the observation specifications.  We cannot
		       add entries to the matrices since there will be
		       no memory allocated for it.  We want to
		       generate an error for this case, but don't want
		       a separate error for each observation
		       specification, so we define a variable that is
		       just a flag for whether or not any observation
		       specificiations have been defined.  After we
		       are all done parsing we will check this flag
		       and generate an error if needed.
		       */

		      observationSpecDefined = 1;
		  }
#line 2492 "parser.c"
break;
case 42:
#line 455 "parser.y"
	{
		   YACCtrace("trans_prob_spec -> TTOK COLONTOK trans_spec_tail\n");
		}
#line 2499 "parser.c"
break;
case 43:
#line 460 "parser.y"
	{ setMatrixContext(mc_trans_single, yystack.l_mark[-4].i_num, yystack.l_mark[-2].i_num, yystack.l_mark[0].i_num, 0); }
#line 2504 "parser.c"
break;
case 44:
#line 461 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );
		   YACCtrace("trans_spec_tail -> action COLONTOK state COLONTOK state prob \n");
		}
#line 2512 "parser.c"
break;
case 45:
#line 466 "parser.y"
	{ setMatrixContext(mc_trans_row, yystack.l_mark[-2].i_num, yystack.l_mark[0].i_num, 0, 0); }
#line 2517 "parser.c"
break;
case 46:
#line 467 "parser.y"
	{
		   YACCtrace("trans_spec_tail -> action COLONTOK state ui_matrix \n");
		}
#line 2524 "parser.c"
break;
case 47:
#line 470 "parser.y"
	{ setMatrixContext(mc_trans_all, yystack.l_mark[0].i_num, 0, 0, 0); }
#line 2529 "parser.c"
break;
case 48:
#line 471 "parser.y"
	{
		   YACCtrace("trans_spec_tail -> action ui_matrix\n");
		}
#line 2536 "parser.c"
break;
case 49:
#line 476 "parser.y"
	{
		   YACCtrace("obs_prob_spec -> OTOK COLONTOK  obs_spec_tail\n");
		}
#line 2543 "parser.c"
break;
case 50:
#line 481 "parser.y"
	{ setMatrixContext(mc_obs_single, yystack.l_mark[-4].i_num, 0, yystack.l_mark[-2].i_num, yystack.l_mark[0].i_num); }
#line 2548 "parser.c"
break;
case 51:
#line 482 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );
		   YACCtrace("obs_spec_tail -> action COLONTOK state COLONTOK obs prob \n");
		}
#line 2556 "parser.c"
break;
case 52:
#line 487 "parser.y"
	{ setMatrixContext(mc_obs_row, yystack.l_mark[-2].i_num, 0, yystack.l_mark[0].i_num, 0); }
#line 2561 "parser.c"
break;
case 53:
#line 488 "parser.y"
	{
		   YACCtrace("obs_spec_tail -> action COLONTOK state COLONTOK u_matrix\n");
		}
#line 2568 "parser.c"
break;
case 54:
#line 491 "parser.y"
	{ setMatrixContext(mc_obs_all, yystack.l_mark[0].i_num, 0, 0, 0); }
#line 2573 "parser.c"
break;
case 55:
#line 492 "parser.y"
	{
		   YACCtrace("obs_spec_tail -> action u_matrix\n");
		}
#line 2580 "parser.c"
break;
case 56:
#line 497 "parser.y"
	{
		   YACCtrace("reward_spec -> RTOK COLONTOK  reward_spec_tail\n");
		}
#line 2587 "parser.c"
break;
case 57:
#line 504 "parser.y"
	{ setMatrixContext(mc_reward_single, yystack.l_mark[-6].i_num, yystack.l_mark[-4].i_num, yystack.l_mark[-2].i_num, yystack.l_mark[0].i_num); }
#line 2592 "parser.c"
break;
case 58:
#line 505 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );

		   /* Only need this for the call to doneImmReward */
		   checkMatrix();  
		   YACCtrace("reward_spec_tail -> action COLONTOK state COLONTOK state COLONTOK obs number\n");
		}
#line 2603 "parser.c"
break;
case 59:
#line 513 "parser.y"
	{ setMatrixContext(mc_reward_row, yystack.l_mark[-4].i_num, yystack.l_mark[-2].i_num, yystack.l_mark[0].i_num, 0); }
#line 2608 "parser.c"
break;
case 60:
#line 514 "parser.y"
	{
                   checkMatrix();
		   YACCtrace("reward_spec_tail -> action COLONTOK state COLONTOK state num_matrix\n");
		 }
#line 2616 "parser.c"
break;
case 61:
#line 519 "parser.y"
	{ setMatrixContext(mc_reward_all, yystack.l_mark[-2].i_num, yystack.l_mark[0].i_num, 0, 0); }
#line 2621 "parser.c"
break;
case 62:
#line 520 "parser.y"
	{
                   checkMatrix();
		   YACCtrace("reward_spec_tail -> action COLONTOK state num_matrix\n");
		}
#line 2629 "parser.c"
break;
case 63:
#line 526 "parser.y"
	{ setMatrixContext(mc_reward_mdp_only, yystack.l_mark[0].i_num, 0, 0, 0); }
#line 2634 "parser.c"
break;
case 64:
#line 527 "parser.y"
	{
                   checkMatrix();
		   YACCtrace("reward_spec_tail -> action num_matrix\n");
                }
#line 2642 "parser.c"
break;
case 65:
#line 533 "parser.y"
	{
                   enterUniformMatrix();
                }
#line 2649 "parser.c"
break;
case 66:
#line 537 "parser.y"
	{
                   enterIdentityMatrix();
                }
#line 2656 "parser.c"
break;
case 67:
#line 541 "parser.y"
	{
                   checkMatrix();
                }
#line 2663 "parser.c"
break;
case 68:
#line 547 "parser.y"
	{
                   enterUniformMatrix();
                }
#line 2670 "parser.c"
break;
case 69:
#line 551 "parser.y"
	{
		  enterResetMatrix();
		}
#line 2677 "parser.c"
break;
case 70:
#line 555 "parser.y"
	{
                   checkMatrix();
                }
#line 2684 "parser.c"
break;
case 71:
#line 560 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );
                }
#line 2691 "parser.c"
break;
case 72:
#line 564 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );
                }
#line 2698 "parser.c"
break;
case 73:
#line 569 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );
                }
#line 2705 "parser.c"
break;
case 74:
#line 573 "parser.y"
	{
                   enterMatrix( yystack.l_mark[0].f_num );
                }
#line 2712 "parser.c"
break;
case 75:
#line 578 "parser.y"
	{
                   if(( yystack.l_mark[0].constBlk->theValue.theInt < 0 ) 
                      || (yystack.l_mark[0].constBlk->theValue.theInt >= gNumStates )) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_STATE_VAL, "");
                      yyval.i_num = 0;
                   }
                   else
                      yyval.i_num = yystack.l_mark[0].constBlk->theValue.theInt;
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2727 "parser.c"
break;
case 76:
#line 590 "parser.y"
	{
                   int num;
                   num = H_lookup( yystack.l_mark[0].constBlk->theValue.theString, nt_state );
                   if (( num < 0 ) || (num >= gNumStates )) {
				 ERR_enter("Parser<ytab>:", currentLineNumber, 
						 BAD_STATE_STR, yystack.l_mark[0].constBlk->theValue.theString );
				 yyval.i_num = 0;
                   }
                   else
				 yyval.i_num = num;

                   XFREE( yystack.l_mark[0].constBlk->theValue.theString );
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2745 "parser.c"
break;
case 77:
#line 605 "parser.y"
	{
                   yyval.i_num = WILDCARD_SPEC;
                }
#line 2752 "parser.c"
break;
case 78:
#line 610 "parser.y"
	{
                   yyval.i_num = yystack.l_mark[0].constBlk->theValue.theInt;
                   if(( yystack.l_mark[0].constBlk->theValue.theInt < 0 ) 
                      || (yystack.l_mark[0].constBlk->theValue.theInt >= gNumActions )) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_ACTION_VAL, "" );
                      yyval.i_num = 0;
                   }
                   else
                      yyval.i_num = yystack.l_mark[0].constBlk->theValue.theInt;
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2768 "parser.c"
break;
case 79:
#line 623 "parser.y"
	{
                   int num;
                   num = H_lookup( yystack.l_mark[0].constBlk->theValue.theString, nt_action );
                   if(( num < 0 ) || (num >= gNumActions )) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_ACTION_STR, yystack.l_mark[0].constBlk->theValue.theString );
                      yyval.i_num = 0;
                   }
                   else
                      yyval.i_num = num;

                   XFREE( yystack.l_mark[0].constBlk->theValue.theString );
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2786 "parser.c"
break;
case 80:
#line 638 "parser.y"
	{
                   yyval.i_num = WILDCARD_SPEC;
                }
#line 2793 "parser.c"
break;
case 81:
#line 643 "parser.y"
	{
                   if(( yystack.l_mark[0].constBlk->theValue.theInt < 0 ) 
                      || (yystack.l_mark[0].constBlk->theValue.theInt >= gNumObservations )) {
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_OBS_VAL, "");
                      yyval.i_num = 0;
                   }
                   else
                      yyval.i_num = yystack.l_mark[0].constBlk->theValue.theInt;
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2808 "parser.c"
break;
case 82:
#line 655 "parser.y"
	{
                   int num;
                   num = H_lookup( yystack.l_mark[0].constBlk->theValue.theString, nt_observation );
                   if(( num < 0 ) || (num >= gNumObservations )) { 
                      ERR_enter("Parser<ytab>:", currentLineNumber, 
                                BAD_OBS_STR, yystack.l_mark[0].constBlk->theValue.theString);
                      yyval.i_num = 0;
                   }
                   else
                      yyval.i_num = num;

                   XFREE( yystack.l_mark[0].constBlk->theValue.theString );
                   XFREE( yystack.l_mark[0].constBlk );
               }
#line 2826 "parser.c"
break;
case 83:
#line 670 "parser.y"
	{
                   yyval.i_num = WILDCARD_SPEC;
                }
#line 2833 "parser.c"
break;
case 84:
#line 675 "parser.y"
	{
                   enterString( yystack.l_mark[0].constBlk );
                }
#line 2840 "parser.c"
break;
case 85:
#line 679 "parser.y"
	{
                   enterString( yystack.l_mark[0].constBlk );
                }
#line 2847 "parser.c"
break;
case 86:
#line 684 "parser.y"
	{
		  yyval.f_num = yystack.l_mark[0].constBlk->theValue.theInt;
		  if( curMatrixContext != mc_mdp_start )
		    if(( yyval.f_num < 0 ) || (yyval.f_num > 1 ))
		      ERR_enter("Parser<ytab>:", currentLineNumber, 
				BAD_PROB_VAL, "");
		  XFREE( yystack.l_mark[0].constBlk );
		}
#line 2859 "parser.c"
break;
case 87:
#line 693 "parser.y"
	{
		  yyval.f_num = yystack.l_mark[0].constBlk->theValue.theFloat;
		  if( curMatrixContext == mc_mdp_start )
		    ERR_enter("Parser<ytab>:", currentLineNumber, 
				    BAD_START_STATE_TYPE, "" );
		  else
		    if(( yyval.f_num < 0.0 ) || (yyval.f_num > 1.0 ))
			 ERR_enter("Parser<ytab>:", currentLineNumber, 
					 BAD_PROB_VAL, "" );
		  XFREE( yystack.l_mark[0].constBlk );
		}
#line 2874 "parser.c"
break;
case 88:
#line 706 "parser.y"
	{
                   if( yystack.l_mark[-1].i_num )
                      yyval.f_num = yystack.l_mark[0].constBlk->theValue.theInt * -1.0;
                   else
                      yyval.f_num = yystack.l_mark[0].constBlk->theValue.theInt;
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2885 "parser.c"
break;
case 89:
#line 714 "parser.y"
	{
                   if( yystack.l_mark[-1].i_num )
                      yyval.f_num = yystack.l_mark[0].constBlk->theValue.theFloat * -1.0;
                   else
                      yyval.f_num = yystack.l_mark[0].constBlk->theValue.theFloat;
                   XFREE( yystack.l_mark[0].constBlk );
                }
#line 2896 "parser.c"
break;
case 90:
#line 723 "parser.y"
	{
                   yyval.i_num = 0;
                }
#line 2903 "parser.c"
break;
case 91:
#line 727 "parser.y"
	{
                   yyval.i_num = 1;
                }
#line 2910 "parser.c"
break;
case 92:
#line 731 "parser.y"
	{
                   yyval.i_num = 0;
                }
#line 2917 "parser.c"
break;
#line 2919 "parser.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
