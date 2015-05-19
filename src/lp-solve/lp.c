#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 10 "lp.y"
#include <string.h>
#include "lpkit.h" 
#include "lpglob.h"
#include "read.h"

/* globals */
char Last_var[NAMELEN];
char Constraint_name[NAMELEN];
int Lin_term_count;
REAL f;
int x;
int Sign;
int isign; 	/* internal_sign variable to make sure nothing goes wrong */
		/* with lookahead */
int make_neg;	/* is true after the relational operator is seen in order */
		/* to remember if lin_term stands before or after re_op */
int Within_int_decl = FALSE; /* TRUE when we are within an int declaration */
#line 37 "lp.c"

#ifndef YYSTYPE
typedef int YYSTYPE;
#endif

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

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define VAR 257
#define CONS 258
#define SIGN 259
#define AR_M_OP 260
#define RE_OP 261
#define END_C 262
#define COMMA 263
#define COLON 264
#define MINIMISE 265
#define MAXIMISE 266
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    2,    0,    3,    3,    5,    7,    5,    9,    6,    4,
    4,   10,   10,   11,   12,   13,   13,   13,    8,   15,
    8,   16,    8,   14,   14,   18,   19,   18,   20,   18,
   17,   17,   17,    1,    1,    1,   21,   21,
};
static const short yylen[] = {                            2,
    0,    4,    1,    2,    1,    0,    4,    0,    5,    0,
    1,    1,    2,    3,    1,    1,    2,    3,    1,    0,
    3,    0,    4,    1,    1,    1,    0,    3,    0,    4,
    1,    2,    3,    2,    2,    1,    2,    1,
};
static const short yydefred[] = {                         1,
    0,    0,   31,    0,   27,   38,    0,    0,    0,   26,
    0,   36,   32,    0,    0,   35,   34,    0,    0,   20,
    0,    3,    5,    0,   19,   24,   29,   37,   33,   28,
    6,    0,    0,    2,    4,    0,   12,    0,   22,    8,
    0,    0,   21,   15,   13,   16,    0,    0,    0,   30,
    7,   17,   14,    0,   23,    0,   18,    9,
};
static const short yydgoto[] = {                          1,
    9,    2,   21,   34,   22,   23,   42,   24,   49,   36,
   37,   38,   47,   25,   32,   48,   26,   11,   15,   41,
   12,
};
static const short yysindex[] = {                         0,
    0, -255,    0, -228,    0,    0, -239, -239, -217,    0,
 -229,    0,    0, -249, -205,    0,    0, -252, -228,    0,
 -214,    0,    0, -223,    0,    0,    0,    0,    0,    0,
    0, -203, -252,    0,    0, -241,    0, -236,    0,    0,
 -205, -211,    0,    0,    0,    0, -248, -203, -211,    0,
    0,    0,    0, -226,    0, -225,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -210, -234,    0,
   35,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -235,    0,    0,   50,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
static const short yygindex[] = {                         0,
    0,    0,    0,    0,   37,   17,    0,   11,    0,    0,
   25,    0,    0,  -31,    0,    0,   -2,    0,    0,    0,
   49,
};
#define YYTABLESIZE 61
static const short yytable[] = {                         10,
   43,    3,    4,    5,   10,   10,    6,   29,   52,    7,
    8,   31,   30,   53,   54,   44,   55,    3,    4,    5,
   46,   15,    6,   31,   25,   31,   25,   25,   13,   27,
   57,   14,   28,   39,   10,   39,   58,   40,   50,   18,
   19,   20,   33,   19,   20,    3,   19,   20,   31,   11,
   31,    3,    4,    3,   19,   16,   17,   35,   51,   56,
   45,
};
static const short yycheck[] = {                          2,
   32,  257,  258,  259,    7,    8,  262,  257,  257,  265,
  266,  264,   15,  262,  263,  257,   48,  257,  258,  259,
  257,  257,  262,  259,  259,  261,  261,  262,  257,  259,
  257,  260,  262,  259,    0,  259,  262,  261,   41,  257,
  258,  259,  257,  258,  259,  257,  258,  259,  259,    0,
  261,  257,  258,  257,  258,    7,    8,   21,   42,   49,
   36,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 266
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"VAR","CONS","SIGN","AR_M_OP",
"RE_OP","END_C","COMMA","COLON","MINIMISE","MAXIMISE","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : inputfile",
"$$1 :",
"inputfile : $$1 objective_function constraints int_declarations",
"constraints : constraint",
"constraints : constraints constraint",
"constraint : real_constraint",
"$$2 :",
"constraint : VAR COLON $$2 real_constraint",
"$$3 :",
"real_constraint : x_lineair_sum RE_OP $$3 x_lineair_sum END_C",
"int_declarations :",
"int_declarations : real_int_decls",
"real_int_decls : int_declaration",
"real_int_decls : real_int_decls int_declaration",
"int_declaration : int_declarator vars END_C",
"int_declarator : VAR",
"vars : VAR",
"vars : vars VAR",
"vars : vars COMMA VAR",
"x_lineair_sum : x_lineair_term",
"$$4 :",
"x_lineair_sum : SIGN $$4 x_lineair_term",
"$$5 :",
"x_lineair_sum : x_lineair_sum SIGN $$5 x_lineair_term",
"x_lineair_term : lineair_term",
"x_lineair_term : CONS",
"lineair_sum : lineair_term",
"$$6 :",
"lineair_sum : SIGN $$6 lineair_term",
"$$7 :",
"lineair_sum : lineair_sum SIGN $$7 lineair_term",
"lineair_term : VAR",
"lineair_term : CONS VAR",
"lineair_term : CONS AR_M_OP VAR",
"objective_function : MAXIMISE real_of",
"objective_function : MINIMISE real_of",
"objective_function : real_of",
"real_of : lineair_sum END_C",
"real_of : END_C",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

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

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 205 "lp.y"
#include <string.h>
#include "lex.c"
#line 247 "lp.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 35 "lp.y"
	{
  init_read();
  isign = 0;
  make_neg = 0;
  Sign = 0;
}
break;
case 6:
#line 53 "lp.y"
	{
  add_constraint_name(Last_var, Rows);
}
break;
case 8:
#line 62 "lp.y"
	{
  store_re_op();
  make_neg = 1; 
}
break;
case 9:
#line 68 "lp.y"
	{
  if(Lin_term_count == 0)
    {
      fprintf(stderr, "WARNING line %d: constraint contains no variables\n",
	      yylineno);
      null_tmp_store();
    }

  if(Lin_term_count  > 1)
    Rows++;

  if(Lin_term_count == 1)
    store_bounds();

  Lin_term_count = 0;
  isign = 0 ;
  make_neg = 0;
  Constraint_name[0] = '\0';
}
break;
case 15:
#line 100 "lp.y"
	{check_decl(Within_int_decl);}
break;
case 16:
#line 103 "lp.y"
	{if(!Ignore_decl)add_int_var((char *)yytext);}
break;
case 17:
#line 104 "lp.y"
	{if(!Ignore_decl)add_int_var((char *)yytext);}
break;
case 18:
#line 105 "lp.y"
	{if(!Ignore_decl)add_int_var((char *)yytext);}
break;
case 20:
#line 110 "lp.y"
	{
  isign = Sign; 
}
break;
case 22:
#line 116 "lp.y"
	{
  isign = Sign; 
}
break;
case 25:
#line 124 "lp.y"
	{
  if (    (isign || !make_neg)
      && !(isign && !make_neg)) /* but not both! */
    f = -f;
  rhs_store(f);
  isign = 0;
}
break;
case 27:
#line 135 "lp.y"
	{
  isign = Sign;
}
break;
case 29:
#line 141 "lp.y"
	{
  isign = Sign;
}
break;
case 31:
#line 148 "lp.y"
	{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    var_store(Last_var, Rows, (REAL) -1);
  else
    var_store(Last_var, Rows, (REAL) 1);
  isign = 0;
}
break;
case 32:
#line 158 "lp.y"
	{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    f = -f;
  var_store(Last_var, Rows, f);
  isign = 0;
}
break;
case 33:
#line 168 "lp.y"
	{
  if (    (isign || make_neg)
      && !(isign && make_neg)) /* but not both! */
    f = -f;
  var_store(Last_var, Rows, f);
  isign = 0;
}
break;
case 34:
#line 178 "lp.y"
	{
  Maximise = TRUE;
}
break;
case 35:
#line 182 "lp.y"
	{
  Maximise = FALSE;
}
break;
case 37:
#line 190 "lp.y"
	{
  Rows++;
  Lin_term_count  =  0;
  isign = 0;
  make_neg = 0;
}
break;
case 38:
#line 197 "lp.y"
	{
  Rows++;
  Lin_term_count  =  0;
  isign = 0;
  make_neg = 0;
}
break;
#line 604 "lp.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
