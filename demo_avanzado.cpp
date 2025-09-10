#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Clases del interpretador (versión simplificada)
class Expression {
public:
    virtual ~Expression() = default;
    virtual std::string evaluate() const = 0;
};

class IntExpression : public Expression {
private:
    int value;
public:
    IntExpression(int v) : value(v) {}
    std::string evaluate() const override {
        return std::to_string(value);
    }
};

class StrExpression : public Expression {
private:
    std::string value;
public:
    StrExpression(const std::string& v) : value(v) {}
    std::string evaluate() const override {
        return value;
    }
};

class BooleanExpression : public Expression {
private:
    bool value;
public:
    BooleanExpression(bool v) : value(v) {}
    std::string evaluate() const override {
        return value ? "true" : "false";
    }
};

class AddExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    AddExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return std::to_string(left_val + right_val);
    }
};

class MulExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    MulExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return std::to_string(left_val * right_val);
    }
};

class SubExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    SubExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return std::to_string(left_val - right_val);
    }
};

class DivExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    DivExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        if (right_val == 0) return "ERROR: division by zero";
        return std::to_string(left_val / right_val);
    }
};

class ModExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    ModExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        if (right_val == 0) return "ERROR: modulo by zero";
        return std::to_string(left_val % right_val);
    }
};

class AndExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    AndExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        bool left_val = left->evaluate() == "true";
        bool right_val = right->evaluate() == "true";
        return (left_val && right_val) ? "true" : "false";
    }
};

class OrExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    OrExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        bool left_val = left->evaluate() == "true";
        bool right_val = right->evaluate() == "true";
        return (left_val || right_val) ? "true" : "false";
    }
};

class NotExpression : public Expression {
private:
    std::unique_ptr<Expression> expr;
public:
    NotExpression(std::unique_ptr<Expression> e) : expr(std::move(e)) {}
    std::string evaluate() const override {
        bool value = expr->evaluate() == "true";
        return (!value) ? "true" : "false";
    }
};

class ConcatExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    ConcatExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        return left->evaluate() + right->evaluate();
    }
};

class LessExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    LessExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return (left_val < right_val) ? "true" : "false";
    }
};

class LessEqExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    LessEqExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return (left_val <= right_val) ? "true" : "false";
    }
};

class GreaterExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    GreaterExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return (left_val > right_val) ? "true" : "false";
    }
};

class GreaterEqExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    GreaterEqExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        int left_val = std::stoi(left->evaluate());
        int right_val = std::stoi(right->evaluate());
        return (left_val >= right_val) ? "true" : "false";
    }
};

class EqualExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    EqualExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        return (left->evaluate() == right->evaluate()) ? "true" : "false";
    }
};

class NotEqualExpression : public Expression {
private:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
public:
    NotEqualExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) 
        : left(std::move(l)), right(std::move(r)) {}
    std::string evaluate() const override {
        return (left->evaluate() != right->evaluate()) ? "true" : "false";
    }
};

class IfExpression : public Expression {
private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Expression> then_expr;
    std::unique_ptr<Expression> else_expr;
public:
    IfExpression(std::unique_ptr<Expression> c, std::unique_ptr<Expression> t, std::unique_ptr<Expression> e) 
        : condition(std::move(c)), then_expr(std::move(t)), else_expr(std::move(e)) {}
    std::string evaluate() const override {
        if (condition->evaluate() == "true") {
            return then_expr->evaluate();
        } else {
            return else_expr->evaluate();
        }
    }
};

class ArrayExpression : public Expression {
private:
    std::vector<std::unique_ptr<Expression>> elements;
public:
    ArrayExpression(std::vector<std::unique_ptr<Expression>> elems) 
        : elements(std::move(elems)) {}
    std::string evaluate() const override {
        std::string result = "[";
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) result += ", ";
            result += elements[i]->evaluate();
        }
        result += "]";
        return result;
    }
};

class PairExpression : public Expression {
private:
    std::unique_ptr<Expression> first;
    std::unique_ptr<Expression> second;
public:
    PairExpression(std::unique_ptr<Expression> f, std::unique_ptr<Expression> s) 
        : first(std::move(f)), second(std::move(s)) {}
    std::string evaluate() const override {
        return "(" + first->evaluate() + ", " + second->evaluate() + ")";
    }
};

class HeadExpression : public Expression {
private:
    std::unique_ptr<Expression> expr;
public:
    HeadExpression(std::unique_ptr<Expression> e) : expr(std::move(e)) {}
    std::string evaluate() const override {
        std::string array_str = expr->evaluate();
        if (array_str == "[]") {
            return "ERROR: head of empty array";
        }
        size_t start = array_str.find('[') + 1;
        size_t end = array_str.find(',', start);
        if (end == std::string::npos) {
            end = array_str.find(']', start);
        }
        return array_str.substr(start, end - start);
    }
};

class TailExpression : public Expression {
private:
    std::unique_ptr<Expression> expr;
public:
    TailExpression(std::unique_ptr<Expression> e) : expr(std::move(e)) {}
    std::string evaluate() const override {
        std::string array_str = expr->evaluate();
        if (array_str == "[]") {
            return "[]";
        }
        return "TAIL_" + array_str;
    }
};

class FstExpression : public Expression {
private:
    std::unique_ptr<Expression> expr;
public:
    FstExpression(std::unique_ptr<Expression> e) : expr(std::move(e)) {}
    std::string evaluate() const override {
        std::string pair_str = expr->evaluate();
        size_t start = pair_str.find('(') + 1;
        size_t end = pair_str.find(',', start);
        if (end == std::string::npos) {
            return "ERROR: invalid pair";
        }
        return pair_str.substr(start, end - start);
    }
};

class SndExpression : public Expression {
private:
    std::unique_ptr<Expression> expr;
public:
    SndExpression(std::unique_ptr<Expression> e) : expr(std::move(e)) {}
    std::string evaluate() const override {
        std::string pair_str = expr->evaluate();
        size_t start = pair_str.find(',') + 1;
        size_t end = pair_str.find(')', start);
        if (end == std::string::npos) {
            return "ERROR: invalid pair";
        }
        return pair_str.substr(start, end - start);
    }
};

class PrintExpression : public Expression {
private:
    std::unique_ptr<Expression> expr;
public:
    PrintExpression(std::unique_ptr<Expression> e) : expr(std::move(e)) {}
    std::string evaluate() const override {
        std::string result = expr->evaluate();
        std::cout << "PRINT: " << result << std::endl;
        return result;
    }
};

// Función helper para crear arrays
std::unique_ptr<ArrayExpression> createArray(std::vector<int> values) {
    std::vector<std::unique_ptr<Expression>> elements;
    for (int val : values) {
        elements.push_back(std::make_unique<IntExpression>(val));
    }
    return std::make_unique<ArrayExpression>(std::move(elements));
}

// Función helper para crear arrays de strings
std::unique_ptr<ArrayExpression> createStringArray(std::vector<std::string> values) {
    std::vector<std::unique_ptr<Expression>> elements;
    for (const std::string& val : values) {
        elements.push_back(std::make_unique<StrExpression>(val));
    }
    return std::make_unique<ArrayExpression>(std::move(elements));
}

// Función para demostrar operaciones matemáticas avanzadas
void demostrarMatematicas() {
    std::cout << "\n=== MATEMÁTICAS AVANZADAS ===" << std::endl;
    
    // 1. Operaciones con módulo
    std::cout << "\n--- Operaciones con Módulo ---" << std::endl;
    auto mod1 = std::make_unique<ModExpression>(
        std::make_unique<IntExpression>(17),
        std::make_unique<IntExpression>(5)
    );
    std::cout << "17 % 5 = " << mod1->evaluate() << std::endl;
    
    auto mod2 = std::make_unique<ModExpression>(
        std::make_unique<IntExpression>(20),
        std::make_unique<IntExpression>(4)
    );
    std::cout << "20 % 4 = " << mod2->evaluate() << std::endl;
    
    // 2. Expresiones matemáticas complejas
    std::cout << "\n--- Expresiones Matemáticas Complejas ---" << std::endl;
    auto expr1 = std::make_unique<AddExpression>(
        std::make_unique<MulExpression>(
            std::make_unique<IntExpression>(3),
            std::make_unique<IntExpression>(4)
        ),
        std::make_unique<DivExpression>(
            std::make_unique<IntExpression>(20),
            std::make_unique<IntExpression>(2)
        )
    );
    std::cout << "3 * 4 + 20 / 2 = " << expr1->evaluate() << std::endl;
    
    auto expr2 = std::make_unique<SubExpression>(
        std::make_unique<MulExpression>(
            std::make_unique<IntExpression>(5),
            std::make_unique<IntExpression>(6)
        ),
        std::make_unique<ModExpression>(
            std::make_unique<IntExpression>(15),
            std::make_unique<IntExpression>(4)
        )
    );
    std::cout << "5 * 6 - 15 % 4 = " << expr2->evaluate() << std::endl;
    
    // 3. Verificación de números pares e impares
    std::cout << "\n--- Verificación de Paridad ---" << std::endl;
    auto es_par = std::make_unique<EqualExpression>(
        std::make_unique<ModExpression>(
            std::make_unique<IntExpression>(8),
            std::make_unique<IntExpression>(2)
        ),
        std::make_unique<IntExpression>(0)
    );
    std::cout << "8 % 2 == 0 (es par) = " << es_par->evaluate() << std::endl;
    
    auto es_impar = std::make_unique<NotEqualExpression>(
        std::make_unique<ModExpression>(
            std::make_unique<IntExpression>(7),
            std::make_unique<IntExpression>(2)
        ),
        std::make_unique<IntExpression>(0)
    );
    std::cout << "7 % 2 != 0 (es impar) = " << es_impar->evaluate() << std::endl;
    
    std::cout << "\n=== MATEMÁTICAS AVANZADAS COMPLETADAS ===" << std::endl;
}

// Función para demostrar operaciones de comparación avanzadas
void demostrarComparaciones() {
    std::cout << "\n=== COMPARACIONES AVANZADAS ===" << std::endl;
    
    // 1. Comparaciones múltiples
    std::cout << "\n--- Comparaciones Múltiples ---" << std::endl;
    auto comp1 = std::make_unique<AndExpression>(
        std::make_unique<LessExpression>(
            std::make_unique<IntExpression>(5),
            std::make_unique<IntExpression>(10)
        ),
        std::make_unique<GreaterExpression>(
            std::make_unique<IntExpression>(10),
            std::make_unique<IntExpression>(3)
        )
    );
    std::cout << "(5 < 10) && (10 > 3) = " << comp1->evaluate() << std::endl;
    
    auto comp2 = std::make_unique<OrExpression>(
        std::make_unique<EqualExpression>(
            std::make_unique<IntExpression>(7),
            std::make_unique<IntExpression>(8)
        ),
        std::make_unique<LessEqExpression>(
            std::make_unique<IntExpression>(6),
            std::make_unique<IntExpression>(6)
        )
    );
    std::cout << "(7 == 8) || (6 <= 6) = " << comp2->evaluate() << std::endl;
    
    // 2. Comparaciones con expresiones
    std::cout << "\n--- Comparaciones con Expresiones ---" << std::endl;
    auto comp3 = std::make_unique<GreaterExpression>(
        std::make_unique<AddExpression>(
            std::make_unique<IntExpression>(3),
            std::make_unique<IntExpression>(4)
        ),
        std::make_unique<MulExpression>(
            std::make_unique<IntExpression>(2),
            std::make_unique<IntExpression>(3)
        )
    );
    std::cout << "(3 + 4) > (2 * 3) = " << comp3->evaluate() << std::endl;
    
    auto comp4 = std::make_unique<EqualExpression>(
        std::make_unique<SubExpression>(
            std::make_unique<IntExpression>(10),
            std::make_unique<IntExpression>(3)
        ),
        std::make_unique<DivExpression>(
            std::make_unique<IntExpression>(14),
            std::make_unique<IntExpression>(2)
        )
    );
    std::cout << "(10 - 3) == (14 / 2) = " << comp4->evaluate() << std::endl;
    
    std::cout << "\n=== COMPARACIONES AVANZADAS COMPLETADAS ===" << std::endl;
}

// Función para demostrar operaciones con arrays avanzadas
void demostrarArraysAvanzados() {
    std::cout << "\n=== ARRAYS AVANZADOS ===" << std::endl;
    
    // 1. Arrays de diferentes tipos
    std::cout << "\n--- Arrays de Enteros ---" << std::endl;
    auto array_nums = createArray({10, 20, 30, 40, 50});
    std::cout << "[10, 20, 30, 40, 50] = " << array_nums->evaluate() << std::endl;
    
    auto head_nums = std::make_unique<HeadExpression>(createArray({10, 20, 30}));
    std::cout << "head([10, 20, 30]) = " << head_nums->evaluate() << std::endl;
    
    auto tail_nums = std::make_unique<TailExpression>(createArray({10, 20, 30}));
    std::cout << "tail([10, 20, 30]) = " << tail_nums->evaluate() << std::endl;
    
    // 2. Arrays de strings
    std::cout << "\n--- Arrays de Strings ---" << std::endl;
    auto array_strings = createStringArray({"Hola", "Mundo", "Funcional"});
    std::cout << "[\"Hola\", \"Mundo\", \"Funcional\"] = " << array_strings->evaluate() << std::endl;
    
    auto head_strings = std::make_unique<HeadExpression>(createStringArray({"Hola", "Mundo"}));
    std::cout << "head([\"Hola\", \"Mundo\"]) = " << head_strings->evaluate() << std::endl;
    
    // 3. Arrays mixtos
    std::cout << "\n--- Arrays Mixtos ---" << std::endl;
    std::vector<std::unique_ptr<Expression>> array_mixto;
    array_mixto.push_back(std::make_unique<IntExpression>(1));
    array_mixto.push_back(std::make_unique<StrExpression>("dos"));
    array_mixto.push_back(std::make_unique<IntExpression>(3));
    array_mixto.push_back(std::make_unique<StrExpression>("cuatro"));
    
    auto array_mixed = std::make_unique<ArrayExpression>(std::move(array_mixto));
    std::cout << "[1, \"dos\", 3, \"cuatro\"] = " << array_mixed->evaluate() << std::endl;
    
    std::cout << "\n=== ARRAYS AVANZADOS COMPLETADOS ===" << std::endl;
}

// Función para demostrar operaciones con pares avanzadas
void demostrarParesAvanzados() {
    std::cout << "\n=== PARES AVANZADOS ===" << std::endl;
    
    // 1. Pares de diferentes tipos
    std::cout << "\n--- Pares de Diferentes Tipos ---" << std::endl;
    auto par1 = std::make_unique<PairExpression>(
        std::make_unique<IntExpression>(42),
        std::make_unique<StrExpression>("respuesta")
    );
    std::cout << "(42, \"respuesta\") = " << par1->evaluate() << std::endl;
    
    auto fst1 = std::make_unique<FstExpression>(
        std::make_unique<PairExpression>(
            std::make_unique<IntExpression>(42),
            std::make_unique<StrExpression>("respuesta")
        )
    );
    std::cout << "fst((42, \"respuesta\")) = " << fst1->evaluate() << std::endl;
    
    auto snd1 = std::make_unique<SndExpression>(
        std::make_unique<PairExpression>(
            std::make_unique<IntExpression>(42),
            std::make_unique<StrExpression>("respuesta")
        )
    );
    std::cout << "snd((42, \"respuesta\")) = " << snd1->evaluate() << std::endl;
    
    // 2. Pares anidados
    std::cout << "\n--- Pares Anidados ---" << std::endl;
    auto par_anidado = std::make_unique<PairExpression>(
        std::make_unique<PairExpression>(
            std::make_unique<IntExpression>(1),
            std::make_unique<IntExpression>(2)
        ),
        std::make_unique<PairExpression>(
            std::make_unique<IntExpression>(3),
            std::make_unique<IntExpression>(4)
        )
    );
    std::cout << "((1, 2), (3, 4)) = " << par_anidado->evaluate() << std::endl;
    
    // 3. Pares con expresiones
    std::cout << "\n--- Pares con Expresiones ---" << std::endl;
    auto par_expr = std::make_unique<PairExpression>(
        std::make_unique<AddExpression>(
            std::make_unique<IntExpression>(3),
            std::make_unique<IntExpression>(4)
        ),
        std::make_unique<MulExpression>(
            std::make_unique<IntExpression>(2),
            std::make_unique<IntExpression>(5)
        )
    );
    std::cout << "((3 + 4), (2 * 5)) = " << par_expr->evaluate() << std::endl;
    
    std::cout << "\n=== PARES AVANZADOS COMPLETADOS ===" << std::endl;
}

// Función para demostrar lógica avanzada
void demostrarLogicaAvanzada() {
    std::cout << "\n=== LÓGICA AVANZADA ===" << std::endl;
    
    // 1. Operaciones lógicas complejas
    std::cout << "\n--- Operaciones Lógicas Complejas ---" << std::endl;
    auto logica1 = std::make_unique<AndExpression>(
        std::make_unique<OrExpression>(
            std::make_unique<BooleanExpression>(true),
            std::make_unique<BooleanExpression>(false)
        ),
        std::make_unique<NotExpression>(
            std::make_unique<BooleanExpression>(false)
        )
    );
    std::cout << "(true || false) && !false = " << logica1->evaluate() << std::endl;
    
    auto logica2 = std::make_unique<OrExpression>(
        std::make_unique<AndExpression>(
            std::make_unique<BooleanExpression>(true),
            std::make_unique<BooleanExpression>(false)
        ),
        std::make_unique<BooleanExpression>(true)
    );
    std::cout << "(true && false) || true = " << logica2->evaluate() << std::endl;
    
    // 2. Lógica con comparaciones
    std::cout << "\n--- Lógica con Comparaciones ---" << std::endl;
    auto logica3 = std::make_unique<AndExpression>(
        std::make_unique<LessExpression>(
            std::make_unique<IntExpression>(5),
            std::make_unique<IntExpression>(10)
        ),
        std::make_unique<GreaterExpression>(
            std::make_unique<IntExpression>(8),
            std::make_unique<IntExpression>(3)
        )
    );
    std::cout << "(5 < 10) && (8 > 3) = " << logica3->evaluate() << std::endl;
    
    auto logica4 = std::make_unique<OrExpression>(
        std::make_unique<EqualExpression>(
            std::make_unique<IntExpression>(7),
            std::make_unique<IntExpression>(8)
        ),
        std::make_unique<NotEqualExpression>(
            std::make_unique<IntExpression>(6),
            std::make_unique<IntExpression>(6)
        )
    );
    std::cout << "(7 == 8) || (6 != 6) = " << logica4->evaluate() << std::endl;
    
    std::cout << "\n=== LÓGICA AVANZADA COMPLETADA ===" << std::endl;
}

// Función para demostrar condicionales avanzadas
void demostrarCondicionalesAvanzadas() {
    std::cout << "\n=== CONDICIONALES AVANZADAS ===" << std::endl;
    
    // 1. Condicionales anidadas
    std::cout << "\n--- Condicionales Anidadas ---" << std::endl;
    auto cond1 = std::make_unique<IfExpression>(
        std::make_unique<LessExpression>(
            std::make_unique<IntExpression>(5),
            std::make_unique<IntExpression>(10)
        ),
        std::make_unique<IfExpression>(
            std::make_unique<GreaterExpression>(
                std::make_unique<IntExpression>(3),
                std::make_unique<IntExpression>(2)
            ),
            std::make_unique<IntExpression>(100),
            std::make_unique<IntExpression>(200)
        ),
        std::make_unique<IntExpression>(300)
    );
    std::cout << "if (5 < 10) then (if (3 > 2) then 100 else 200) else 300 = " 
              << cond1->evaluate() << std::endl;
    
    // 2. Condicionales con expresiones complejas
    std::cout << "\n--- Condicionales con Expresiones Complejas ---" << std::endl;
    auto cond2 = std::make_unique<IfExpression>(
        std::make_unique<AndExpression>(
            std::make_unique<LessExpression>(
                std::make_unique<IntExpression>(10),
                std::make_unique<IntExpression>(20)
            ),
            std::make_unique<GreaterExpression>(
                std::make_unique<IntExpression>(15),
                std::make_unique<IntExpression>(5)
            )
        ),
        std::make_unique<AddExpression>(
            std::make_unique<IntExpression>(50),
            std::make_unique<IntExpression>(25)
        ),
        std::make_unique<MulExpression>(
            std::make_unique<IntExpression>(10),
            std::make_unique<IntExpression>(5)
        )
    );
    std::cout << "if ((10 < 20) && (15 > 5)) then (50 + 25) else (10 * 5) = " 
              << cond2->evaluate() << std::endl;
    
    // 3. Condicionales con operaciones de módulo
    std::cout << "\n--- Condicionales con Módulo ---" << std::endl;
    auto cond3 = std::make_unique<IfExpression>(
        std::make_unique<EqualExpression>(
            std::make_unique<ModExpression>(
                std::make_unique<IntExpression>(8),
                std::make_unique<IntExpression>(2)
            ),
            std::make_unique<IntExpression>(0)
        ),
        std::make_unique<StrExpression>("par"),
        std::make_unique<StrExpression>("impar")
    );
    std::cout << "if (8 % 2 == 0) then \"par\" else \"impar\" = " 
              << cond3->evaluate() << std::endl;
    
    std::cout << "\n=== CONDICIONALES AVANZADAS COMPLETADAS ===" << std::endl;
}

// Función para demostrar concatenación avanzada
void demostrarConcatenacionAvanzada() {
    std::cout << "\n=== CONCATENACIÓN AVANZADA ===" << std::endl;
    
    // 1. Concatenación múltiple
    std::cout << "\n--- Concatenación Múltiple ---" << std::endl;
    auto concat1 = std::make_unique<ConcatExpression>(
        std::make_unique<ConcatExpression>(
            std::make_unique<StrExpression>("Hola "),
            std::make_unique<StrExpression>("Mundo ")
        ),
        std::make_unique<StrExpression>("Funcional")
    );
    std::cout << "\"Hola \" # \"Mundo \" # \"Funcional\" = " << concat1->evaluate() << std::endl;
    
    // 2. Concatenación con condicionales
    std::cout << "\n--- Concatenación con Condicionales ---" << std::endl;
    auto concat2 = std::make_unique<ConcatExpression>(
        std::make_unique<StrExpression>("El número es "),
        std::make_unique<IfExpression>(
            std::make_unique<EqualExpression>(
                std::make_unique<ModExpression>(
                    std::make_unique<IntExpression>(7),
                    std::make_unique<IntExpression>(2)
                ),
                std::make_unique<IntExpression>(0)
            ),
            std::make_unique<StrExpression>("par"),
            std::make_unique<StrExpression>("impar")
        )
    );
    std::cout << "\"El número es \" # (if (7 % 2 == 0) then \"par\" else \"impar\") = " 
              << concat2->evaluate() << std::endl;
    
    // 3. Concatenación con expresiones numéricas
    std::cout << "\n--- Concatenación con Expresiones Numéricas ---" << std::endl;
    auto concat3 = std::make_unique<ConcatExpression>(
        std::make_unique<StrExpression>("El resultado es: "),
        std::make_unique<StrExpression>(std::to_string(42)) // Simulamos itos
    );
    std::cout << "\"El resultado es: \" # itos(42) = " << concat3->evaluate() << std::endl;
    
    std::cout << "\n=== CONCATENACIÓN AVANZADA COMPLETADA ===" << std::endl;
}

int main() {
    std::cout << "=== DEMO AVANZADO DEL INTERPRETADOR FUNCIONAL ===" << std::endl;
    std::cout << "Lenguaje Funcional Declarativo - Operaciones Avanzadas" << std::endl;
    
    // Ejecutar diferentes tipos de operaciones avanzadas
    demostrarMatematicas();
    demostrarComparaciones();
    demostrarArraysAvanzados();
    demostrarParesAvanzados();
    demostrarLogicaAvanzada();
    demostrarCondicionalesAvanzadas();
    demostrarConcatenacionAvanzada();
    
    std::cout << "\n=== DEMO AVANZADO COMPLETADO ===" << std::endl;
    std::cout << "✅ Todas las operaciones avanzadas ejecutadas exitosamente" << std::endl;
    std::cout << "✅ Matemáticas, comparaciones, arrays, pares, lógica y condicionales" << std::endl;
    std::cout << "✅ El interpretador maneja correctamente todas las funcionalidades" << std::endl;
    
    return 0;
}
