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




%% /* ---------- grammar ---------- */

program : expr                           { parser_result = 0; }
        ;

expr : expr TOKEN_OR and_expr            
     | expr TOKEN_XOR and_expr          
     | and_expr                         
     | expr expr
     ;

and_expr : and_expr TOKEN_AND equality_expr  
         | equality_expr                     
         ;

equality_expr : equality_expr TOKEN_EQUAL relational_expr    
              | equality_expr TOKEN_NOTEQUAL relational_expr 
              | relational_expr                              
              ;

relational_expr : relational_expr TOKEN_LESS concat_expr     
                | relational_expr TOKEN_GREAT concat_expr    
                | relational_expr TOKEN_LESSEQL concat_expr  
                | relational_expr TOKEN_GREATEQL concat_expr 
                | concat_expr                                
                ;

concat_expr : concat_expr TOKEN_CONCAT additive_expr 
            | additive_expr                          
            ;

additive_expr : additive_expr TOKEN_ADD multiplicative_expr       
              | additive_expr TOKEN_SUBSTRACT multiplicative_expr 
              | multiplicative_expr                               
              ;

multiplicative_expr : multiplicative_expr TOKEN_MULTIPLY unary_expr 
                    | multiplicative_expr TOKEN_DIVIDE unary_expr   
                    | multiplicative_expr TOKEN_MOD unary_expr      
                    | unary_expr                                    
                    ;

unary_expr : TOKEN_NOT unary_expr                       
           | TOKEN_SUBSTRACT unary_expr                
           | primary_expr                               
           ;


primary_expr : TOKEN_LPAREN expr TOKEN_RPAREN           
             | literal                                 
             | function_call                           
             | TOKEN_IDENTIFIER                         
             | TOKEN_FUN TOKEN_IDENTIFIER TOKEN_LPAREN TOKEN_IDENTIFIER TOKEN_RPAREN  expr TOKEN_END
             ;



literal : TOKEN_INT                                   
        | TOKEN_REAL                                   
        | TOKEN_STRING                                
        | TOKEN_TRUE                      
        | TOKEN_FALSE                    
        | array_literal                   
        ;

array_literal : TOKEN_LCORCH elements TOKEN_RCORCH 
              | TOKEN_LCORCH TOKEN_RCORCH         
              ;

elements : elements TOKEN_COMA expr               
         | expr 
          
         ;

function_call : TOKEN_PRINT TOKEN_LPAREN expr TOKEN_RPAREN  
              | TOKEN_FST TOKEN_LPAREN expr TOKEN_RPAREN    
              | TOKEN_SND TOKEN_LPAREN expr TOKEN_RPAREN    
              | TOKEN_ETOS TOKEN_LPAREN expr TOKEN_RPAREN   
              | TOKEN_RTOS TOKEN_LPAREN expr TOKEN_RPAREN   
              | TOKEN_ETOR TOKEN_LPAREN expr TOKEN_RPAREN   
              | TOKEN_RTOE TOKEN_LPAREN expr TOKEN_RPAREN   
              | TOKEN_LET  TOKEN_IDENTIFIER  TOKEN_ASIG expr  TOKEN_IN expr 
              | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN  expr  TOKEN_ELSE  expr  TOKEN_END
              | TOKEN_EMPTY 
              | TOKEN_IDENTIFIER TOKEN_LPAREN expr TOKEN_RPAREN
              | TOKEN_HEAD TOKEN_LPAREN expr TOKEN_RPAREN
              | TOKEN_TAIL TOKEN_LPAREN expr TOKEN_RPAREN
              | TOKEN_ADD_ARRAY TOKEN_LPAREN array_literal TOKEN_COMA expr  TOKEN_RPAREN 
              | TOKEN_DEL_ARRAY TOKEN_LPAREN expr TOKEN_COMA expr  TOKEN_RPAREN 
              | TOKEN_DEL_ARRAY TOKEN_LPAREN expr TOKEN_COMA expr  TOKEN_RPAREN 
            
              ;
                          


%% /* ---------- user code ---------- */

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
        return 1;
    } 
