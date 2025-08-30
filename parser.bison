%{
    #include <stdio.h>
    #include <expression.hpp>

    #define YYSTYPE Expression*


    extern int yylex();
    extern char* yytext;
    int yyerror(const char*);
    
    Expression* parser_result{nullptr};
%}

%token TOKEN_EOF
%token TOKEN_IF
%token TOKEN_ELSE

%token TOKEN_CONCAT
%token TOKEN_RTOS
%token TOKEN_ETOS
%token TOKEN_ETOR
%token TOKEN_RTOE
    
%token TOKEN_EMPTY
%token TOKEN_HEAD
%token TOKEN_TAIL
    
%token TOKEN_ASIG
%token TOKEN_ADD
%token TOKEN_SUBSTRACT
%token TOKEN_MULTIPLY
%token TOKEN_DIVIDE
%token TOKEN_MOD

%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_NOT
%token TOKEN_XOR

%token TOKEN_VAL
%token TOKEN_LET
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_INT
%token TOKEN_REAL
%token TOKEN_STRING
%token TOKEN_FUN
%token TOKEN_IN
%token TOKEN_IDENTIFIER
%token TOKEN_UNKNOWN

%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LCORCH
%token TOKEN_RCORCH

%token TOKEN_PAIR
%token TOKEN_FST
%token TOKEN_SND

%token TOKEN_LESS
%token TOKEN_GREAT
%token TOKEN_LESSEQL
%token TOKEN_GREATEQL
%token TOKEN_NOTEQUAL
%token TOKEN_EQUAL

%token TOKEN_CONMMENT
%token TOKEN_LCONMEN
%token TOKEN_RCONMEN

%token  TOKEN_PRINT


%% /* ---------- grammar ---------- */


    program : expr                           { parser_result = $1; }
        ;

expr : expr TOKEN_ADD term              { $$ = new Addition($1, $3); }
     | expr TOKEN_SUBSTRACT term             { $$ = new Subtraction($1, $3); }
     | term                              { $$ = $1; }
     ;

term : term TOKEN_MULTIPLY factor             { $$ = new Multiplication($1, $3); }
     | term TOKEN_DIVIDE factor             { $$ = new Division($1, $3); }
     | term TOKEN_MOD factor             { $$ = new Module($1, $3); }
     | factor                            { $$ = $1; }
     ;

factor : TOKEN_SUBSTRACT factor              { $$ = new Subtraction(new Value(0), $2); }
       | TOKEN_LPAREN expr TOKEN_RPAREN  { $$ = $2; }
       | TOKEN_INT                       { $$ = new Value(atoi(yytext)); }
       ;
%% /* ---------- user code ---------- */

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
        return 1;
    } 