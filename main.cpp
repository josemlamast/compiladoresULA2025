#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <variant>
#include <memory>
#include "expression.hpp"
#include "symbol_table.hpp"
#include "datatype.hpp"
#include "statement.hpp"
#include "ast_node_interface.hpp"

extern FILE* yyin;
extern int yyparse();
extern Expression* parser_result;
extern void cleanup_lexer();



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

    if (result == 0 && parser_result != nullptr)
    {
        printf("Parse ok!\n");
        
        // Crear tabla de símbolos
        SymbolTable symbol_table;
        
        // Resolver nombres
        printf("Resolving names...\n");
        bool name_resolution = parser_result->resolve_name(symbol_table);
        if (name_resolution)
        {
            printf("Name resolution: OK\n");
        }
        else
        {
            printf("Name resolution: FAILED\n");
        }
        
        // Verificar tipos
        printf("Type checking...\n");
        auto type_result = parser_result->type_check();
        if (type_result.first)
        {
            printf("Type checking: OK\n");
            if (type_result.second != nullptr)
            {
                printf("Expression type: ");
                if (type_result.second->is<IntegerDatatype>())
                    printf("Integer\n");
                else if (type_result.second->is<RealDatatype>())
                    printf("Real\n");
                else if (type_result.second->is<StringDatatype>())
                    printf("String\n");
                else if (type_result.second->is<BooleanDatatype>())
                    printf("Boolean\n");
                else if (type_result.second->is<ArrayDatatype>())
                    printf("Array\n");
                else if (type_result.second->is<FunctionDatatype>())
                    printf("Function\n");
                else if (type_result.second->is<VoidDatatype>())
                    printf("Void\n");
                else
                    printf("Unknown\n");
                
                type_result.second->destroy();
                delete type_result.second;
            }
        }
        else
        {
            printf("Type checking: FAILED\n");
        }
        
        // Limpiar memoria
        parser_result->destroy();
        delete parser_result;
    }
    else
    {
        printf("Parse failed!\n");
        return 1;
    }

    if (argc == 2) {
        fclose(yyin);
    }
    cleanup_lexer();
    return 0;
}