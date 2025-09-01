#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <variant>
#include <expression.hpp>

extern FILE* yyin;
extern int yyparse();
extern Expression* parser_result;

void usage(char* argv[])
{
    printf("Usage: %s input_file\n", argv[0]);
    exit(1);
}

std::string value_to_string(const Value_t& val) {
    return std::visit([](const auto& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, bool>) {
            return v ? "true" : "false";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return v;
        } else {
            return std::to_string(v);
        }
    }, val);
}
int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        usage(argv);
    }

    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (!yyin)
        {
            printf("Could not open %s\n", argv[1]);
            exit(1);
        }
    } else {
        yyin = stdin;
        printf("Ingrese una expresion (presione Ctrl+D para terminar):\n");
    }
    
    int result = yyparse();

    if (result == 0 && parser_result)
    {
        Environment* env = new Environment();
        
        try {
            auto eval_result = parser_result->eval(env);
            printf("Expresion: %s\n", parser_result->to_string().c_str());
            printf("Resultado: %s\n", value_to_string(eval_result).c_str());
            printf("Tipo: %s\n", parser_result->get_type().c_str());
        } catch (const std::exception& e) {
            printf("Error en evaluación: %s\n", e.what());
        }

        parser_result->destroy();
        delete parser_result;
        delete env;
    }
    else
    {
        printf("Parse failed!\n");
    }

    if (argc == 2) {
        fclose(yyin);
    }

    return 0;
}