%{
#include "token.h"
%}

SPACE      [ \t\n]
DIGIT      [0-9]
LETTER     [A-Za-z] 
INT     ({DIGIT}*)
REAL ({DIGIT}*([.|,]{DIGIT}*))
IDENTIFIER ({LETTER})({DIGIT}|{LETTER}|_)*
TEXT       (\"({DIGIT}|{LETTER}|{SPACE})*\")
COMMENTL (@@({DIGIT}|{LETTER}|{TEXT}|{SPACE})*)
COMMENTML   (\@({DIGIT}|{LETTER}|{TEXT}|{SPACE})*\@)

COMMENT ({COMMENTL}|{COMMENTML})
%%
{SPACE} { }
"#" { return TOKEN_CONCAT; }
"rtos" { return TOKEN_RTOS; }
"itos"  { return TOKEN_ETOS; }
"itor"  { return TOKEN_ETOR; }
"rtoi"  { return TOKEN_RTOE; }
"(" { return TOKEN_LPAREN; }
")" { return TOKEN_RPAREN; }
"[" { return TOKEN_LCORCH; }
"]" { return TOKEN_RCORCH; }
"if" { return TOKEN_IF; }
"else" { return TOKEN_ELSE; }
"empty" { return TOKEN_EMPTY; }
"head" { return TOKEN_HEAD; }
"tail" { return TOKEN_TAIL; }  //resto de la lista sin el
"=" { return TOKEN_ASIG; }//cambiar a asignacion
{REAL} { return TOKEN_REAL; }
{INT} { return TOKEN_INT; }
"+" { return TOKEN_ADD; }
"-" { return TOKEN_SUBSTRACT; }
"*" { return TOKEN_MULTIPLY; }
"/" { return TOKEN_DIVIDE; }
"%" { return TOKEN_MOD; }
"and" { return TOKEN_AND; }
"or" { return TOKEN_OR; }
"not" { return TOKEN_NOT; }
"xor" { return TOKEN_XOR; }
"val" { return TOKEN_VAL; }
"let" { return TOKEN_LET; }
"in" { return TOKEN_IN; }
"fun" { return TOKEN_FUN; }
"print" { return TOKEN_PRINT; }
"pair" { return TOKEN_PAIR; }
"fst" { return TOKEN_FST; }
"snd" { return TOKEN_SND; }
"true" { return TOKEN_TRUE; }
"false" { return TOKEN_FALSE; }
"<" { return TOKEN_LESS; }
">" { return TOKEN_GREAT; }
"<=" { return TOKEN_LESSEQL; }
">=" { return TOKEN_GREATEQL; }
"!=" { return TOKEN_NOTEQUAL; }
"==" { return TOKEN_EQUAL; }
{COMMENT} { /*ignorar*/ }

{IDENTIFIER} { return TOKEN_IDENTIFIER; }
{TEXT}       { return TOKEN_STRING; }



%%
int yywrap() { return 1; }




