%{
    #include <stdio.h>
    #include <expression.hpp>
    #include <stdlib.h>
    #include <string.h>

    #define YYSTYPE Expression*


    extern int yylex();
    extern char* yytext;
    extern char* last_identifier;
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


expr : | and_expr                          { $$ = $1; }
     ;

and_expr :

equality_expr : 

relational_expr : 

concat_expr : concat_expr TOKEN_CONCAT additive_expr { $$ = new StringConcat($1, $3); }
            | additive_expr                           { $$ = $1; }
            ;

additive_expr : additive_expr TOKEN_ADD multiplicative_expr       { $$ = new Addition($1, $3); }
              | additive_expr TOKEN_SUBSTRACT multiplicative_expr { $$ = new Subtraction($1, $3); }
              | multiplicative_expr                               { $$ = $1; }
              ;

multiplicative_expr : multiplicative_expr TOKEN_MULTIPLY unary_expr { $$ = new Multiplication($1, $3); }
                    | multiplicative_expr TOKEN_DIVIDE unary_expr   { $$ = new Division($1, $3); }
                    | multiplicative_expr TOKEN_MOD unary_expr      { $$ = new Modulo($1, $3); }
                    | unary_expr                                    { $$ = $1; }
                    ;

unary_expr : TOKEN_NOT unary_expr                        { $$ = new LogicalNot($2); }
           | TOKEN_SUBSTRACT unary_expr                  { $$ = new Subtraction(new IntegerValue(0), $2); }
           | primary_expr                                { $$ = $1; }
           ;

primary_expr : TOKEN_LPAREN expr TOKEN_RPAREN           { $$ = $2; }
             | literal                                  { $$ = $1; }
             | TOKEN_IDENTIFIER                         { $$ = new Identifier(last_identifier); }
             ;

literal : TOKEN_INT                                     { $$ = new IntegerValue(atoi(yytext)); }
        | TOKEN_REAL                                    { $$ = new RealValue(atof(yytext)); }
        | TOKEN_STRING                                  { 
            // Remove quotes from string literal
            char* str = strdup(yytext + 1);
            str[strlen(str) - 1] = '\0';
            $$ = new StringValue(str);
            free(str);
        }
        | TOKEN_TRUE                                    { $$ = new BooleanValue(true); }
        | TOKEN_FALSE                                   { $$ = new BooleanValue(false); }
        ;
      
              ;
%% /* ---------- user code ---------- */

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
        return 1;
    } 