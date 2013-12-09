
/*  A Bison parser, made from gmparser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse gmparse
#define yylex gmlex
#define yyerror gmerror
#define yylval gmlval
#define yychar gmchar
#define yydebug gmdebug
#define yynerrs gmnerrs
#define	KEYWORD_LOCAL	258
#define	KEYWORD_GLOBAL	259
#define	KEYWORD_MEMBER	260
#define	KEYWORD_AND	261
#define	KEYWORD_OR	262
#define	KEYWORD_IF	263
#define	KEYWORD_ELSE	264
#define	KEYWORD_WHILE	265
#define	KEYWORD_FOR	266
#define	KEYWORD_FOREACH	267
#define	KEYWORD_IN	268
#define	KEYWORD_BREAK	269
#define	KEYWORD_CONTINUE	270
#define	KEYWORD_NULL	271
#define	KEYWORD_DOWHILE	272
#define	KEYWORD_RETURN	273
#define	KEYWORD_FUNCTION	274
#define	KEYWORD_TABLE	275
#define	KEYWORD_THIS	276
#define	KEYWORD_TRUE	277
#define	KEYWORD_FALSE	278
#define	KEYWORD_FORK	279
#define	IDENTIFIER	280
#define	CONSTANT_HEX	281
#define	CONSTANT_BINARY	282
#define	CONSTANT_INT	283
#define	CONSTANT_CHAR	284
#define	CONSTANT_FLOAT	285
#define	CONSTANT_STRING	286
#define	SYMBOL_ASGN_BSR	287
#define	SYMBOL_ASGN_BSL	288
#define	SYMBOL_ASGN_ADD	289
#define	SYMBOL_ASGN_MINUS	290
#define	SYMBOL_ASGN_TIMES	291
#define	SYMBOL_ASGN_DIVIDE	292
#define	SYMBOL_ASGN_REM	293
#define	SYMBOL_ASGN_BAND	294
#define	SYMBOL_ASGN_BOR	295
#define	SYMBOL_ASGN_BXOR	296
#define	SYMBOL_RIGHT_SHIFT	297
#define	SYMBOL_LEFT_SHIFT	298
#define	SYMBOL_LTE	299
#define	SYMBOL_GTE	300
#define	SYMBOL_EQ	301
#define	SYMBOL_NEQ	302
#define	TOKEN_ERROR	303



#define YYPARSER
#include "gmConfig.h"
#include "gmCodeTree.h"
#define YYSTYPE gmCodeTreeNode *

extern gmCodeTreeNode * g_codeTree;

#define GM_BISON_DEBUG
#ifdef GM_BISON_DEBUG
#define YYDEBUG 1
#define YYERROR_VERBOSE
#endif // GM_BISON_DEBUG

//
// HELPERS
//

void ATTACH(gmCodeTreeNode * &a_res, gmCodeTreeNode * a_a, gmCodeTreeNode * a_b)
{
  YYSTYPE t = a_a;
  if(t != NULL)
  {
    while(t->m_sibling != NULL)
    {
      t = t->m_sibling;
    }
    t->m_sibling = a_b;
    if(a_b) { a_b->m_parent = t; }
    a_res = a_a;
  }
  else
  {
    a_res = a_b;
  }
}

gmCodeTreeNode * CreateOperation(int a_subTypeType, gmCodeTreeNode * a_left = NULL, gmCodeTreeNode * a_right = NULL)
{
  gmCodeTreeNode * node = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_OPERATION, gmlineno, a_subTypeType);
  node->SetChild(0, a_left);
  node->SetChild(1, a_right);
  return node;
}

gmCodeTreeNode * CreateAsignExpression(int a_subTypeType, gmCodeTreeNode * a_left, gmCodeTreeNode * a_right)
{
  // we need to evaluate the complexety of the l-value... if it is a function call, index or dot to the left of a dot or index, we need to cache
  // into a hidden variable.

  // todo

  gmCodeTreeNode * opNode = CreateOperation(a_subTypeType, a_left, a_right);
  return CreateOperation(CTNOT_ASSIGN, a_left, opNode);
}


#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		251
#define	YYFLAG		-32768
#define	YYNTBASE	73

#define YYTRANSLATE(x) ((unsigned)(x) <= 303 ? yytranslate[x] : 111)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    67,     2,     2,     2,    64,    57,     2,    51,
    52,    62,    60,    72,    61,    71,    63,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    70,    53,    58,
    54,    59,    66,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    68,     2,    69,    56,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    49,    55,    50,    65,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     7,     9,    11,    13,    15,    17,    19,
    22,    26,    32,    39,    43,    49,    56,    64,    66,    68,
    70,    72,    75,    81,    89,    97,   101,   104,   110,   116,
   123,   131,   139,   149,   152,   155,   158,   162,   164,   168,
   172,   176,   180,   184,   188,   192,   196,   200,   204,   208,
   210,   213,   215,   217,   221,   223,   227,   229,   233,   235,
   239,   241,   245,   247,   251,   255,   257,   261,   265,   269,
   273,   275,   279,   283,   285,   289,   293,   295,   299,   303,
   307,   309,   312,   314,   316,   318,   320,   322,   324,   328,
   333,   337,   342,   348,   355,   359,   361,   365,   369,   374,
   377,   381,   386,   392,   397,   399,   403,   405,   409,   411,
   415,   417,   421,   423,   426,   428,   430,   432,   434,   438,
   440,   442,   444,   446,   448,   450,   452,   454,   456,   458,
   460
};

static const short yyrhs[] = {    74,
     0,    75,     0,    74,    75,     0,    80,     0,    78,     0,
    81,     0,    82,     0,    83,     0,    77,     0,    49,    50,
     0,    49,    74,    50,     0,    19,   108,    51,    52,    76,
     0,    19,   108,    51,   105,    52,    76,     0,    79,   108,
    53,     0,    79,   108,    54,    86,    53,     0,    79,    19,
   108,    51,    52,    76,     0,    79,    19,   108,    51,   105,
    52,    76,     0,     3,     0,     4,     0,     5,     0,    53,
     0,    84,    53,     0,     8,    51,    86,    52,    76,     0,
     8,    51,    86,    52,    76,     9,    76,     0,     8,    51,
    86,    52,    76,     9,    81,     0,    24,   108,    76,     0,
    24,    76,     0,    10,    51,    86,    52,    76,     0,    17,
    51,    86,    52,    76,     0,    11,    51,    80,    85,    52,
    76,     0,    11,    51,    80,    85,    84,    52,    76,     0,
    12,    51,   108,    13,    86,    52,    76,     0,    12,    51,
   108,     6,   108,    13,    86,    52,    76,     0,    15,    53,
     0,    14,    53,     0,    18,    53,     0,    18,    86,    53,
     0,    87,     0,    99,    54,    87,     0,    99,    32,    87,
     0,    99,    33,    87,     0,    99,    34,    87,     0,    99,
    35,    87,     0,    99,    36,    87,     0,    99,    37,    87,
     0,    99,    38,    87,     0,    99,    39,    87,     0,    99,
    40,    87,     0,    99,    41,    87,     0,    53,     0,    86,
    53,     0,    87,     0,    88,     0,    87,     7,    88,     0,
    89,     0,    88,     6,    89,     0,    90,     0,    89,    55,
    90,     0,    91,     0,    90,    56,    91,     0,    92,     0,
    91,    57,    92,     0,    93,     0,    92,    46,    93,     0,
    92,    47,    93,     0,    94,     0,    93,    58,    94,     0,
    93,    59,    94,     0,    93,    44,    94,     0,    93,    45,
    94,     0,    95,     0,    94,    43,    95,     0,    94,    42,
    95,     0,    96,     0,    95,    60,    96,     0,    95,    61,
    96,     0,    97,     0,    96,    62,    97,     0,    96,    63,
    97,     0,    96,    64,    97,     0,    99,     0,    98,    97,
     0,    60,     0,    61,     0,    65,     0,    66,     0,    67,
     0,   107,     0,    99,    68,    69,     0,    99,    68,    86,
    69,     0,    99,    51,    52,     0,    99,    51,   100,    52,
     0,    99,    70,   108,    51,    52,     0,    99,    70,   108,
    51,   100,    52,     0,    99,    71,   108,     0,    86,     0,
   100,    72,    86,     0,    20,    51,    52,     0,    20,    51,
   103,    52,     0,    49,    50,     0,    49,   103,    50,     0,
    49,   103,    72,    50,     0,    19,    51,   105,    52,    76,
     0,    19,    51,    52,    76,     0,   104,     0,   103,    72,
   104,     0,    86,     0,   108,    54,    86,     0,   106,     0,
   105,    72,   106,     0,   108,     0,   108,    54,    86,     0,
   108,     0,    71,   108,     0,    21,     0,   109,     0,   101,
     0,   102,     0,    51,    86,    52,     0,    25,     0,    26,
     0,    27,     0,    28,     0,    22,     0,    23,     0,    29,
     0,    30,     0,   110,     0,    16,     0,    31,     0,   110,
    31,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   123,   130,   134,   141,   145,   149,   153,   157,   161,   168,
   172,   181,   192,   207,   212,   218,   227,   240,   244,   248,
   254,   258,   265,   271,   278,   285,   291,   299,   305,   311,
   318,   326,   333,   344,   348,   352,   356,   364,   372,   376,
   380,   384,   388,   392,   396,   400,   404,   408,   412,   420,
   424,   431,   438,   442,   449,   453,   460,   464,   472,   476,
   484,   488,   496,   500,   504,   511,   515,   519,   523,   527,
   534,   538,   543,   551,   555,   560,   568,   572,   577,   582,
   590,   594,   603,   607,   611,   615,   619,   626,   630,   634,
   638,   643,   649,   655,   662,   669,   673,   680,   684,   689,
   693,   698,   706,   712,   720,   724,   731,   735,   742,   746,
   753,   758,   767,   771,   776,   780,   784,   788,   792,   799,
   808,   813,   818,   823,   828,   833,   879,   884,   888,   896,
   910
};

static const char * const yytname[] = {   "$","error","$undefined.","KEYWORD_LOCAL",
"KEYWORD_GLOBAL","KEYWORD_MEMBER","KEYWORD_AND","KEYWORD_OR","KEYWORD_IF","KEYWORD_ELSE",
"KEYWORD_WHILE","KEYWORD_FOR","KEYWORD_FOREACH","KEYWORD_IN","KEYWORD_BREAK",
"KEYWORD_CONTINUE","KEYWORD_NULL","KEYWORD_DOWHILE","KEYWORD_RETURN","KEYWORD_FUNCTION",
"KEYWORD_TABLE","KEYWORD_THIS","KEYWORD_TRUE","KEYWORD_FALSE","KEYWORD_FORK",
"IDENTIFIER","CONSTANT_HEX","CONSTANT_BINARY","CONSTANT_INT","CONSTANT_CHAR",
"CONSTANT_FLOAT","CONSTANT_STRING","SYMBOL_ASGN_BSR","SYMBOL_ASGN_BSL","SYMBOL_ASGN_ADD",
"SYMBOL_ASGN_MINUS","SYMBOL_ASGN_TIMES","SYMBOL_ASGN_DIVIDE","SYMBOL_ASGN_REM",
"SYMBOL_ASGN_BAND","SYMBOL_ASGN_BOR","SYMBOL_ASGN_BXOR","SYMBOL_RIGHT_SHIFT",
"SYMBOL_LEFT_SHIFT","SYMBOL_LTE","SYMBOL_GTE","SYMBOL_EQ","SYMBOL_NEQ","TOKEN_ERROR",
"'{'","'}'","'('","')'","';'","'='","'|'","'^'","'&'","'<'","'>'","'+'","'-'",
"'*'","'/'","'%'","'~'","'?'","'!'","'['","']'","':'","'.'","','","program",
"statement_list","statement","compound_statement","function_statement","var_statement",
"var_type","expression_statement","selection_statement","iteration_statement",
"jump_statement","assignment_expression","constant_expression_statement","constant_expression",
"logical_or_expression","logical_and_expression","inclusive_or_expression","exclusive_or_expression",
"and_expression","equality_expression","relational_expression","shift_expression",
"additive_expression","multiplicative_expression","unary_expression","unary_operator",
"postfix_expression","argument_expression_list","table_constructor","function_constructor",
"field_list","field","parameter_list","parameter","primary_expression","identifier",
"constant","constant_string_list",""
};
#endif

static const short yyr1[] = {     0,
    73,    74,    74,    75,    75,    75,    75,    75,    75,    76,
    76,    77,    77,    78,    78,    78,    78,    79,    79,    79,
    80,    80,    81,    81,    81,    81,    81,    82,    82,    82,
    82,    82,    82,    83,    83,    83,    83,    84,    84,    84,
    84,    84,    84,    84,    84,    84,    84,    84,    84,    85,
    85,    86,    87,    87,    88,    88,    89,    89,    90,    90,
    91,    91,    92,    92,    92,    93,    93,    93,    93,    93,
    94,    94,    94,    95,    95,    95,    96,    96,    96,    96,
    97,    97,    98,    98,    98,    98,    98,    99,    99,    99,
    99,    99,    99,    99,    99,   100,   100,   101,   101,   101,
   101,   101,   102,   102,   103,   103,   104,   104,   105,   105,
   106,   106,   107,   107,   107,   107,   107,   107,   107,   108,
   109,   109,   109,   109,   109,   109,   109,   109,   109,   110,
   110
};

static const short yyr2[] = {     0,
     1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
     3,     5,     6,     3,     5,     6,     7,     1,     1,     1,
     1,     2,     5,     7,     7,     3,     2,     5,     5,     6,
     7,     7,     9,     2,     2,     2,     3,     1,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
     2,     1,     1,     3,     1,     3,     1,     3,     1,     3,
     1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
     1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
     1,     2,     1,     1,     1,     1,     1,     1,     3,     4,
     3,     4,     5,     6,     3,     1,     3,     3,     4,     2,
     3,     4,     5,     4,     1,     3,     1,     3,     1,     3,
     1,     3,     1,     2,     1,     1,     1,     1,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     2
};

static const short yydefact[] = {     0,
    18,    19,    20,     0,     0,     0,     0,     0,     0,   129,
     0,     0,     0,     0,   115,   124,   125,     0,   120,   121,
   122,   123,   126,   127,   130,     0,     0,    21,    83,    84,
    85,    86,    87,     0,     1,     2,     9,     5,     0,     4,
     6,     7,     8,     0,    38,    53,    55,    57,    59,    61,
    63,    66,    71,    74,    77,     0,    81,   117,   118,    88,
   113,   116,   128,     0,     0,     0,     0,    35,    34,     0,
     0,    36,     0,    52,    81,     0,     0,     0,     0,    27,
     0,   100,   107,     0,   105,   113,     0,   114,     3,     0,
     0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   131,     0,     0,     0,
     0,     0,    37,     0,     0,   109,   111,     0,    98,     0,
    10,     0,    26,   101,     0,     0,   119,     0,    14,     0,
    54,    56,    58,    60,    62,    64,    65,    69,    70,    67,
    68,    73,    72,    75,    76,    78,    79,    80,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49,    91,    96,
     0,    39,    89,     0,     0,    95,     0,     0,    50,     0,
     0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
    99,     0,    11,   102,   106,   108,     0,     0,    92,     0,
    90,     0,    23,    28,     0,     0,    51,     0,     0,    29,
   103,   110,   112,    12,     0,     0,     0,    15,    97,    93,
     0,     0,    30,     0,     0,     0,    13,    16,     0,    94,
    24,    25,    31,     0,    32,    17,     0,    33,     0,     0,
     0
};

static const short yydefgoto[] = {   249,
    35,    36,    80,    37,    38,    39,    40,    41,    42,    43,
    44,   190,    83,    74,    46,    47,    48,    49,    50,    51,
    52,    53,    54,    55,    56,    75,   181,    58,    59,    84,
    85,   135,   136,    60,    61,    62,    63
};

static const short yypact[] = {   340,
-32768,-32768,-32768,   -29,    19,    36,    38,   -39,   -35,-32768,
    49,   393,   -13,    50,-32768,-32768,-32768,    -8,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   446,   924,-32768,-32768,-32768,
-32768,-32768,-32768,     5,   340,-32768,-32768,-32768,     1,-32768,
-32768,-32768,-32768,    -4,    25,    66,    24,    46,    47,     8,
    15,    35,    21,    28,-32768,   924,   139,-32768,-32768,-32768,
-32768,-32768,    74,   924,   924,   499,     5,-32768,-32768,   924,
    56,-32768,    58,    25,   -28,   -19,    68,   552,   210,-32768,
    60,-32768,-32768,   -37,-32768,    67,    65,-32768,-32768,     5,
    41,-32768,   924,   924,   924,   924,   924,   924,   924,   924,
   924,   924,   924,   924,   924,   924,   924,   924,   924,   924,
-32768,   924,   924,   924,   924,   924,   924,   924,   924,   924,
   924,   605,   924,   658,     5,     5,-32768,    70,    71,   712,
    63,    72,-32768,    60,   -33,-32768,    73,   -18,-32768,   -27,
-32768,   275,-32768,-32768,   765,   924,-32768,    69,-32768,   924,
    66,    24,    46,    47,     8,    15,    15,    35,    35,    35,
    35,    21,    21,    28,    28,-32768,-32768,-32768,    25,    25,
    25,    25,    25,    25,    25,    25,    25,    25,-32768,-32768,
   -24,    25,-32768,    57,    78,-32768,    60,    60,-32768,   818,
    77,     5,   924,    60,-32768,    60,     5,   924,    60,   -21,
-32768,   924,-32768,-32768,-32768,-32768,   -15,    79,-32768,   924,
-32768,   871,   116,-32768,    60,    81,-32768,   118,    84,-32768,
-32768,-32768,-32768,-32768,    60,    60,   -16,-32768,-32768,-32768,
    -5,    -3,-32768,    60,   924,    60,-32768,-32768,    60,-32768,
-32768,-32768,-32768,    85,-32768,-32768,    60,-32768,   149,   152,
-32768
};

static const short yypgoto[] = {-32768,
    75,   -34,   -31,-32768,-32768,-32768,    87,   -77,-32768,-32768,
   -32,-32768,   -12,    27,    90,    76,    64,    89,    91,    -2,
   -17,    -6,     7,   -45,-32768,     9,   -52,-32768,-32768,    83,
  -141,  -136,   -30,-32768,   -10,-32768,-32768
};


#define	YYLAST		995


static const short yytable[] = {    73,
    89,   200,    77,   205,     4,    19,    19,    81,    57,    19,
   111,    19,   144,    68,    87,    86,    19,    69,   196,    90,
    18,    64,   122,    88,   201,    19,    45,   209,    91,    19,
   225,    93,   134,   199,   145,   239,   226,    76,   197,   124,
    79,   125,   126,    57,   202,    79,   240,   210,    92,   143,
   197,   128,   129,    98,    99,   197,   131,   132,   100,   101,
   205,    45,   166,   167,   168,   137,   210,    86,   192,    65,
   227,    94,   102,   103,    57,   193,   104,   105,    95,   148,
   106,   107,   158,   159,   160,   161,    66,    57,    67,   108,
   109,   110,    45,   149,   150,   156,   157,   162,   163,    70,
    78,    96,   195,    97,   127,    45,    76,    89,    79,   180,
   133,   184,   164,   165,   185,   186,   147,   191,   138,   207,
   146,   187,   188,   194,   232,   211,   198,   137,   212,   217,
   235,   228,   234,   206,    86,   236,   247,   208,   169,   170,
   171,   172,   173,   174,   175,   176,   177,   178,   250,   182,
    57,   251,   130,   142,   242,   213,   214,   216,   153,   231,
   140,     0,   220,     0,   221,     0,   222,   224,    45,   152,
   112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
   219,   218,   151,   233,   154,   223,   137,   155,     0,   122,
     0,    86,   123,   237,   238,     0,   137,   229,    57,   180,
   241,     0,   243,     0,   245,     0,   124,   246,   125,   126,
     0,     0,     1,     2,     3,   248,    45,     4,     0,     5,
     6,     7,   244,     8,     9,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    26,   141,
    27,     0,    28,     0,     0,     0,     0,     0,     0,    29,
    30,     0,     0,     0,    31,    32,    33,     1,     2,     3,
    34,     0,     4,     0,     5,     6,     7,     0,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
    20,    21,    22,    23,    24,    25,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    26,   203,    27,     0,    28,     0,     0,
     0,     0,     0,     0,    29,    30,     0,     0,     0,    31,
    32,    33,     1,     2,     3,    34,     0,     4,     0,     5,
     6,     7,     0,     8,     9,    10,    11,    12,    13,    14,
    15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
    25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    26,     0,
    27,     0,    28,     0,     0,     0,     0,     0,     0,    29,
    30,     0,     0,     0,    31,    32,    33,     0,    10,     0,
    34,    71,    14,    15,    16,    17,     0,    19,    20,    21,
    22,    23,    24,    25,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    26,     0,    27,     0,    72,     0,     0,     0,     0,
     0,     0,    29,    30,     0,     0,     0,    31,    32,    33,
     0,    10,     0,    34,    71,    14,    15,    16,    17,     0,
    19,    20,    21,    22,    23,    24,    25,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    26,    82,    27,     0,     0,     0,
     0,     0,     0,     0,     0,    29,    30,     0,     0,     0,
    31,    32,    33,     0,    10,     0,    34,    71,    14,    15,
    16,    17,     0,    19,    20,    21,    22,    23,    24,    25,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    26,     0,    27,
     0,    28,     0,     0,     0,     0,     0,     0,    29,    30,
     0,     0,     0,    31,    32,    33,     0,    10,     0,    34,
    71,    14,    15,    16,    17,     0,    19,    20,    21,    22,
    23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    26,     0,    27,   139,     0,     0,     0,     0,     0,     0,
     0,    29,    30,     0,     0,     0,    31,    32,    33,     0,
    10,     0,    34,    71,    14,    15,    16,    17,     0,    19,
    20,    21,    22,    23,    24,    25,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    26,     0,    27,   179,     0,     0,     0,
     0,     0,     0,     0,    29,    30,     0,     0,     0,    31,
    32,    33,     0,    10,     0,    34,    71,    14,    15,    16,
    17,     0,    19,    20,    21,    22,    23,    24,    25,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    26,     0,    27,     0,
     0,     0,     0,     0,     0,     0,     0,    29,    30,     0,
     0,     0,    31,    32,    33,     0,   183,    10,    34,     0,
    71,    14,    15,    16,    17,     0,    19,    20,    21,    22,
    23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    26,     0,    27,     0,   189,     0,     0,     0,     0,     0,
     0,    29,    30,     0,     0,     0,    31,    32,    33,     0,
    10,     0,    34,    71,    14,    15,    16,    17,     0,    19,
    20,    21,    22,    23,    24,    25,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    26,   204,    27,     0,     0,     0,     0,
     0,     0,     0,     0,    29,    30,     0,     0,     0,    31,
    32,    33,     0,    10,     0,    34,    71,    14,    15,    16,
    17,     0,    19,    20,    21,    22,    23,    24,    25,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    26,     0,    27,   215,
     0,     0,     0,     0,     0,     0,     0,    29,    30,     0,
     0,     0,    31,    32,    33,     0,    10,     0,    34,    71,
    14,    15,    16,    17,     0,    19,    20,    21,    22,    23,
    24,    25,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    26,
     0,    27,   230,     0,     0,     0,     0,     0,     0,     0,
    29,    30,     0,     0,     0,    31,    32,    33,     0,    10,
     0,    34,    71,    14,    15,    16,    17,     0,    19,    20,
    21,    22,    23,    24,    25,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    26,     0,    27,     0,     0,     0,     0,     0,
     0,     0,     0,    29,    30,     0,     0,     0,    31,    32,
    33,     0,     0,     0,    34
};

static const short yycheck[] = {    12,
    35,   138,    13,   145,     8,    25,    25,    18,     0,    25,
    56,    25,    50,    53,    27,    26,    25,    53,    52,    19,
    24,    51,    51,    34,    52,    25,     0,    52,    39,    25,
    52,     7,    52,    52,    72,    52,    52,    51,    72,    68,
    49,    70,    71,    35,    72,    49,    52,    72,    53,    81,
    72,    64,    65,    46,    47,    72,    67,    70,    44,    45,
   202,    35,   108,   109,   110,    76,    72,    78,     6,    51,
   207,     6,    58,    59,    66,    13,    42,    43,    55,    90,
    60,    61,   100,   101,   102,   103,    51,    79,    51,    62,
    63,    64,    66,    53,    54,    98,    99,   104,   105,    51,
    51,    56,   134,    57,    31,    79,    51,   142,    49,   122,
    53,   124,   106,   107,   125,   126,    52,   130,    51,    51,
    54,    52,    52,    52,     9,    69,    54,   138,    51,    53,
    13,    53,    52,   146,   145,    52,    52,   150,   112,   113,
   114,   115,   116,   117,   118,   119,   120,   121,     0,   123,
   142,     0,    66,    79,   232,   187,   188,   190,    95,   212,
    78,    -1,   194,    -1,   196,    -1,   197,   199,   142,    94,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
   193,   192,    93,   215,    96,   198,   197,    97,    -1,    51,
    -1,   202,    54,   225,   226,    -1,   207,   210,   190,   212,
   232,    -1,   234,    -1,   236,    -1,    68,   239,    70,    71,
    -1,    -1,     3,     4,     5,   247,   190,     8,    -1,    10,
    11,    12,   235,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
    51,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    60,
    61,    -1,    -1,    -1,    65,    66,    67,     3,     4,     5,
    71,    -1,     8,    -1,    10,    11,    12,    -1,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    49,    50,    51,    -1,    53,    -1,    -1,
    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
    66,    67,     3,     4,     5,    71,    -1,     8,    -1,    10,
    11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,
    51,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    60,
    61,    -1,    -1,    -1,    65,    66,    67,    -1,    16,    -1,
    71,    19,    20,    21,    22,    23,    -1,    25,    26,    27,
    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    49,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,
    -1,    -1,    60,    61,    -1,    -1,    -1,    65,    66,    67,
    -1,    16,    -1,    71,    19,    20,    21,    22,    23,    -1,
    25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,
    65,    66,    67,    -1,    16,    -1,    71,    19,    20,    21,
    22,    23,    -1,    25,    26,    27,    28,    29,    30,    31,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,
    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
    -1,    -1,    -1,    65,    66,    67,    -1,    16,    -1,    71,
    19,    20,    21,    22,    23,    -1,    25,    26,    27,    28,
    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    49,    -1,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    60,    61,    -1,    -1,    -1,    65,    66,    67,    -1,
    16,    -1,    71,    19,    20,    21,    22,    23,    -1,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    49,    -1,    51,    52,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
    66,    67,    -1,    16,    -1,    71,    19,    20,    21,    22,
    23,    -1,    25,    26,    27,    28,    29,    30,    31,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
    -1,    -1,    65,    66,    67,    -1,    69,    16,    71,    -1,
    19,    20,    21,    22,    23,    -1,    25,    26,    27,    28,
    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    49,    -1,    51,    -1,    53,    -1,    -1,    -1,    -1,    -1,
    -1,    60,    61,    -1,    -1,    -1,    65,    66,    67,    -1,
    16,    -1,    71,    19,    20,    21,    22,    23,    -1,    25,
    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    49,    50,    51,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,
    66,    67,    -1,    16,    -1,    71,    19,    20,    21,    22,
    23,    -1,    25,    26,    27,    28,    29,    30,    31,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    52,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
    -1,    -1,    65,    66,    67,    -1,    16,    -1,    71,    19,
    20,    21,    22,    23,    -1,    25,    26,    27,    28,    29,
    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
    -1,    51,    52,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    60,    61,    -1,    -1,    -1,    65,    66,    67,    -1,    16,
    -1,    71,    19,    20,    21,    22,    23,    -1,    25,    26,
    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    60,    61,    -1,    -1,    -1,    65,    66,
    67,    -1,    -1,    -1,    71
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        return(0)
#define YYABORT         return(1)
#define YYERROR         goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (yychar == YYEMPTY && yylen == 1)                          \
    { yychar = (token), yylval = (value);                       \
      yychar1 = YYTRANSLATE (yychar);                           \
      YYPOPSTACK;                                               \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    { yyerror ("syntax error: cannot back up"); YYERROR; }      \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YYPURE
#define YYLEX           yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX           yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX           yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX           yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX           yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int     yychar;                 /*  the lookahead symbol                */
YYSTYPE yylval;                 /*  the semantic value of the           */
                                /*  lookahead symbol                    */

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;                 /*  location data for the lookahead     */
                                /*  symbol                              */
#endif

int yynerrs;                    /*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)      __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;              /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YYSTYPE yyvsa[YYINITDEPTH];   /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;        /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];   /*  the location stack                  */
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;                /*  the variable used to return         */
                                /*  semantic values from the action     */
                                /*  routines                            */

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;             /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = (short) yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = (int)(yyssp - yyss + 1); // _GD_ cast for 64bit build

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
         the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
         but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
                 &yyss1, size * sizeof (*yyssp),
                 &yyvs1, size * sizeof (*yyvsp),
                 &yyls1, size * sizeof (*yylsp),
                 &yystacksize);
#else
      yyoverflow("parser stack overflow",
                 &yyss1, size * sizeof (*yyssp),
                 &yyvs1, size * sizeof (*yyvsp),
                 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
        {
          yyerror("parser stack overflow");
          return 2;
        }
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
        yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
        YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)              /* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;           /* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
        {
          fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
          /* Give the individual parser a way to print the precise meaning
             of a token, for further debugging info.  */
#ifdef YYPRINT
          YYPRINT (stderr, yychar, yylval);
#endif
          fprintf (stderr, ")\n");
        }
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
               yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
        fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{
      g_codeTree = yyvsp[0];
    ;
    break;}
case 2:
{
      yyval = yyvsp[0];
    ;
    break;}
case 3:
{
      ATTACH(yyval, yyvsp[-1], yyvsp[0]);
    ;
    break;}
case 4:
{
      yyval = yyvsp[0];
    ;
    break;}
case 5:
{
      yyval = yyvsp[0];
    ;
    break;}
case 6:
{
      yyval = yyvsp[0];
    ;
    break;}
case 7:
{
      yyval = yyvsp[0];
    ;
    break;}
case 8:
{
      yyval = yyvsp[0];
    ;
    break;}
case 9:
{
	  yyval = yyvsp[0];
	;
    break;}
case 10:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_COMPOUND, gmlineno);
    ;
    break;}
case 11:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_COMPOUND, gmlineno);
      yyval->SetChild(0, yyvsp[-1]);
    ;
    break;}
case 12:
{
		gmCodeTreeNode* func = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_FUNCTION, gmlineno);
		func->SetChild(1, yyvsp[0]);
		

		yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_VARIABLE, gmlineno, (int)GMMACHINE_DEFAULT_FUNCTION);
		yyval->SetChild(0, yyvsp[-3]);
		ATTACH(yyval, yyval, CreateOperation(CTNOT_ASSIGN, yyvsp[-3], func));
	;
    break;}
case 13:
{
		gmCodeTreeNode* func = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_FUNCTION, gmlineno);
		func->SetChild(0, yyvsp[-2]);
		func->SetChild(1, yyvsp[0]);
		

		yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_VARIABLE, gmlineno, (int)GMMACHINE_DEFAULT_FUNCTION);
		yyval->SetChild(0, yyvsp[-4]);
		ATTACH(yyval, yyval, CreateOperation(CTNOT_ASSIGN, yyvsp[-4], func));
	;
    break;}
case 14:
{
      yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_VARIABLE, gmlineno, (intptr_t) yyvsp[-2]);
      yyval->SetChild(0, yyvsp[-1]);
    ;
    break;}
case 15:
{
      yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_VARIABLE, gmlineno, (intptr_t) yyvsp[-4]);
      yyval->SetChild(0, yyvsp[-3]);
      ATTACH(yyval, yyval, CreateOperation(CTNOT_ASSIGN, yyvsp[-3], yyvsp[-1]));
    ;
    break;}
case 16:
{
      gmCodeTreeNode* func = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_FUNCTION, gmlineno);
      func->SetChild(1, yyvsp[0]);

      yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_VARIABLE, gmlineno, (intptr_t) yyvsp[-5]);
      yyval->SetChild(0, yyvsp[-3]);
      ATTACH(yyval, yyval, CreateOperation(CTNOT_ASSIGN, yyvsp[-3], func));
    ;
    break;}
case 17:
{
      gmCodeTreeNode* func = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_FUNCTION, gmlineno);
      func->SetChild(0, yyvsp[-2]);
      func->SetChild(1, yyvsp[0]);

      yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_VARIABLE, gmlineno, (intptr_t) yyvsp[-6]);
      yyval->SetChild(0, yyvsp[-4]);
      ATTACH(yyval, yyval, CreateOperation(CTNOT_ASSIGN, yyvsp[-4], func));
    ;
    break;}
case 18:
{
      yyval = (YYSTYPE) CTVT_LOCAL;
    ;
    break;}
case 19:
{
      yyval = (YYSTYPE) CTVT_GLOBAL;
    ;
    break;}
case 20:
{
      yyval = (YYSTYPE) CTVT_MEMBER;
    ;
    break;}
case 21:
{
      yyval = NULL;
    ;
    break;}
case 22:
{
      yyval = yyvsp[-1];
    ;
    break;}
case 23:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_IF, (yyvsp[-2]) ? yyvsp[-2]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[0]);
    ;
    break;}
case 24:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_IF, (yyvsp[-4]) ? yyvsp[-4]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-4]);
      yyval->SetChild(1, yyvsp[-2]);
      yyval->SetChild(2, yyvsp[0]);
    ;
    break;}
case 25:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_IF, (yyvsp[-4]) ? yyvsp[-4]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-4]);
      yyval->SetChild(1, yyvsp[-2]);
      yyval->SetChild(2, yyvsp[0]);
    ;
    break;}
case 26:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_FORK, (yyvsp[-1]) ? yyvsp[-1]->m_lineNumber : gmlineno );
      yyval->SetChild(0, yyvsp[0] );
      yyval->SetChild(1, yyvsp[-1] );
    ;
    break;}
case 27:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_FORK, (yyvsp[0]) ? yyvsp[0]->m_lineNumber : gmlineno );
      yyval->SetChild(0, yyvsp[0] );
    ;
    break;}
case 28:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_WHILE, (yyvsp[-2]) ? yyvsp[-2]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[0]);
    ;
    break;}
case 29:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_DOWHILE, (yyvsp[-2]) ? yyvsp[-2]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[0]);
    ;
    break;}
case 30:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_FOR, (yyvsp[-3]) ? yyvsp[-3]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-3]);
      yyval->SetChild(1, yyvsp[-2]);
      yyval->SetChild(3, yyvsp[0]);
    ;
    break;}
case 31:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_FOR, (yyvsp[-4]) ? yyvsp[-4]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-4]);
      yyval->SetChild(1, yyvsp[-3]);
      yyval->SetChild(2, yyvsp[-2]);
      yyval->SetChild(3, yyvsp[0]);
    ;
    break;}
case 32:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_FOREACH, (yyvsp[-2]) ? yyvsp[-2]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[-4]);
      yyval->SetChild(3, yyvsp[0]);
    ;
    break;}
case 33:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_FOREACH, (yyvsp[-2]) ? yyvsp[-2]->m_lineNumber : gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[-4]);
      yyval->SetChild(2, yyvsp[-6]);
      yyval->SetChild(3, yyvsp[0]);
    ;
    break;}
case 34:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_CONTINUE, gmlineno);
    ;
    break;}
case 35:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_BREAK, gmlineno);
    ;
    break;}
case 36:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_RETURN, gmlineno);
    ;
    break;}
case 37:
{
      yyval = gmCodeTreeNode::Create(CTNT_STATEMENT, CTNST_RETURN, gmlineno);
      yyval->SetChild(0, yyvsp[-1]);
    ;
    break;}
case 38:
{
      yyval = yyvsp[0];
      if(yyval)
      {
        yyval->m_flags |= gmCodeTreeNode::CTN_POP;
      }
    ;
    break;}
case 39:
{
      yyval = CreateOperation(CTNOT_ASSIGN, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 40:
{
      yyval = CreateAsignExpression(CTNOT_SHIFT_RIGHT, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 41:
{
      yyval = CreateAsignExpression(CTNOT_SHIFT_LEFT, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 42:
{
      yyval = CreateAsignExpression(CTNOT_ADD, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 43:
{
      yyval = CreateAsignExpression(CTNOT_MINUS, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 44:
{
      yyval = CreateAsignExpression(CTNOT_TIMES, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 45:
{
      yyval = CreateAsignExpression(CTNOT_DIVIDE, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 46:
{
      yyval = CreateAsignExpression(CTNOT_REM, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 47:
{
      yyval = CreateAsignExpression(CTNOT_BIT_AND, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 48:
{
      yyval = CreateAsignExpression(CTNOT_BIT_OR, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 49:
{
      yyval = CreateAsignExpression(CTNOT_BIT_XOR, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 50:
{
      yyval = NULL;
    ;
    break;}
case 51:
{
      yyval = yyvsp[-1];
    ;
    break;}
case 52:
{
      yyval = yyvsp[0];
    ;
    break;}
case 53:
{
      yyval = yyvsp[0];
    ;
    break;}
case 54:
{
      yyval = CreateOperation(CTNOT_OR, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 55:
{
      yyval = yyvsp[0];
    ;
    break;}
case 56:
{
      yyval = CreateOperation(CTNOT_AND, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 57:
{
      yyval = yyvsp[0];
    ;
    break;}
case 58:
{
      yyval = CreateOperation(CTNOT_BIT_OR, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 59:
{
      yyval = yyvsp[0];
    ;
    break;}
case 60:
{
      yyval = CreateOperation(CTNOT_BIT_XOR, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 61:
{
      yyval = yyvsp[0];
    ;
    break;}
case 62:
{
      yyval = CreateOperation(CTNOT_BIT_AND, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 63:
{
      yyval = yyvsp[0];
    ;
    break;}
case 64:
{
      yyval = CreateOperation(CTNOT_EQ, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 65:
{
      yyval = CreateOperation(CTNOT_NEQ, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 66:
{
      yyval = yyvsp[0];
    ;
    break;}
case 67:
{
      yyval = CreateOperation(CTNOT_LT, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 68:
{
      yyval = CreateOperation(CTNOT_GT, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 69:
{
      yyval = CreateOperation(CTNOT_LTE, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 70:
{
      yyval = CreateOperation(CTNOT_GTE, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 71:
{
      yyval = yyvsp[0];
    ;
    break;}
case 72:
{
      yyval = CreateOperation(CTNOT_SHIFT_LEFT, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 73:
{
      yyval = CreateOperation(CTNOT_SHIFT_RIGHT, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 74:
{
      yyval = yyvsp[0];
    ;
    break;}
case 75:
{
      yyval = CreateOperation(CTNOT_ADD, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 76:
{
      yyval = CreateOperation(CTNOT_MINUS, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 77:
{
      yyval = yyvsp[0];
    ;
    break;}
case 78:
{
      yyval = CreateOperation(CTNOT_TIMES, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 79:
{
      yyval = CreateOperation(CTNOT_DIVIDE, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 80:
{
      yyval = CreateOperation(CTNOT_REM, yyvsp[-2], yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 81:
{
      yyval = yyvsp[0];
    ;
    break;}
case 82:
{
      yyval = yyvsp[-1];
      yyval->SetChild(0, yyvsp[0]);
      yyval->ConstantFold();
    ;
    break;}
case 83:
{
      yyval = CreateOperation(CTNOT_UNARY_PLUS);
    ;
    break;}
case 84:
{
      yyval = CreateOperation(CTNOT_UNARY_MINUS);
    ;
    break;}
case 85:
{
      yyval = CreateOperation(CTNOT_UNARY_COMPLEMENT);
    ;
    break;}
case 86:
{
      yyval = CreateOperation(CTNOT_UNARY_ISNOTNULL);
    ;
    break;}
case 87:
{
      yyval = CreateOperation(CTNOT_UNARY_NOT);
    ;
    break;}
case 88:
{
      yyval = yyvsp[0];
    ;
    break;}
case 89:
{
      yyval = CreateOperation(CTNOT_ARRAY_INDEX, yyvsp[-2]);
    ;
    break;}
case 90:
{
      yyval = CreateOperation(CTNOT_ARRAY_INDEX, yyvsp[-3], yyvsp[-1]);
    ;
    break;}
case 91:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CALL, gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
    ;
    break;}
case 92:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CALL, gmlineno);
      yyval->SetChild(0, yyvsp[-3]);
      yyval->SetChild(1, yyvsp[-1]);
    ;
    break;}
case 93:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CALL, gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(2, yyvsp[-4]);
    ;
    break;}
case 94:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CALL, gmlineno);
      yyval->SetChild(0, yyvsp[-3]);
      yyval->SetChild(1, yyvsp[-1]);
      yyval->SetChild(2, yyvsp[-5]);
    ;
    break;}
case 95:
{
      yyval = CreateOperation(CTNOT_DOT, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 96:
{
      yyval = yyvsp[0];
    ;
    break;}
case 97:
{
      ATTACH(yyval, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 98:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_TABLE, gmlineno);
    ;
    break;}
case 99:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_TABLE, gmlineno);
      yyval->SetChild(0, yyvsp[-1]);
    ;
    break;}
case 100:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_TABLE, gmlineno);
    ;
    break;}
case 101:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_TABLE, gmlineno);
      yyval->SetChild(0, yyvsp[-1]);
    ;
    break;}
case 102:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_TABLE, gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
    ;
    break;}
case 103:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_FUNCTION, gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[0]);
    ;
    break;}
case 104:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_FUNCTION, gmlineno);
      yyval->SetChild(1, yyvsp[0]);
    ;
    break;}
case 105:
{
      yyval = yyvsp[0];
    ;
    break;}
case 106:
{
      ATTACH(yyval, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 107:
{
      yyval = yyvsp[0];
    ;
    break;}
case 108:
{
      yyval = CreateOperation(CTNOT_ASSIGN_FIELD, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 109:
{
      yyval = yyvsp[0];
    ;
    break;}
case 110:
{
      ATTACH(yyval, yyvsp[-2], yyvsp[0]);
    ;
    break;}
case 111:
{
      yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_PARAMETER, gmlineno);
      yyval->SetChild(0, yyvsp[0]);
    ;
    break;}
case 112:
{
      yyval = gmCodeTreeNode::Create(CTNT_DECLARATION, CTNDT_PARAMETER, gmlineno);
      yyval->SetChild(0, yyvsp[-2]);
      yyval->SetChild(1, yyvsp[0]);
    ;
    break;}
case 113:
{
      yyval = yyvsp[0];
    ;
    break;}
case 114:
{
      yyval = yyvsp[0];
      yyval->m_flags |= gmCodeTreeNode::CTN_MEMBER;
    ;
    break;}
case 115:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_THIS, gmlineno);
    ;
    break;}
case 116:
{
      yyval = yyvsp[0];
    ;
    break;}
case 117:
{
      yyval = yyvsp[0];
    ;
    break;}
case 118:
{
      yyval = yyvsp[0];
    ;
    break;}
case 119:
{
      yyval = yyvsp[-1];
    ;
    break;}
case 120:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_IDENTIFIER, gmlineno);
      yyval->m_data.m_string = (char *) gmCodeTree::Get().Alloc((int)strlen(gmtext) + 1);
      strcpy(yyval->m_data.m_string, gmtext);
    ;
    break;}
case 121:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_INT);
      yyval->m_data.m_iValue = (int)strtoul(gmtext + 2, NULL, 16);
    ;
    break;}
case 122:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_INT);
      yyval->m_data.m_iValue = (int)strtoul(gmtext + 2, NULL, 2);
    ;
    break;}
case 123:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_INT);
      yyval->m_data.m_iValue = atoi(gmtext);
    ;
    break;}
case 124:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_INT);
      yyval->m_data.m_iValue = 1;
    ;
    break;}
case 125:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_INT);
      yyval->m_data.m_iValue = 0;
    ;
    break;}
case 126:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_INT);

      char * c = (char *) gmCodeTree::Get().Alloc((int)strlen(gmtext) + 1);
      strcpy(c, gmtext);
      int result = 0;
      int shr = 0;

      while(*c)
      {
        if(c[0] == '\'')
        {
          ++c;
          continue;
        }
        else if(c[0] == '\\')
        {
          if(shr) result <<= 8;
          switch(c[1])
          {
            case 'a' : result |= (unsigned char) '\a'; break;
            case 'b' : result |= (unsigned char) '\b'; break;
            case 'f' : result |= (unsigned char) '\f'; break;
            case 'n' : result |= (unsigned char) '\n'; break;
            case 'r' : result |= (unsigned char) '\r'; break;
            case 't' : result |= (unsigned char) '\t'; break;
            case 'v' : result |= (unsigned char) '\v'; break;
            case '\'' : result |= (unsigned char) '\''; break;
            case '\"' : result |= (unsigned char) '\"'; break;
            case '\\' : result |= (unsigned char) '\\'; break;
            default: result |= (unsigned char) c[1];
          }
          ++shr;
          c += 2;
          continue;
        }
        if(shr) result <<= 8;
        result |= (unsigned char) *(c++);
        ++shr;
      }

      if(shr > 4 && gmCodeTree::Get().GetLog()) gmCodeTree::Get().GetLog()->LogEntry("truncated char, line %d", gmlineno);

      yyval->m_data.m_iValue = result;
    ;
    break;}
case 127:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_FLOAT);
      yyval->m_data.m_fValue = (float) atof(gmtext);
    ;
    break;}
case 128:
{
      yyval = yyvsp[0];
    ;
    break;}
case 129:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_NULL);
      yyval->m_data.m_iValue = 0;
    ;
    break;}
case 130:
{
      yyval = gmCodeTreeNode::Create(CTNT_EXPRESSION, CTNET_CONSTANT, gmlineno, CTNCT_STRING);
      yyval->m_data.m_string = (char *) gmCodeTree::Get().Alloc((int)strlen(gmtext) + 1);
      strcpy(yyval->m_data.m_string, gmtext);
      if(gmtext[0] == '"')
      {
        gmProcessDoubleQuoteString(yyval->m_data.m_string);
      }
      else if(gmtext[0] == '`')
      {
        gmProcessSingleQuoteString(yyval->m_data.m_string);
      }
    ;
    break;}
case 131:
{
      yyval = yyvsp[-1];
      int alen = (int)strlen(yyval->m_data.m_string);
      int blen = (int)strlen(gmtext);
      char * str = (char *) gmCodeTree::Get().Alloc(alen + blen + 1);
      if(str)
      {
        memcpy(str, yyvsp[-1]->m_data.m_string, alen);
        memcpy(str + alen, gmtext, blen);
        str[alen + blen] = '\0';
        if(str[alen] == '"')
        {
          gmProcessDoubleQuoteString(str + alen);
        }
        else if(str[alen] == '`')
        {
          gmProcessSingleQuoteString(str + alen);
        }
        yyval->m_data.m_string = str;
      }
    ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
        fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
        {
          int size = 0;
          char *msg;
          int x, count;

          count = 0;
          /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
          for (x = (yyn < 0 ? -yyn : 0);
               x < (int)(sizeof(yytname) / sizeof(char *)); x++) //_GD_
            if (yycheck[x + yyn] == x)
              size += (int)strlen(yytname[x]) + 15, count++; // _GD_ add cast for 64bit build
          //_GD_ msg = (char *) malloc(size + 15);
          msg = GM_NEW( char [size + 15] );
          if (msg != 0)
            {
              strcpy(msg, "parse error");

              if (count < 5)
                {
                  count = 0;
                  for (x = (yyn < 0 ? -yyn : 0);
                       x < (sizeof(yytname) / sizeof(char *)); x++)
                    if (yycheck[x + yyn] == x)
                      {
                        strcat(msg, count == 0 ? ", expecting `" : " or `");
                        strcat(msg, yytname[x]);
                        strcat(msg, "'");
                        count++;
                      }
                }
              yyerror(msg);
              //_GD_ free(msg);
              delete [] msg;
            }
          else
            yyerror ("parse error; also virtual memory exceeded");
        }
      else
#endif /* YYERROR_VERBOSE */
        yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
        YYABORT;

#if YYDEBUG != 0
      if (yydebug)
        fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
        fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
        goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}


#include <stdio.h>











