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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <variant>
#include <expression.hpp>

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

    if (result == 0 && parser_result == 0)
        printf("Parse ok!\n");
    else
    {
        printf("Parse failed!\n");
        return 0;
    }



    if (argc == 2) {
        fclose(yyin);
    }
    cleanup_lexer();
    return 0;
}