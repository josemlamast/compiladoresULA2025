#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <variant>
#include <memory>
#include "expression.hpp"
#include "utils.hpp"

extern FILE* yyin;
extern int yyparse();
extern Expression* parser_result;
extern void cleanup_lexer();
extern Environment global_env;

int main(int argc, char* argv[])
{
 
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (!yyin)
        {
            printf("Could not open %s\n", argv[1]);
            exit(1);
        }
    }
    
    int result = yyparse();

    if (result == 0)
        printf("Parse ok!\n");
    else
    {
        printf("Parse failed!\n");
        return 0;
    }

    if (parser_result) {
        printf("Parsed expression: %s\n", parser_result->to_string().c_str());
        
        printf("Type checking...\n");
        auto [type_ok, type_result] = parser_result->type_check(global_env);
        if (type_ok) {
            printf("Type check passed. Type: %s\n", datatype_to_string(type_result).c_str());
        } else {
            printf("Type check failed...\n");
            return 0;
        }
        
        try {
            printf("Evaluating expression...\n");
            // Usar el entorno global que contiene las funciones definidas
            auto result = parser_result->eval(global_env);
            printf("Result: %s\n", result->to_string().c_str());
        } catch (const std::exception& e) {
            printf("Evaluation error: %s\n", e.what());
        }
        
       
    } else {
        printf("No expression parsed\n");
    }
    if (argc == 2) {
        fclose(yyin);
    }
    cleanup_lexer();
    return 0;
}