/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_EOF = 258,
     TOKEN_IF = 259,
     TOKEN_ELSE = 260,
     TOKEN_CONCAT = 261,
     TOKEN_RTOS = 262,
     TOKEN_ETOS = 263,
     TOKEN_ETOR = 264,
     TOKEN_RTOE = 265,
     TOKEN_EMPTY = 266,
     TOKEN_HEAD = 267,
     TOKEN_TAIL = 268,
     TOKEN_ASIG = 269,
     TOKEN_ADD = 270,
     TOKEN_SUBSTRACT = 271,
     TOKEN_MULTIPLY = 272,
     TOKEN_DIVIDE = 273,
     TOKEN_MOD = 274,
     TOKEN_AND = 275,
     TOKEN_OR = 276,
     TOKEN_NOT = 277,
     TOKEN_XOR = 278,
     TOKEN_VAL = 279,
     TOKEN_LET = 280,
     TOKEN_TRUE = 281,
     TOKEN_FALSE = 282,
     TOKEN_INT = 283,
     TOKEN_REAL = 284,
     TOKEN_STRING = 285,
     TOKEN_FUN = 286,
     TOKEN_IN = 287,
     TOKEN_IDENTIFIER = 288,
     TOKEN_UNKNOWN = 289,
     TOKEN_LPAREN = 290,
     TOKEN_RPAREN = 291,
     TOKEN_LCORCH = 292,
     TOKEN_RCORCH = 293,
     TOKEN_PAIR = 294,
     TOKEN_FST = 295,
     TOKEN_SND = 296,
     TOKEN_LESS = 297,
     TOKEN_GREAT = 298,
     TOKEN_LESSEQL = 299,
     TOKEN_GREATEQL = 300,
     TOKEN_NOTEQUAL = 301,
     TOKEN_EQUAL = 302,
     TOKEN_CONMMENT = 303,
     TOKEN_LCONMEN = 304,
     TOKEN_RCONMEN = 305,
     TOKEN_COMA = 306,
     TOKEN_END = 307,
     TOKEN_PRINT = 308,
     TOKEN_DEL_ARRAY = 309,
     TOKEN_ADD_ARRAY = 310,
     TOKEN_NARRAY = 311,
     TOKEN_NOT_EQUAL = 312,
     TOKEN_GREATER_EQ = 313,
     TOKEN_LESS_EQ = 314,
     TOKEN_GREATER = 315
   };
#endif
/* Tokens.  */
#define TOKEN_EOF 258
#define TOKEN_IF 259
#define TOKEN_ELSE 260
#define TOKEN_CONCAT 261
#define TOKEN_RTOS 262
#define TOKEN_ETOS 263
#define TOKEN_ETOR 264
#define TOKEN_RTOE 265
#define TOKEN_EMPTY 266
#define TOKEN_HEAD 267
#define TOKEN_TAIL 268
#define TOKEN_ASIG 269
#define TOKEN_ADD 270
#define TOKEN_SUBSTRACT 271
#define TOKEN_MULTIPLY 272
#define TOKEN_DIVIDE 273
#define TOKEN_MOD 274
#define TOKEN_AND 275
#define TOKEN_OR 276
#define TOKEN_NOT 277
#define TOKEN_XOR 278
#define TOKEN_VAL 279
#define TOKEN_LET 280
#define TOKEN_TRUE 281
#define TOKEN_FALSE 282
#define TOKEN_INT 283
#define TOKEN_REAL 284
#define TOKEN_STRING 285
#define TOKEN_FUN 286
#define TOKEN_IN 287
#define TOKEN_IDENTIFIER 288
#define TOKEN_UNKNOWN 289
#define TOKEN_LPAREN 290
#define TOKEN_RPAREN 291
#define TOKEN_LCORCH 292
#define TOKEN_RCORCH 293
#define TOKEN_PAIR 294
#define TOKEN_FST 295
#define TOKEN_SND 296
#define TOKEN_LESS 297
#define TOKEN_GREAT 298
#define TOKEN_LESSEQL 299
#define TOKEN_GREATEQL 300
#define TOKEN_NOTEQUAL 301
#define TOKEN_EQUAL 302
#define TOKEN_CONMMENT 303
#define TOKEN_LCONMEN 304
#define TOKEN_RCONMEN 305
#define TOKEN_COMA 306
#define TOKEN_END 307
#define TOKEN_PRINT 308
#define TOKEN_DEL_ARRAY 309
#define TOKEN_ADD_ARRAY 310
#define TOKEN_NARRAY 311
#define TOKEN_NOT_EQUAL 312
#define TOKEN_GREATER_EQ 313
#define TOKEN_LESS_EQ 314
#define TOKEN_GREATER 315




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

