%{
#include "token.h"
%}

SPACE      [ \t\n]
DIGIT      [0-9]
LETTER     [A-Za-z] 
ENTERO     (([1-9]+){DIGIT}*)
FLOTANTE ({DIGIT}([.]{DIGIT}*){ENTERO}{DIGIT}*)|({ENTERO}+[.]{DIGIT}+)
NUMERO {ENTERO}|{FLOTANTE}
IDENTIFIER ({LETTER})({DIGIT}|{LETTER}|_)*
TEXT       \"({DIGIT}|{LETTER}|{SPACE})*\"


%%
{SPACE} { }

"concat" { return TOKEN_CONCAT; }
"converNRtoS" { return TOKEN_CONVER_NRTOS; }
"converNEtoS"  { return TOKEN_CONVER_NETOS; }
"converNEtoNR"  { return TOKEN_CONVER_NETONR; }
"converNRtoNE"  { return TOKEN_CONVER_NRTONE; }
"(" { return TOKEN_LPAREN; }
")" { return TOKEN_RPAREN; }
"[" { return TOKEN_LCORCH; }
"]" { return TOKEN_RCORCH; }
"if" { return TOKEN_IF; }
"else" { return TOKEN_ELSE; }
"empty" { return TOKEN_EMPTY; }
"head" { return TOKEN_HEAD; }
"tail" { return TOKEN_TAIL; }
"=" { return TOKEN_ASIG; }//cambiar a asignacion
{ENTERO} { return TOKEN_NENTERO; }
{FLOTANTE} { return TOKEN_NREAL; }
"+" { return TOKEN_ADD; }
"-" { return TOKEN_SUBSTRACT; }
"*" { return TOKEN_MULTIPLY; }
"/" { return TOKEN_DIVIDE; }
"%" { return TOKEN_RESTODIV; }
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
"<" { return TOKEN_MEN; }
">" { return TOKEN_MAY; }
"<=" { return TOKEN_MENEQL; }
">=" { return TOKEN_MAYEQL; }
"!=" { return TOKEN_DIF; }
"==" { return TOKEN_EQUAL; }
{IDENTIFIER} { return TOKEN_IDENTIFIER; }
{TEXT}       { return TOKEN_STRING; }
"@" { return TOKEN_CONMEN; }
"@/" { return TOKEN_LCONMEN; }
"/@" { return TOKEN_RCONMEN; }



%%
int yywrap() { return 1; }




