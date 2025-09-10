#include <iostream>
#include <memory>
#include <string>

// Clases simplificadas para el demo
class SimpleExpression {
public:
    virtual ~SimpleExpression() = default;
    virtual std::string evaluate() const = 0;
};

class IntExpression : public SimpleExpression {
private:
    int value;
public:
    IntExpression(int v) : value(v) {}
    std::string evaluate() const override {
        return std::to_string(value);
    }
};

class StrExpression : public SimpleExpression {
private:
    std::string value;
public:
    StrExpression(const std::string& v) : value(v) {}
    std::string evaluate() const override {
        return value;
    }
};

class BooleanExpression : public SimpleExpression {
private:
    bool value;
public:
    BooleanExpression(bool v) : value(v) {}
    std::string evaluate() const override {
        return value ? "true" : "false";
    }
};

class AddExpression : public SimpleExpression {
private:
    std::unique_ptr<SimpleExpression> left;
    std::unique_ptr<SimpleExpression> right;
public:
    AddExpression(SimpleExpression* l, SimpleExpression* r) 
        : left(l), right(r) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return std::to_string(left_val + right_val);
    }
};

class MulExpression : public SimpleExpression {
private:
    std::unique_ptr<SimpleExpression> left;
    std::unique_ptr<SimpleExpression> right;
public:
    MulExpression(SimpleExpression* l, SimpleExpression* r) 
        : left(l), right(r) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return std::to_string(left_val * right_val);
    }
};

class AndExpression : public SimpleExpression {
private:
    std::unique_ptr<SimpleExpression> left;
    std::unique_ptr<SimpleExpression> right;
public:
    AndExpression(SimpleExpression* l, SimpleExpression* r) 
        : left(l), right(r) {}
    std::string evaluate() const override {
        bool left_val = left->evaluate() == "true";
        bool right_val = right->evaluate() == "true";
        return (left_val && right_val) ? "true" : "false";
    }
};

class ConcatExpression : public SimpleExpression {
private:
    std::unique_ptr<SimpleExpression> left;
    std::unique_ptr<SimpleExpression> right;
public:
    ConcatExpression(SimpleExpression* l, SimpleExpression* r) 
        : left(l), right(r) {}
    std::string evaluate() const override {
        return left->evaluate() + right->evaluate();
    }
};

class LessExpression : public SimpleExpression {
private:
    std::unique_ptr<SimpleExpression> left;
    std::unique_ptr<SimpleExpression> right;
public:
    LessExpression(SimpleExpression* l, SimpleExpression* r) 
        : left(l), right(r) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return (left_val < right_val) ? "true" : "false";
    }
};

class IfExpression : public SimpleExpression {
private:
    std::unique_ptr<SimpleExpression> condition;
    std::unique_ptr<SimpleExpression> then_expr;
    std::unique_ptr<SimpleExpression> else_expr;
public:
    IfExpression(SimpleExpression* c, SimpleExpression* t, SimpleExpression* e) 
        : condition(c), then_expr(t), else_expr(e) {}
    std::string evaluate() const override {
        if (condition->evaluate() == "true") {
            return then_expr->evaluate();
        } else {
            return else_expr->evaluate();
        }
    }
};

int main() {
    std::cout << "=== DEMO DEL INTERPRETADOR SIMPLIFICADO ===" << std::endl;
    
    // Expresiones básicas
    auto int_expr = std::make_unique<IntExpression>(42);
    auto str_expr = std::make_unique<StrExpression>("Hello");
    auto bool_expr = std::make_unique<BooleanExpression>(true);
    
    std::cout << "\n--- Expresiones Básicas ---" << std::endl;
    std::cout << "IntExpression(42): " << int_expr->evaluate() << std::endl;
    std::cout << "StrExpression(\"Hello\"): " << str_expr->evaluate() << std::endl;
    std::cout << "BooleanExpression(true): " << bool_expr->evaluate() << std::endl;
    
    // Operaciones aritméticas
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
    
    // Operaciones lógicas
    auto and_expr = std::make_unique<AndExpression>(
        new BooleanExpression(true),
        new BooleanExpression(false)
    );
    
    std::cout << "\n--- Operaciones Lógicas ---" << std::endl;
    std::cout << "true && false: " << and_expr->evaluate() << std::endl;
    
    // Concatenación de strings
    auto concat_expr = std::make_unique<ConcatExpression>(
        new StrExpression("Hello "),
        new StrExpression("World")
    );
    
    std::cout << "\n--- Concatenación de Strings ---" << std::endl;
    std::cout << "\"Hello \" # \"World\": " << concat_expr->evaluate() << std::endl;
    
    // Operaciones de comparación
    auto less_expr = std::make_unique<LessExpression>(
        new IntExpression(5),
        new IntExpression(10)
    );
    
    std::cout << "\n--- Operaciones de Comparación ---" << std::endl;
    std::cout << "5 < 10: " << less_expr->evaluate() << std::endl;
    
    // Expresión condicional
    auto if_expr = std::make_unique<IfExpression>(
        new BooleanExpression(true),
        new IntExpression(100),
        new IntExpression(200)
    );
    
    std::cout << "\n--- Expresiones Condicionales ---" << std::endl;
    std::cout << "if true then 100 else 200: " << if_expr->evaluate() << std::endl;
    
    // Expresión más compleja
    auto complex_expr = std::make_unique<AddExpression>(
        new MulExpression(
            new IntExpression(3),
            new IntExpression(4)
        ),
        new IntExpression(2)
    );
    
    std::cout << "\n--- Expresión Compleja ---" << std::endl;
    std::cout << "3 * 4 + 2: " << complex_expr->evaluate() << std::endl;
    
    std::cout << "\n=== DEMO COMPLETADO ===" << std::endl;
    
    return 0;
}
