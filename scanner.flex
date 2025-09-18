%{
#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


char* function_name = nullptr;
char* last_identifier = nullptr;

int let_context = 0;
%}

SPACE      [ \t\n]
DIGIT      [0-9]
LETTER     [A-Za-z] 
INT     ({DIGIT}+)
REAL ({DIGIT}+([.]{DIGIT}+))
IDENTIFIER ({LETTER})({DIGIT}|{LETTER}|_)*
TEXT       (\"({DIGIT}|{LETTER}|{SPACE}|[+\-*/])*\")
COMMENTL (@@[^@]*)
COMMENTML   @[^@]*@
COMMENT ({COMMENTL}|{COMMENTML})
NARRAY (<{DIGIT}+>)
%%
{SPACE} { }
"#" { return TOKEN_CONCAT; }
"rtos" { return TOKEN_RTOS; }
"itos"  { return TOKEN_ETOS; }
"itor"  { return TOKEN_ETOR; }
"rtoi"  { return TOKEN_RTOE; }
"isunit"  { return TOKEN_ISUNIT; }
"unit"  { return TOKEN_UNIT; }
"(" { return TOKEN_LPAREN; }
")" { return TOKEN_RPAREN; }
"[" { return TOKEN_LCORCH; }
"]" { return TOKEN_RCORCH; }
"if" { return TOKEN_IF; }
"else" { return TOKEN_ELSE; }
"empty" { return TOKEN_EMPTY; }
"head" { return TOKEN_HEAD; }
"tail" { return TOKEN_TAIL; }  //resto de la lista sin el
"length" { return TOKEN_LENGTH; }
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
"let" { 
    let_context = 1; 
    return TOKEN_LET; 
}
"true" { return TOKEN_TRUE; }
"false" { return TOKEN_FALSE; }
"in" { 
    let_context = 0; 
    return TOKEN_IN; 
}
"fun" { return TOKEN_FUN; }
"print" { return TOKEN_PRINT; }
"fst" { return TOKEN_FST; }
"snd" { return TOKEN_SND; }
"<" { return TOKEN_LESS; }
">" { return TOKEN_GREAT; }
"<=" { return TOKEN_LESSEQL; }
">=" { return TOKEN_GREATEQL; }
"!=" { return TOKEN_NOTEQUAL; }
"==" { return TOKEN_EQUAL; }
"end" { return TOKEN_END;}
"," { return TOKEN_COMA;}
"<+>" { return TOKEN_ADD_ARRAY;}
"<->" { return TOKEN_DEL_ARRAY;}
{NARRAY} { return TOKEN_NARRAY; }


{COMMENT} {/*ignorar*/}

{IDENTIFIER} {
    if (last_identifier != nullptr) {
        free(last_identifier);
    }
    last_identifier = strdup(yytext);
    
    // Check if this is followed by a parenthesis (function call)
    int c = yyinput();
    if (c == '(') {
        // This is a function call, save the function name
        if (function_name != nullptr) {
            free(function_name);
        }
        function_name = strdup(yytext);
        unput(c); // Put back the '('
    } else {
        unput(c); // Put back the character
    }
    return TOKEN_IDENTIFIER;
}
{TEXT}       { return TOKEN_STRING; }

. {
    return TOKEN_UNKNOWN;}

%%


int yywrap() { return 1; }

void cleanup_lexer(){
    if(last_identifier){
        free(last_identifier);
        last_identifier = nullptr;
    }
    if(function_name){
        free(function_name);
        function_name = nullptr;
    }
}




