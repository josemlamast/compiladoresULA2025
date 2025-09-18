%{
    #include <stdio.h> 
    #include "expression.hpp"
    #include "utils.hpp"
    #include <stdlib.h>
    #include <string.h>
    #include <memory>
        #include <iostream>


    #define YYSTYPE Expression*

    extern int yylex();
    extern char* yytext;
    extern char* last_identifier;
    char* let_var_stack[100];
    int let_var_stack_top = 0;
    extern char* function_name;

    char* saved_function_name = nullptr;
    char* saved_param_name = nullptr;
    char* saved_let_var_name = nullptr;



    int yyerror(const char*);

    Expression* parser_result{nullptr};

    char* copy_string(const char* src) {
        if (!src) return nullptr;
        char* dst = (char*)malloc(strlen(src) + 1);
        strcpy(dst, src);
        return dst;
    }
    
// Función auxiliar para manejar el resultado del parser
void set_parser_result(Expression* expr) {
    parser_result = expr;
}



// Entorno global para almacenar funciones
Environment global_env;

// Función para crear una secuencia de declaraciones
Expression* create_statement_sequence(Expression* prev, Expression* current) {
    
    // Si la expresión anterior es una declaración de función, la almacenamos en el entorno global
    if (prev != nullptr) {
        auto fun_expr = dynamic_cast<FunExpression*>(prev);
        if (fun_expr != nullptr) {
            // Store the function in the global environment
            std::string func_name = fun_expr->get_name();
            
            // First do type check
            auto [type_ok, type_result] = fun_expr->type_check(global_env);
            if (!type_ok) {
                return current; // Continue with current expression
            }
            
            // Create closure directly without evaluating the function
            // Get parameter name for type inference
            auto param_name_expr = std::dynamic_pointer_cast<NameExpression>(fun_expr->get_parameter_name_expression());
            std::string param_name = param_name_expr ? param_name_expr->get_name() : "unknown";
            
            // Infer function types (same logic as FunExpression::eval)
            auto [param_type, return_type] = infer_function_types(
                fun_expr->get_body_expression(), 
                param_name, 
                global_env
            );
            
            // Create closure directly
            auto closure = std::make_shared<Closure>(global_env, param_name, fun_expr->get_body_expression(),
                                                    param_type, return_type);
            global_env.add(func_name, closure);
        }
    }
    
    // Si la expresión actual es una declaración de función, la almacenamos en el entorno global
    if (current != nullptr) {
        auto fun_expr = dynamic_cast<FunExpression*>(current);
        if (fun_expr != nullptr) {
            // Store the function in the global environment
            std::string func_name = fun_expr->get_name();
            
            // First do type check
            auto [type_ok, type_result] = fun_expr->type_check(global_env);
            if (!type_ok) {
                return current; // Continue with current expression
            }
            
            // Create closure directly without evaluating the function
            // Get parameter name for type inference
            auto param_name_expr = std::dynamic_pointer_cast<NameExpression>(fun_expr->get_parameter_name_expression());
            std::string param_name = param_name_expr ? param_name_expr->get_name() : "unknown";
            
            // Infer function types (same logic as FunExpression::eval)
            auto [param_type, return_type] = infer_function_types(
                fun_expr->get_body_expression(), 
                param_name, 
                global_env
            );
            
            // Create closure directly
            auto closure = std::make_shared<Closure>(global_env, param_name, fun_expr->get_body_expression(),
                                                    param_type, return_type);
            global_env.add(func_name, closure);
        }
    }
    
    // Retornamos la expresión actual para evaluación
    return current;
}

// Functions to manage let variable stack
void push_let_var(char* var_name) {
    if (let_var_stack_top < 100) {
        let_var_stack[let_var_stack_top++] = copy_string(var_name);
    }
}

char* pop_let_var() {
    if (let_var_stack_top > 0) {
        return let_var_stack[--let_var_stack_top];
    }
    return nullptr;
}

char* peek_let_var() {
    if (let_var_stack_top > 0) {
        return let_var_stack[let_var_stack_top - 1];
    }
    return nullptr;
}

%}

%left TOKEN_OR
%left TOKEN_XOR
%left TOKEN_AND
%left TOKEN_EQUAL TOKEN_NOTEQUAL
%left TOKEN_LESS TOKEN_GREAT TOKEN_LESSEQL TOKEN_GREATEQL
%left TOKEN_CONCAT
%left TOKEN_ADD TOKEN_SUBSTRACT
%left TOKEN_MULTIPLY TOKEN_DIVIDE TOKEN_MOD
%right TOKEN_NOT
%right TOKEN_LPAREN TOKEN_RPAREN

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
%token TOKEN_LENGTH
%token TOKEN_ISUNIT
%token TOKEN_UNIT
    
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

program : statement_list { set_parser_result($1); }
        ;

statement_list : statement_list statement 
        { $$ = create_statement_sequence($1, $2); }
    | statement 
        { $$ = $1; }
    ;

statement : function_declaration 
    | TOKEN_PRINT TOKEN_LPAREN expr TOKEN_RPAREN 
        { $$ = new PrintExpression(std::shared_ptr<Expression>($3)); }             
    | variable_declaration
    | expr { $$ = $1; }
    ;

variable_declaration : TOKEN_LET let_var_save TOKEN_ASIG expr TOKEN_IN expr TOKEN_END
    {
        char* let_var = pop_let_var();
        
        // Use the let variable from the stack
        auto var_name = std::make_shared<NameExpression>(copy_string(let_var));
        auto var_expr = std::shared_ptr<Expression>($4);
        auto body_expr = std::shared_ptr<Expression>($6);
        $$ = new LetExpression(var_name, var_expr, body_expr);
        
        // Free the popped variable name
        if (let_var) {
            free(let_var);
        }
    }

function_declaration : TOKEN_FUN fname_save TOKEN_LPAREN param_save TOKEN_RPAREN  statement TOKEN_END
    {
        
        auto func_name = std::make_shared<NameExpression>(copy_string(saved_function_name));
        auto param_name = std::make_shared<NameExpression>(copy_string(saved_param_name));
        auto body_expr = std::shared_ptr<Expression>(dynamic_cast<Expression*>($6));
        $$ = new FunExpression(func_name, param_name, body_expr);
    }

fname_save : TOKEN_IDENTIFIER
    {
        if (saved_function_name != nullptr) {
            free(saved_function_name);
        }
        saved_function_name = strdup(last_identifier);
        $$ = nullptr; // No necesitamos un valor semántico
    }


param_save : TOKEN_IDENTIFIER
    {
        if (saved_param_name != nullptr) {
            free(saved_param_name);
        }
        saved_param_name = strdup(last_identifier);
        $$ = nullptr; // No necesitamos un valor semántico
    }

let_var_save : TOKEN_IDENTIFIER
    {
        if (saved_let_var_name != nullptr) {
            free(saved_let_var_name);
        }
        saved_let_var_name = strdup(last_identifier);
        push_let_var(last_identifier);
        $$ = nullptr; // No necesitamos un valor semántico
    }


expr : expr TOKEN_OR and_expr            
    {
        $$ = new OrExpression(
        std::shared_ptr<Expression>($1), 
        std::shared_ptr<Expression>($3)
    ); }
     | expr TOKEN_XOR and_expr     
    {
        $$ = new XorExpression(
        std::shared_ptr<Expression>($1), 
        std::shared_ptr<Expression>($3)
    ); }    
    | and_expr                 
    | TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement TOKEN_ELSE statement TOKEN_END
    {
        $$ = new IfElseExpression( 
            std::shared_ptr<Expression>($3), 
            std::shared_ptr<Expression>($5), 
            std::shared_ptr<Expression>($7)
        );
    }
    | variable_declaration { $$ = $1; }                   
     ;



and_expr : and_expr TOKEN_AND equality_expr  {
        $$ = new AndExpression(
        std::shared_ptr<Expression>($1), 
        std::shared_ptr<Expression>($3)
    ); }
    | equality_expr                     
    ;

equality_expr : equality_expr TOKEN_EQUAL relational_expr   
        {
            $$ = new EqualExpression(
            std::shared_ptr<Expression>($1), 
            std::shared_ptr<Expression>($3)
        ); }
    | equality_expr TOKEN_NOTEQUAL relational_expr 
        {
            $$ = new NotEqualExpression(
            std::shared_ptr<Expression>($1), 
            std::shared_ptr<Expression>($3)
        ); }
        | relational_expr                              
              ;

relational_expr : relational_expr TOKEN_LESS concat_expr     
            {
                $$ = new LessExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | relational_expr TOKEN_GREAT concat_expr    
            {
                $$ = new GreaterExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | relational_expr TOKEN_LESSEQL concat_expr  
            {
                $$ = new LessEqExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | relational_expr TOKEN_GREATEQL concat_expr 
            {
                $$ = new GreaterEqExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | concat_expr                                
        ;


concat_expr : concat_expr TOKEN_CONCAT additive_expr 
            {
                $$ = new ConcatExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); 
            }
        | additive_expr                          
        ;


additive_expr : additive_expr TOKEN_ADD multiplicative_expr       
            {
                $$ = new AddExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | additive_expr TOKEN_SUBSTRACT multiplicative_expr 
            {
                $$ = new SubExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | multiplicative_expr                               
        ;

multiplicative_expr : multiplicative_expr TOKEN_MULTIPLY unary_expr 
            {
                $$ = new MulExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | multiplicative_expr TOKEN_DIVIDE unary_expr    
            {
                $$ = new DivExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); }
        | multiplicative_expr TOKEN_MOD unary_expr    
            {
                $$ = new ModExpression(
                std::shared_ptr<Expression>($1), 
                std::shared_ptr<Expression>($3)
            ); } 
        | unary_expr                                    
        ;

unary_expr : TOKEN_NOT unary_expr     
                { $$ = new NotExpression(std::shared_ptr<Expression>($2)); }
           | TOKEN_SUBSTRACT unary_expr 
                { $$ = new NegExpression(std::shared_ptr<Expression>($2)); }   
           | primary_expr                               
           ;


primary_expr : TOKEN_LPAREN expr TOKEN_RPAREN   { $$ = $2; }     
             | literal                                 
             | identifier
             | function_call
             | TOKEN_PRINT TOKEN_LPAREN expr TOKEN_RPAREN 
                { $$ = new PrintExpression(std::shared_ptr<Expression>($3)); }
             ;

identifier : TOKEN_IDENTIFIER
                    { 
                        $$ = new NameExpression(copy_string(last_identifier)); 
                    }

function_call : TOKEN_IDENTIFIER TOKEN_LPAREN expr TOKEN_RPAREN
                    { 
                        auto func_name = std::make_shared<NameExpression>(copy_string(function_name));
                        $$ = new CallExpression(func_name, std::shared_ptr<Expression>($3));
                    }
                  | TOKEN_FST TOKEN_LPAREN expr TOKEN_RPAREN     
                    { $$ = new FstExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_SND TOKEN_LPAREN expr TOKEN_RPAREN  
                    { $$ = new SndExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_RTOS TOKEN_LPAREN expr TOKEN_RPAREN   
                    { $$ = new RtoSExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_ETOS TOKEN_LPAREN expr TOKEN_RPAREN   
                    { $$ = new ItoSExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_ETOR TOKEN_LPAREN expr TOKEN_RPAREN   
                    { $$ = new ItoRExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_RTOE TOKEN_LPAREN expr TOKEN_RPAREN   
                    { $$ = new RtoIExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_ISUNIT TOKEN_LPAREN expr TOKEN_RPAREN   
                    { $$ = new IsUniTExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_UNIT TOKEN_LPAREN expr TOKEN_RPAREN   
                    { $$ = new UnitExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_HEAD TOKEN_LPAREN expr TOKEN_RPAREN
                    { $$ = new HeadExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_TAIL TOKEN_LPAREN expr TOKEN_RPAREN
                    { $$ = new TailExpression(std::shared_ptr<Expression>($3)); }
                  | TOKEN_LENGTH TOKEN_LPAREN expr TOKEN_RPAREN
                    { $$ = new LengthExpression(std::shared_ptr<Expression>($3)); } 
                  | TOKEN_ADD_ARRAY TOKEN_LPAREN array_literal TOKEN_COMA expr TOKEN_RPAREN 
                    {
                        $$ = new ArrayAddExpression(
                        std::shared_ptr<Expression>($3), 
                        std::shared_ptr<Expression>($5)
                    ); } 
                  | TOKEN_DEL_ARRAY TOKEN_LPAREN expr TOKEN_COMA expr TOKEN_RPAREN 
                       {
                        $$ = new ArrayDelExpression(
                        std::shared_ptr<Expression>($3), 
                        std::shared_ptr<Expression>($5)
                    ); } 
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
                { $$ = $2; }
              | TOKEN_LCORCH TOKEN_RCORCH     
                { $$ = new ArrayExpression(std::vector<std::shared_ptr<Expression>>()); }
              | TOKEN_EMPTY    
                { $$ = new ArrayExpression(std::vector<std::shared_ptr<Expression>>()); }
              ;

pair : TOKEN_LPAREN expr TOKEN_COMA expr TOKEN_RPAREN
    {
        $$ = new PairExpression(
            std::shared_ptr<Expression>($2),
            std::shared_ptr<Expression>($4)
        );
    }

elements : elements TOKEN_COMA expr               
            { 
                auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(std::shared_ptr<Expression>($1));
                if (array_expr) {
                    auto new_elements = array_expr->get_elements();
                    new_elements.push_back(std::shared_ptr<Expression>($3));
                    $$ = new ArrayExpression(new_elements);
                } else {
                    // Si no es un ArrayExpression, crear uno nuevo
                    std::vector<std::shared_ptr<Expression>> new_elements;
                    new_elements.push_back(std::shared_ptr<Expression>($1));
                    new_elements.push_back(std::shared_ptr<Expression>($3));
                    $$ = new ArrayExpression(new_elements);
                }
            }
         | expr 
            { 
                std::vector<std::shared_ptr<Expression>> elements;
                elements.push_back(std::shared_ptr<Expression>($1));
                $$ = new ArrayExpression(elements);
            }
         ;



%% /* ---------- user code ---------- */

int yyerror(const char* s) {
        return 1;
    }