%{
    #include <stdio.h>
    #include "expression.hpp"
    #include "statement.hpp"
    #include "declaration.hpp"
    #include "ast_node_interface.hpp"
    #include <stdlib.h>
    #include <string.h>

    #define YYSTYPE ASTNodeInterface*

    extern int yylex();
    extern char* yytext;
    extern char* last_identifier;
    int yyerror(const char*);

    ASTNodeInterface* parser_result{nullptr};

    char* copy_string(const char* src) {
        if (!src) return nullptr;
        char* dst = (char*)malloc(strlen(src) + 1);
        strcpy(dst, src);
        return dst;
    }
    
    Body ast_to_body(ASTNodeInterface* node) {
        Body body;
        if (auto stmt = dynamic_cast<Statement*>(node)) {
            body.push_back(stmt);
        }
        return body;
    }

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

program : statements { parser_result = $0; }
        ;

statements : statements statement 
    | statement 
    ;

statement : declaration_statement 
    | expr TOKEN_END
    ;

declaration_statement :  function_declaration
    ;
    

function_declaration : 
    | TOKEN_FUN TOKEN_IDENTIFIER TOKEN_LPAREN TOKEN_IDENTIFIER TOKEN_RPAREN  expr TOKEN_END
    ;


expr : expr TOKEN_OR and_expr            
    {
        $$ = new OrExpression(
        dynamic_cast<Expression*>($1), 
        dynamic_cast<Expression*>($3)
    ); }
     | expr TOKEN_XOR and_expr     
    {
        $$ = new XorExpression(
        dynamic_cast<Expression*>($1), 
        dynamic_cast<Expression*>($3)
    ); }    
    | and_expr                 
    | variable_declaration                         
        
     ;
variable_declaration : TOKEN_LET TOKEN_IDENTIFIER TOKEN_ASIG expr TOKEN_IN expr     

and_expr : and_expr TOKEN_AND equality_expr  {
        $$ = new AndExpression(
        dynamic_cast<Expression*>($1), 
        dynamic_cast<Expression*>($3)
    ); }
    | equality_expr                     
    ;

equality_expr : equality_expr TOKEN_EQUAL relational_expr   
        {
            $$ = new EqualExpression(
            dynamic_cast<Expression*>($1), 
            dynamic_cast<Expression*>($3)
        ); }
    | equality_expr TOKEN_NOTEQUAL relational_expr 
        {
            $$ = new NotEqualExpression(
            dynamic_cast<Expression*>($1), 
            dynamic_cast<Expression*>($3)
        ); }
        | relational_expr                              
              ;

relational_expr : relational_expr TOKEN_LESS concat_expr     
            {
                $$ = new LessExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | relational_expr TOKEN_GREAT concat_expr    
            {
                $$ = new GreaterExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | relational_expr TOKEN_LESSEQL concat_expr  
            {
                $$ = new LessEqExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | relational_expr TOKEN_GREATEQL concat_expr 
            {
                $$ = new GreaterEqExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | concat_expr                                
        ;


concat_expr : concat_expr TOKEN_CONCAT additive_expr 
            {
                $$ = new ConcatExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); 
            }
        | additive_expr                          
        ;


additive_expr : additive_expr TOKEN_ADD multiplicative_expr       
            {
                $$ = new AddExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | additive_expr TOKEN_SUBSTRACT multiplicative_expr 
            {
                $$ = new SubExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | multiplicative_expr                               
        ;

multiplicative_expr : multiplicative_expr TOKEN_MULTIPLY unary_expr 
            {
                $$ = new MulExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | multiplicative_expr TOKEN_DIVIDE unary_expr    
            {
                $$ = new DivExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); }
        | multiplicative_expr TOKEN_MOD unary_expr    
            {
                $$ = new ModExpression(
                dynamic_cast<Expression*>($1), 
                dynamic_cast<Expression*>($3)
            ); } 
        | unary_expr                                    
        ;

unary_expr : TOKEN_NOT unary_expr     
                { $$ = new NotExpression(dynamic_cast<Expression*>($2)); }
           | TOKEN_SUBSTRACT unary_expr 
                { $$ = new NegExpression(dynamic_cast<Expression*>($2)); }   
           | primary_expr                               
           ;


primary_expr : TOKEN_LPAREN expr TOKEN_RPAREN   { $$ = $2; }     
             | literal                                 
             | function_call                           
             | TOKEN_IDENTIFIER    { $$ = new NameExpression(copy_string(last_identifier)); }                      
             ;

literal : TOKEN_INT    
            { $$ = new IntExpression(atoi(yytext)); }                                
        | TOKEN_REAL   
            { $$ = new RealExpression(atof(yytext)); }                         
        | TOKEN_STRING 
            { 
                std::string str(yytext);
                str = str.substr(1, str.length() - 2);
                $$ = new StrExpression(str);
            }
        | TOKEN_TRUE   
            { $$ = new BoolExpression(true); }               
        | TOKEN_FALSE  
            { $$ = new BoolExpression(false); }              
        | array_literal      
        | pair                             
        ;

array_literal : TOKEN_LCORCH elements TOKEN_RCORCH 
              | TOKEN_LCORCH TOKEN_RCORCH     
              | TOKEN_EMPTY    
              ;

pair :TOKEN_LPAREN expr TOKEN_COMA expr TOKEN_RPAREN

elements : elements TOKEN_COMA expr               
         | expr 
         ;

function_call : TOKEN_PRINT TOKEN_LPAREN expr TOKEN_RPAREN  
                { $$ = new PrintExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_FST TOKEN_LPAREN expr TOKEN_RPAREN     
                { $$ = new FstExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_SND TOKEN_LPAREN expr TOKEN_RPAREN  
                { $$ = new SndExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_RTOS TOKEN_LPAREN expr TOKEN_RPAREN   
                { $$ = new RtoSExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_ETOS TOKEN_LPAREN expr TOKEN_RPAREN   
                { $$ = new ItoSExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_ETOR TOKEN_LPAREN expr TOKEN_RPAREN   
                { $$ = new ItoRExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_RTOE TOKEN_LPAREN expr TOKEN_RPAREN   
                { $$ = new RtoIExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_HEAD TOKEN_LPAREN expr TOKEN_RPAREN
                { $$ = new HeadExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_TAIL TOKEN_LPAREN expr TOKEN_RPAREN
                { $$ = new TailExpression(dynamic_cast<Expression*>($3)); } 
            | TOKEN_IDENTIFIER TOKEN_LPAREN expr TOKEN_RPAREN
                { 
                    Expression* func_name = new NameExpression(copy_string(last_identifier));
                    $$ = new CallExpression(func_name, dynamic_cast<Expression*>($3));
                }
            | TOKEN_ADD_ARRAY TOKEN_LPAREN array_literal TOKEN_COMA expr TOKEN_RPAREN 
                {
                    $$ = new ArrayAddExpression(
                    dynamic_cast<Expression*>($3), 
                    dynamic_cast<Expression*>($5)
                ); } 
            | TOKEN_DEL_ARRAY TOKEN_LPAREN expr TOKEN_COMA expr TOKEN_RPAREN 
                 {
                    $$ = new ArrayDelExpression(
                    dynamic_cast<Expression*>($3), 
                    dynamic_cast<Expression*>($5)
                ); } 
            | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN  expr  TOKEN_ELSE  expr  TOKEN_END
              ;


%% /* ---------- user code ---------- */

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
        return 1;
    }