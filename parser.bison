%{
    #include <stdio.h>
    #include "expression.hpp"
    #include <stdlib.h>
    #include <string.h>

    #define YYSTYPE Expression*

    extern int yylex();
    extern char* yytext;
    extern char* last_identifier;
    extern char* prev_identifier;
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
%token TOKEN_COMA
%token TOKEN_END

%token  TOKEN_PRINT

%token  TOKEN_DEL_ARRAY
%token  TOKEN_ADD_ARRAY
%token  TOKEN_NARRAY

%right TOKEN_IN
%left TOKEN_AND
%left TOKEN_OR
%left TOKEN_XOR
%left TOKEN_EQUAL TOKEN_NOT_EQUAL
%left TOKEN_LESS TOKEN_GREATER TOKEN_LESS_EQ TOKEN_GREATER_EQ
%left TOKEN_CONCAT
%left TOKEN_ADD TOKEN_ADD_ARRAY TOKEN_DEL_ARRAY
%left TOKEN_MULTIPLY TOKEN_DIVIDE TOKEN_MOD
%right TOKEN_NOT TOKEN_SUBSTRACT

%% /* ---------- grammar ---------- */

program : expr                           { parser_result = $1; }
        ;

expr : TOKEN_LET TOKEN_IDENTIFIER TOKEN_ASIG expr TOKEN_IN expr { printf("PARSER: Creating LetExpression for '%s'\n", last_identifier); $$ = new LetExpression(last_identifier, $4, $6); }
     | or_expr
     ;

or_expr : or_expr TOKEN_OR xor_expr      { $$ = new OrExpression($1, $3); }
        | xor_expr                       { $$ = $1; }
        ;

xor_expr : xor_expr TOKEN_XOR and_expr   { $$ = new XorExpression($1, $3); }
         | and_expr                      { $$ = $1; }
         ;

and_expr : and_expr TOKEN_AND equality_expr  { $$ = new AndExpression($1, $3); }
         | equality_expr                     { $$ = $1; }
         ;

equality_expr : equality_expr TOKEN_EQUAL relational_expr    { $$ = new EqualExpression($1, $3); }
              | equality_expr TOKEN_NOTEQUAL relational_expr { $$ = new NotEqualExpression($1, $3); }
              | relational_expr                             { $$ = $1; }
              ;

relational_expr : relational_expr TOKEN_LESS concat_expr     { $$ = new LessExpression($1, $3); }
                | relational_expr TOKEN_GREAT concat_expr    { $$ = new GreaterExpression($1, $3); }
                | relational_expr TOKEN_LESSEQL concat_expr  { $$ = new LessEqExpression($1, $3); }
                | relational_expr TOKEN_GREATEQL concat_expr { $$ = new GreaterEqExpression($1, $3); }
                | concat_expr                                { $$ = $1; }
                ;

concat_expr : concat_expr TOKEN_CONCAT additive_expr { $$ = new ConcatExpression($1, $3); }
            | concat_expr TOKEN_ADD_ARRAY additive_expr { $$ = new AddArrayExpression($1, $3); }
            | concat_expr TOKEN_DEL_ARRAY additive_expr { $$ = new DelArrayExpression($1, $3); }
            | additive_expr                          { $$ = $1; }
            ;

additive_expr : additive_expr TOKEN_ADD multiplicative_expr       { $$ = new AddExpression($1, $3); }
              | additive_expr TOKEN_SUBSTRACT multiplicative_expr { $$ = new SubExpression($1, $3); }
              | multiplicative_expr                               { $$ = $1; }
              ;

multiplicative_expr : multiplicative_expr TOKEN_MULTIPLY unary_expr { $$ = new MulExpression($1, $3); }
                    | multiplicative_expr TOKEN_DIVIDE unary_expr   { $$ = new DivExpression($1, $3); }
                    | multiplicative_expr TOKEN_MOD unary_expr      { $$ = new ModExpression($1, $3); }
                    | unary_expr                                   { $$ = $1; }
                    ;

unary_expr : TOKEN_NOT unary_expr                       { $$ = new NotExpression($2); }
           | TOKEN_SUBSTRACT unary_expr                 { $$ = new SubExpression(new IntExpression(0), $2); }
           | primary_expr                               { $$ = $1; }
           ;

primary_expr : TOKEN_LPAREN expr TOKEN_RPAREN           { $$ = $2; }
             | literal                                 { $$ = $1; }
             | function_call                           { $$ = $1; }
             | TOKEN_IDENTIFIER                        { $$ = new NameExpression(last_identifier); }
             | TOKEN_FUN TOKEN_IDENTIFIER TOKEN_LPAREN TOKEN_IDENTIFIER TOKEN_RPAREN  expr TOKEN_END { $$ = new FunctionExpression(last_identifier, prev_identifier, $6); }
             ;



literal : TOKEN_INT                                   { $$ = new IntExpression(atoi(yytext)); }
        | TOKEN_REAL                                  { $$ = new RealExpression(atof(yytext)); }
        | TOKEN_STRING                               { $$ = new StrExpression(yytext); }
        | TOKEN_TRUE                                 { $$ = new BooleanExpression(true); }
        | TOKEN_FALSE                                { $$ = new BooleanExpression(false); }
        | array_literal                              { $$ = $1; }
        ;

array_literal : TOKEN_LCORCH elements TOKEN_RCORCH { $$ = new ArrayExpression({$2}); }
              | TOKEN_LCORCH TOKEN_RCORCH         { $$ = new ArrayExpression({}); }
              ;

elements : elements TOKEN_COMA expr               { /* TODO: implementar lista de elementos */ $$ = $1; }
         | expr                                   { /* TODO: implementar lista de elementos */ $$ = $1; }
         ;

function_call : TOKEN_PRINT TOKEN_LPAREN expr TOKEN_RPAREN  { $$ = new PrintExpression($3); }
              | TOKEN_FST TOKEN_LPAREN expr TOKEN_RPAREN    { $$ = new FstExpression($3); }
              | TOKEN_SND TOKEN_LPAREN expr TOKEN_RPAREN    { $$ = new SndExpression($3); }
              | TOKEN_ETOS TOKEN_LPAREN expr TOKEN_RPAREN   { $$ = new ItosExpression($3); }
              | TOKEN_RTOS TOKEN_LPAREN expr TOKEN_RPAREN   { $$ = new RtosExpression($3); }
              | TOKEN_ETOR TOKEN_LPAREN expr TOKEN_RPAREN   { $$ = new ItorExpression($3); }
              | TOKEN_RTOE TOKEN_LPAREN expr TOKEN_RPAREN   { $$ = new RtoiExpression($3); }
              | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN  expr  TOKEN_ELSE  expr  TOKEN_END { /* TODO: implementar if */ $$ = $5; }
              | TOKEN_EMPTY { /* TODO: implementar empty */ $$ = new IntExpression(0); }
              | TOKEN_IDENTIFIER TOKEN_LPAREN expr TOKEN_RPAREN { /* TODO: implementar llamada a función */ $$ = $3; }
              | TOKEN_HEAD TOKEN_LPAREN expr TOKEN_RPAREN { $$ = new HeadExpression($3); }
              | TOKEN_TAIL TOKEN_LPAREN expr TOKEN_RPAREN { $$ = new TailExpression($3); }
              | TOKEN_ADD_ARRAY TOKEN_LPAREN array_literal TOKEN_COMA expr  TOKEN_RPAREN { $$ = new AddArrayExpression($3, $5); }
              | TOKEN_DEL_ARRAY TOKEN_LPAREN expr TOKEN_COMA expr  TOKEN_RPAREN { $$ = new DelArrayExpression($3, $5); }
              ;
                          


%% /* ---------- user code ---------- */

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
        return 1;
    } 
