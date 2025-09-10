#include <iostream>
#include <memory>
#include "expression.hpp"
#include "datatype.hpp"
#include "symbol_table.hpp"

int main() {
    std::cout << "=== DEMO DEL INTERPRETADOR ===" << std::endl;
    
    // Crear expresiones básicas
    auto int_expr = std::make_unique<IntExpression>(42);
    auto str_expr = std::make_unique<StrExpression>("Hello");
    auto bool_expr = std::make_unique<BooleanExpression>(true);
    auto real_expr = std::make_unique<RealExpression>(3.14);
    
    std::cout << "\n--- Expresiones Básicas ---" << std::endl;
    std::cout << "IntExpression(42): " << int_expr->evaluate() << std::endl;
    std::cout << "StrExpression(\"Hello\"): " << str_expr->evaluate() << std::endl;
    std::cout << "BooleanExpression(true): " << bool_expr->evaluate() << std::endl;
    std::cout << "RealExpression(3.14): " << real_expr->evaluate() << std::endl;
    
    // Crear operaciones aritméticas
    auto add_expr = std::make_unique<AddExpression>(
        new IntExpression(5),
        new IntExpression(3)
    );
    
    auto mul_expr = std::make_unique<MulExpression>(
        new IntExpression(4),
        new IntExpression(6)
    );
    
    std::cout << "\n--- Operaciones Aritméticas ---" << std::endl;
    std::cout << "5 + 3: " << add_expr->evaluate() << std::endl;
    std::cout << "4 * 6: " << mul_expr->evaluate() << std::endl;
    
    // Crear operaciones lógicas
    auto and_expr = std::make_unique<AndExpression>(
        new BooleanExpression(true),
        new BooleanExpression(false)
    );
    
    auto or_expr = std::make_unique<OrExpression>(
        new BooleanExpression(true),
        new BooleanExpression(false)
    );
    
    std::cout << "\n--- Operaciones Lógicas ---" << std::endl;
    std::cout << "true && false: " << and_expr->evaluate() << std::endl;
    std::cout << "true || false: " << or_expr->evaluate() << std::endl;
    
    // Crear concatenación de strings
    auto concat_expr = std::make_unique<ConcatExpression>(
        new StrExpression("Hello "),
        new StrExpression("World")
    );
    
    std::cout << "\n--- Concatenación de Strings ---" << std::endl;
    std::cout << "\"Hello \" # \"World\": " << concat_expr->evaluate() << std::endl;
    
    // Crear operaciones de comparación
    auto less_expr = std::make_unique<LessExpression>(
        new IntExpression(5),
        new IntExpression(10)
    );
    
    auto equal_expr = std::make_unique<EqualExpression>(
        new IntExpression(5),
        new IntExpression(5)
    );
    
    std::cout << "\n--- Operaciones de Comparación ---" << std::endl;
    std::cout << "5 < 10: " << less_expr->evaluate() << std::endl;
    std::cout << "5 == 5: " << equal_expr->evaluate() << std::endl;
    
    // Crear un par
    auto pair_expr = std::make_unique<PairExpression>(
        new IntExpression(1),
        new IntExpression(2)
    );
    
    std::cout << "\n--- Pares ---" << std::endl;
    std::cout << "(1, 2): " << pair_expr->evaluate() << std::endl;
    
    // Crear un array
    std::vector<Expression*> elements = {
        new IntExpression(1),
        new IntExpression(2),
        new IntExpression(3)
    };
    auto array_expr = std::make_unique<ArrayExpression>(elements);
    
    std::cout << "\n--- Arrays ---" << std::endl;
    std::cout << "[1, 2, 3]: " << array_expr->evaluate() << std::endl;
    
    // Crear una expresión condicional
    auto if_expr = std::make_unique<IfExpression>(
        new BooleanExpression(true),
        new IntExpression(100),
        new IntExpression(200)
    );
    
    std::cout << "\n--- Expresiones Condicionales ---" << std::endl;
    std::cout << "if true then 100 else 200: " << if_expr->evaluate() << std::endl;
    
    std::cout << "\n=== DEMO COMPLETADO ===" << std::endl;
    
    return 0;
}
