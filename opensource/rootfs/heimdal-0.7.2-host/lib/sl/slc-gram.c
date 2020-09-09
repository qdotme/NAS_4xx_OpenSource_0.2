/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LITERAL = 258,
     STRING = 259
   };
#endif
#define LITERAL 258
#define STRING 259




/* Copy the first part of user declarations.  */
#line 1 "slc-gram.y"

/*
 * Copyright (c) 2004 Kungliga Tekniska Högskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
RCSID("$Id: slc-gram.y,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $");
#endif

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <ctype.h>
#include <limits.h>
#include <getarg.h>
#include <vers.h>
#include <roken.h>

#include "slc.h"
extern FILE *yyin;
extern struct assignment *a;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 54 "slc-gram.y"
typedef union YYSTYPE {
	char *string;
	struct assignment *assignment;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 141 "slc-gram.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 153 "slc-gram.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   7

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  8
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  4
/* YYNRULES -- Number of rules. */
#define YYNRULES  6
/* YYNRULES -- Number of states. */
#define YYNSTATES  12

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   259

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     5,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     6,     2,     7,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    14
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
       9,     0,    -1,    10,    -1,    11,    10,    -1,    11,    -1,
       3,     5,     4,    -1,     3,     5,     6,    10,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    67,    67,    73,    78,    81,    90
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LITERAL", "STRING", "'='", "'{'", "'}'", 
  "$accept", "start", "assignments", "assignment", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,    61,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,     8,     9,    10,    10,    11,    11
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     3,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     2,     4,     0,     1,     3,     5,     0,
       0,     6
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     3,     4
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -5
static const yysigned_char yypact[] =
{
      -1,     1,     4,    -5,    -1,    -3,    -5,    -5,    -5,    -1,
       0,    -5
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
      -5,    -5,    -4,    -5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
       7,     8,     1,     9,     6,    10,     5,    11
};

static const unsigned char yycheck[] =
{
       4,     4,     3,     6,     0,     9,     5,     7
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     9,    10,    11,     5,     0,    10,     4,     6,
      10,     7
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 68 "slc-gram.y"
    {
			a = yyvsp[0].assignment;
		}
    break;

  case 3:
#line 74 "slc-gram.y"
    {
			yyvsp[-1].assignment->next = yyvsp[0].assignment;
			yyval.assignment = yyvsp[-1].assignment;
		}
    break;

  case 5:
#line 82 "slc-gram.y"
    {
			yyval.assignment = malloc(sizeof(*yyval.assignment));
			yyval.assignment->name = yyvsp[-2].string;
			yyval.assignment->type = a_value;
			yyval.assignment->lineno = lineno;
			yyval.assignment->u.value = yyvsp[0].string;
			yyval.assignment->next = NULL;
		}
    break;

  case 6:
#line 91 "slc-gram.y"
    {
			yyval.assignment = malloc(sizeof(*yyval.assignment));
			yyval.assignment->name = yyvsp[-4].string;
			yyval.assignment->type = a_assignment;
			yyval.assignment->lineno = lineno;
			yyval.assignment->u.assignment = yyvsp[-1].assignment;
			yyval.assignment->next = NULL;
		}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1057 "slc-gram.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 101 "slc-gram.y"

char *filename;
FILE *cfile, *hfile;
int error_flag;
struct assignment *a;


static void
ex(struct assignment *a, const char *fmt, ...)
{
    va_list ap;
    fprintf(stderr, "%s:%d: ", a->name, a->lineno);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}



static int
check_option(struct assignment *as)
{
    struct assignment *a;
    int seen_long = 0;
    int seen_short = 0;
    int seen_type = 0;
    int seen_argument = 0;
    int seen_help = 0;
    int seen_default = 0;
    int ret = 0;

    for(a = as; a != NULL; a = a->next) {
	if(strcmp(a->name, "long") == 0)
	    seen_long++;
	else if(strcmp(a->name, "short") == 0)
	    seen_short++;
	else if(strcmp(a->name, "type") == 0)
	    seen_type++;
	else if(strcmp(a->name, "argument") == 0)
	    seen_argument++;
	else if(strcmp(a->name, "help") == 0)
	    seen_help++;
	else if(strcmp(a->name, "default") == 0)
	    seen_default++;
	else {
	    ex(a, "unknown name");
	    ret++;
	}
    }
    if(seen_long == 0 && seen_short == 0) {
	ex(as, "neither long nor short option");
	ret++;
    }
    if(seen_long > 1) {
	ex(as, "multiple long options");
	ret++;
    }
    if(seen_short > 1) {
	ex(as, "multiple short options");
	ret++;
    }
    if(seen_type > 1) {
	ex(as, "multiple types");
	ret++;
    }
    if(seen_argument > 1) {
	ex(as, "multiple arguments");
	ret++;
    }
    if(seen_help > 1) {
	ex(as, "multiple help strings");
	ret++;
    }
    if(seen_default > 1) {
	ex(as, "multiple default values");
	ret++;
    }
    return ret;
}

static int
check_command(struct assignment *as)
{
	struct assignment *a;
	int seen_name = 0;
	int seen_function = 0;
	int seen_help = 0;
	int seen_argument = 0;
	int seen_minargs = 0;
	int seen_maxargs = 0;
	int ret = 0;
	for(a = as; a != NULL; a = a->next) {
		if(strcmp(a->name, "name") == 0)
			seen_name++;
		else if(strcmp(a->name, "function") == 0) {
			seen_function++;
		} else if(strcmp(a->name, "option") == 0)
			ret += check_option(a->u.assignment);
		else if(strcmp(a->name, "help") == 0) {
			seen_help++;
		} else if(strcmp(a->name, "argument") == 0) {
			seen_argument++;
		} else if(strcmp(a->name, "min_args") == 0) {
			seen_minargs++;
		} else if(strcmp(a->name, "max_args") == 0) {
			seen_maxargs++;
		} else {
			ex(a, "unknown name");
			ret++;
		}
	}
	if(seen_name == 0) {
		ex(as, "no command name");
		ret++;
	}
	if(seen_function > 1) {
		ex(as, "multiple function names");
		ret++;
	}
	if(seen_help > 1) {
		ex(as, "multiple help strings");
		ret++;
	}
	if(seen_argument > 1) {
		ex(as, "multiple argument strings");
		ret++;
	}
	if(seen_minargs > 1) {
		ex(as, "multiple min_args strings");
		ret++;
	}
	if(seen_maxargs > 1) {
		ex(as, "multiple max_args strings");
		ret++;
	}
	
	return ret;
}

static int
check(struct assignment *as)
{
    struct assignment *a;
    int ret = 0;
    for(a = as; a != NULL; a = a->next) {
	if(strcmp(a->name, "command")) {
	    fprintf(stderr, "unknown type %s line %d\n", a->name, a->lineno);
	    ret++;
	    continue;
	}
	if(a->type != a_assignment) {
	    fprintf(stderr, "bad command definition %s line %d\n", a->name, a->lineno);
	    ret++;
	    continue;
	}
	ret += check_command(a->u.assignment);
    }
    return ret;
}

static struct assignment *
find_next(struct assignment *as, const char *name)
{
    for(as = as->next; as != NULL; as = as->next) {
	if(strcmp(as->name, name) == 0)
	    return as;
    }
    return NULL;
}

static struct assignment *
find(struct assignment *as, const char *name)
{
    for(; as != NULL; as = as->next) {
	if(strcmp(as->name, name) == 0)
	    return as;
    }
    return NULL;
}

static void
space(FILE *f, int level)
{
    fprintf(f, "%*.*s", level * 4, level * 4, " ");
}

static void
cprint(int level, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    space(cfile, level);
    vfprintf(cfile, fmt, ap);
    va_end(ap);
}

static void
hprint(int level, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    space(hfile, level);
    vfprintf(hfile, fmt, ap);
    va_end(ap);
}

static void gen_name(char *str);

static void
gen_command(struct assignment *as)
{
    struct assignment *a, *b;
    char *f;
    a = find(as, "name");
    f = strdup(a->u.value);
    gen_name(f);
    cprint(1, "    { ");
    fprintf(cfile, "\"%s\", ", a->u.value);
    fprintf(cfile, "%s_wrap, ", f);
    b = find(as, "argument");
    if(b)
	fprintf(cfile, "\"%s %s\", ", a->u.value, b->u.value);
    else
	fprintf(cfile, "\"%s\", ", a->u.value);
    b = find(as, "help");
    if(b)
	fprintf(cfile, "\"%s\"", b->u.value);
    else
	fprintf(cfile, "NULL");
    fprintf(cfile, " },\n");
    for(a = a->next; a != NULL; a = a->next)
	if(strcmp(a->name, "name") == 0)
	    cprint(1, "    { \"%s\" },\n", a->u.value);
    cprint(0, "\n");
}

static void
gen_name(char *str)
{
    char *p;
    for(p = str; *p != '\0'; p++)
	if(!isalnum((unsigned char)*p))
	    *p = '_';
}

static char *
make_name(struct assignment *as)
{
    struct assignment *lopt;
    struct assignment *type;
    char *s;

    lopt = find(as, "long");
    if(lopt == NULL)
	lopt = find(as, "name");
    if(lopt == NULL)
	return NULL;
    
    type = find(as, "type");
    if(strcmp(type->u.value, "-flag") == 0)
	asprintf(&s, "%s_flag", lopt->u.value);
    else
	asprintf(&s, "%s_%s", lopt->u.value, type->u.value);
    gen_name(s);
    return s;
}

static void
gen_options(struct assignment *opt1, const char *name)
{
    struct assignment *tmp;

    hprint(0, "struct %s_options {\n", name);

    for(tmp = opt1; 
	tmp != NULL; 
	tmp = find_next(tmp, "option")) {
	struct assignment *type;
	char *s;
	
	s = make_name(tmp->u.assignment);
	type = find(tmp->u.assignment, "type");
	if(strcmp(type->u.value, "string") == 0)
	    hprint(1, "char *%s;\n", s);
	else if(strcmp(type->u.value, "strings") == 0)
	    hprint(1, "struct getarg_strings %s;\n", s);
	else if(strcmp(type->u.value, "integer") == 0)
	    hprint(1, "int %s;\n", s);
	else if(strcmp(type->u.value, "flag") == 0) 
	    hprint(1, "int %s;\n", s);
	else if(strcmp(type->u.value, "-flag") == 0) 
	    hprint(1, "int %s;\n", s);
	else {
	    ex(type, "unknown type \"%s\"", type->u.value);
	    exit(1);
	}
	free(s);
    }
    hprint(0, "};\n");
}

static void
gen_wrapper(struct assignment *as)
{
    struct assignment *name;
    struct assignment *arg;
    struct assignment *opt1;
    struct assignment *function;
    struct assignment *tmp;
    char *f;
    int nargs = 0;
    int seen_strings = 0;

    name = find(as, "name");
    arg = find(as, "argument");
    opt1 = find(as, "option");
    function = find(as, "function");
    if(function == NULL)
	function = name;
    f = strdup(name->u.value);
    gen_name(f);
       
    if(opt1 != NULL) {
	gen_options(opt1, name->u.value);
	hprint(0, "int %s(struct %s_options*, int, char **);\n", 
	       function->u.value, name->u.value);
    } else {
	hprint(0, "int %s(void*, int, char **);\n", 
	       function->u.value);
    }

    fprintf(cfile, "static int\n");
    fprintf(cfile, "%s_wrap(int argc, char **argv)\n", f);
    fprintf(cfile, "{\n");
    if(opt1 != NULL)
	cprint(1, "struct %s_options opt;\n", name->u.value);
    cprint(1, "int ret;\n");
    cprint(1, "int optind = 0;\n");
    cprint(1, "struct getargs args[] = {\n");
    for(tmp = find(as, "option"); 
	tmp != NULL; 
	tmp = find_next(tmp, "option")) {
	struct assignment *type = find(tmp->u.assignment, "type");
	struct assignment *lopt = find(tmp->u.assignment, "long");
	struct assignment *sopt = find(tmp->u.assignment, "short");
	struct assignment *arg = find(tmp->u.assignment, "argument");
	struct assignment *help = find(tmp->u.assignment, "help");
	
	cprint(2, "{ ");
	if(lopt)
	    fprintf(cfile, "\"%s\", ", lopt->u.value);
	else
	    fprintf(cfile, "NULL, ");
	if(sopt)
	    fprintf(cfile, "'%c', ", *sopt->u.value);
	else
	    fprintf(cfile, "0, ");
	if(strcmp(type->u.value, "string") == 0)
	    fprintf(cfile, "arg_string, ");
	else if(strcmp(type->u.value, "strings") == 0)
	    fprintf(cfile, "arg_strings, ");
	else if(strcmp(type->u.value, "integer") == 0)
	    fprintf(cfile, "arg_integer, ");
	else if(strcmp(type->u.value, "flag") == 0)
	    fprintf(cfile, "arg_flag, ");
	else if(strcmp(type->u.value, "-flag") == 0)
	    fprintf(cfile, "arg_negative_flag, ");
	else {
	    ex(type, "unknown type \"%s\"", type->u.value);
	    exit(1);
	}
	fprintf(cfile, "NULL, ");
	if(help)
	    fprintf(cfile, "\"%s\", ", help->u.value);
	else
	    fprintf(cfile, "NULL, ");
	if(arg)
	    fprintf(cfile, "\"%s\"", arg->u.value);
	else
	    fprintf(cfile, "NULL");
	fprintf(cfile, " },\n");
    }
    cprint(2, "{ \"help\", 'h', arg_flag, NULL, NULL, NULL }\n");
    cprint(1, "};\n");
    cprint(1, "int help_flag = 0;\n");

    for(tmp = find(as, "option"); 
	tmp != NULL; 
	tmp = find_next(tmp, "option")) {
	char *s;
	struct assignment *type = find(tmp->u.assignment, "type");

	struct assignment *defval = find(tmp->u.assignment, "default");
	s = make_name(tmp->u.assignment);
	if(strcmp(type->u.value, "string") == 0) {
	    if(defval != NULL)
		cprint(1, "opt.%s = \"%s\";\n", s, defval->u.value);
	    else
		cprint(1, "opt.%s = NULL;\n", s);
	} else if(strcmp(type->u.value, "strings") == 0) {
	    seen_strings = 1;
	    cprint(1, "opt.%s.num_strings = 0;\n", s);
	    cprint(1, "opt.%s.strings = NULL;\n", s);
	} else if(strcmp(type->u.value, "integer") == 0) {
	    if(defval != NULL)
		cprint(1, "opt.%s = %s;\n", s, defval->u.value);
	    else
		cprint(1, "opt.%s = 0;\n", s);
	} else if(strcmp(type->u.value, "flag") == 0 ||
		  strcmp(type->u.value, "-flag") == 0) {
	    if(defval != NULL)
		cprint(1, "opt.%s = %s;\n", s, defval->u.value);
	    else
		cprint(1, "opt.%s = 0;\n", s);
	} else {
	    ex(type, "unknown type \"%s\"", type->u.value);
	    exit(1);
	}
	free(s);
    }

    for(tmp = find(as, "option"); 
	tmp != NULL; 
	tmp = find_next(tmp, "option")) {
	char *s;
	s = make_name(tmp->u.assignment);
	cprint(1, "args[%d].value = &opt.%s;\n", nargs++, s);
	free(s);
    }
    cprint(1, "args[%d].value = &help_flag;\n", nargs++);
    cprint(1, "if(getarg(args, %d, argc, argv, &optind))\n", nargs);
    cprint(2, "goto usage;\n");

    {
	int min_args = -1;
	int max_args = -1;
	char *end;
	if(arg == NULL) {
	    max_args = 0;
	} else {
	    if((tmp = find(as, "min_args")) != NULL) {
		min_args = strtol(tmp->u.value, &end, 0);
		if(*end != '\0') {
		    ex(tmp, "min_args is not numeric");
		    exit(1);
		}
		if(min_args < 0) {
		    ex(tmp, "min_args must be non-negative");
		    exit(1);
		}
	    }
	    if((tmp = find(as, "max_args")) != NULL) {
		max_args = strtol(tmp->u.value, &end, 0);
		if(*end != '\0') {
		    ex(tmp, "max_args is not numeric");
		    exit(1);
		}
		if(max_args < 0) {
		    ex(tmp, "max_args must be non-negative");
		    exit(1);
		}
	    }
	}
	if(min_args != -1 || max_args != -1) {
	    if(min_args == max_args) {
		cprint(1, "if(argc - optind != %d) {\n", 
		       min_args);
		cprint(2, "fprintf(stderr, \"Need exactly %u parameters (%%u given).\\n\\n\", argc - optind);\n", min_args);
		cprint(2, "goto usage;\n");
		cprint(1, "}\n");
	    } else {
		if(max_args != -1) {
		    cprint(1, "if(argc - optind > %d) {\n", max_args);
		    cprint(2, "fprintf(stderr, \"Arguments given (%%u) are more than expected (%u).\\n\\n\", argc - optind);\n", max_args);
		    cprint(2, "goto usage;\n");
		    cprint(1, "}\n");
		}
		if(min_args != -1) {
		    cprint(1, "if(argc - optind < %d) {\n", min_args);
		    cprint(2, "fprintf(stderr, \"Arguments given (%%u) are less than expected (%u).\\n\\n\", argc - optind);\n", min_args);
		    cprint(2, "goto usage;\n");
		    cprint(1, "}\n");
		}
	    }
	}
    }
    
    cprint(1, "if(help_flag)\n");
    cprint(2, "goto usage;\n");

    cprint(1, "ret = %s(%s, argc - optind, argv + optind);\n", 
	   function->u.value, 
	   opt1 ? "&opt": "NULL");
    if(seen_strings) {
	if(seen_strings) {
	    for(tmp = find(as, "option"); 
		tmp != NULL; 
		tmp = find_next(tmp, "option")) {
		char *s;
		struct assignment *type = find(tmp->u.assignment, "type");
		
		s = make_name(tmp->u.assignment);
		if(strcmp(type->u.value, "strings") == 0) {
		    cprint(1, "free_getarg_strings (&opt.%s);\n", s);
		} 
		free(s);
	    }
	}
    }
    cprint(1, "return ret;\n");

    cprint(0, "usage:\n");
    cprint(1, "arg_printusage (args, %d, \"%s\", \"%s\");\n", nargs, 
	   name->u.value, arg ? arg->u.value : "");
    if(seen_strings) {
	for(tmp = find(as, "option"); 
	    tmp != NULL; 
	    tmp = find_next(tmp, "option")) {
	    char *s;
	    struct assignment *type = find(tmp->u.assignment, "type");

	    s = make_name(tmp->u.assignment);
	    if(strcmp(type->u.value, "strings") == 0) {
		cprint(1, "free_getarg_strings (&opt.%s);\n", s);
	    } 
	    free(s);
	}
    }
    cprint(1, "return 0;\n");
    cprint(0, "}\n");
    cprint(0, "\n");
}

char cname[PATH_MAX];
char hname[PATH_MAX];

static void
gen(struct assignment *as)
{
    struct assignment *a;
    cprint(0, "#include <stdio.h>\n");
    cprint(0, "#include <getarg.h>\n");
    cprint(0, "#include <sl.h>\n");
    cprint(0, "#include \"%s\"\n\n", hname);

    hprint(0, "#include <stdio.h>\n");
    hprint(0, "#include <sl.h>\n");
    hprint(0, "\n");


    for(a = as; a != NULL; a = a->next)
	gen_wrapper(a->u.assignment);

    cprint(0, "SL_cmd commands[] = {\n");
    for(a = as; a != NULL; a = a->next)
	gen_command(a->u.assignment);
    cprint(1, "{ NULL }\n");
    cprint(0, "};\n");

    hprint(0, "extern SL_cmd commands[];\n");
}

int version_flag;
int help_flag;
struct getargs args[] = {
    { "version", 0, arg_flag, &version_flag },
    { "help", 0, arg_flag, &help_flag }
};
int num_args = sizeof(args) / sizeof(args[0]);

static void
usage(int code)
{
    arg_printusage(args, num_args, NULL, "command-table");
    exit(code);
}

int
main(int argc, char **argv)
{
    char *p;

    int optind = 0;

    setprogname(argv[0]);
    if(getarg(args, num_args, argc, argv, &optind))
	usage(1);
    if(help_flag)
	usage(0);
    if(version_flag) {
	print_version(NULL);
	exit(0);
    }
    
    if(argc == optind)
	usage(1);

    filename = argv[optind];
    yyin = fopen(filename, "r");
    if(yyin == NULL)
	err(1, "%s", filename);
    p = strrchr(filename, '/');
    if(p)
	strlcpy(cname, p + 1, sizeof(cname));
    else
	strlcpy(cname, filename, sizeof(cname));
    p = strrchr(cname, '.');
    if(p)
	*p = '\0';
    strlcpy(hname, cname, sizeof(hname));
    strlcat(cname, ".c", sizeof(cname));
    strlcat(hname, ".h", sizeof(hname));
    yyparse();
    if(error_flag)
	exit(1);
    if(check(a) == 0) {
	cfile = fopen(cname, "w");
	if(cfile == NULL)
	  err(1, "%s", cname);
	hfile = fopen(hname, "w");
	if(hfile == NULL)
	  err(1, "%s", hname);
	gen(a);
	fclose(cfile);
	fclose(hfile);
    }
    return 0;
}

