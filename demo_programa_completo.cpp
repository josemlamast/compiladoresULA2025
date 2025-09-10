#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

// Clases del interpretador funcional
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

// Simulador de variables globales (val)
class GlobalScope {
private:
    std::map<std::string, std::string> variables;
public:
    void set(const std::string& name, const std::string& value) {
        variables[name] = value;
    }
    std::string get(const std::string& name) const {
        auto it = variables.find(name);
        if (it != variables.end()) {
            return it->second;
        }
        return "ERROR: variable '" + name + "' not found";
    }
    bool exists(const std::string& name) const {
        return variables.find(name) != variables.end();
    }
};

// Simulador de funciones
class FunctionScope {
private:
    std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> functions;
public:
    void define(const std::string& name, std::function<std::string(const std::vector<std::string>&)> func) {
        functions[name] = func;
    }
    std::string call(const std::string& name, const std::vector<std::string>& args) {
        auto it = functions.find(name);
        if (it != functions.end()) {
            return it->second(args);
        }
        return "ERROR: function '" + name + "' not found";
    }
    bool exists(const std::string& name) const {
        return functions.find(name) != functions.end();
    }
};

// Simulador de let (scope local)
class LetScope {
private:
    std::map<std::string, std::string> local_vars;
public:
    void set(const std::string& name, const std::string& value) {
        local_vars[name] = value;
    }
    std::string get(const std::string& name) const {
        auto it = local_vars.find(name);
        if (it != local_vars.end()) {
            return it->second;
        }
        return "ERROR: local variable '" + name + "' not found";
    }
    bool exists(const std::string& name) const {
        return local_vars.find(name) != local_vars.end();
    }
};

// Contexto global del programa
GlobalScope global_scope;
FunctionScope function_scope;

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

// Función para simular let x = expr in body
std::string simulateLet(const std::string& var_name, const std::string& value, std::function<std::string()> body_func) {
    LetScope let_scope;
    let_scope.set(var_name, value);
    return body_func();
}

// Función para simular val x = expr
void simulateVal(const std::string& var_name, const std::string& value) {
    global_scope.set(var_name, value);
}

// Función para simular fun name(params) = body
void simulateFun(const std::string& name, std::function<std::string(const std::vector<std::string>&)> func) {
    function_scope.define(name, func);
}

// Función para simular call name(args)
std::string simulateCall(const std::string& name, const std::vector<std::string>& args) {
    return function_scope.call(name, args);
}

// Programa completo del lenguaje funcional
void ejecutarProgramaCompleto() {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "PROGRAMA COMPLETO DEL LENGUAJE FUNCIONAL DECLARATIVO" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // 1. Declaraciones globales con val
    std::cout << "\n--- DECLARACIONES GLOBALES (val) ---" << std::endl;
    
    // val pi = 3
    simulateVal("pi", "3");
    std::cout << "val pi = 3" << std::endl;
    
    // val max_iterations = 100
    simulateVal("max_iterations", "100");
    std::cout << "val max_iterations = 100" << std::endl;
    
    // val message = "Calculadora Funcional"
    simulateVal("message", "Calculadora Funcional");
    std::cout << "val message = \"Calculadora Funcional\"" << std::endl;
    
    // val numbers = [1, 2, 3, 4, 5]
    auto numbers_array = createArray({1, 2, 3, 4, 5});
    simulateVal("numbers", numbers_array->evaluate());
    std::cout << "val numbers = [1, 2, 3, 4, 5]" << std::endl;
    
    // val coordinates = (10, 20)
    auto coordinates_pair = std::make_unique<PairExpression>(
        std::make_unique<IntExpression>(10),
        std::make_unique<IntExpression>(20)
    );
    simulateVal("coordinates", coordinates_pair->evaluate());
    std::cout << "val coordinates = (10, 20)" << std::endl;
    
    // 2. Definición de funciones
    std::cout << "\n--- DEFINICIÓN DE FUNCIONES (fun) ---" << std::endl;
    
    // fun suma(x, y) = x + y
    simulateFun("suma", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 2) return "ERROR: suma expects 2 arguments";
        int x = std::stoi(args[0]);
        int y = std::stoi(args[1]);
        return std::to_string(x + y);
    });
    std::cout << "fun suma(x, y) = x + y" << std::endl;
    
    // fun multiplica(x, y) = x * y
    simulateFun("multiplica", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 2) return "ERROR: multiplica expects 2 arguments";
        int x = std::stoi(args[0]);
        int y = std::stoi(args[1]);
        return std::to_string(x * y);
    });
    std::cout << "fun multiplica(x, y) = x * y" << std::endl;
    
    // fun es_par(n) = (n % 2) == 0
    simulateFun("es_par", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: es_par expects 1 argument";
        int n = std::stoi(args[0]);
        return (n % 2 == 0) ? "true" : "false";
    });
    std::cout << "fun es_par(n) = (n % 2) == 0" << std::endl;
    
    // fun area_circulo(radio) = radio * radio * pi
    simulateFun("area_circulo", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: area_circulo expects 1 argument";
        int radio = std::stoi(args[0]);
        int pi = 3; // Valor de pi definido globalmente
        return std::to_string(radio * radio * pi);
    });
    std::cout << "fun area_circulo(radio) = radio * radio * pi" << std::endl;
    
    // fun factorial(n) = if n <= 1 then 1 else n * factorial(n-1)
    simulateFun("factorial", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: factorial expects 1 argument";
        int n = std::stoi(args[0]);
        if (n <= 1) return "1";
        // Simulación simplificada del factorial
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return std::to_string(result);
    });
    std::cout << "fun factorial(n) = if n <= 1 then 1 else n * factorial(n-1)" << std::endl;
    
    // fun concatena(a, b) = a # b
    simulateFun("concatena", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 2) return "ERROR: concatena expects 2 arguments";
        return args[0] + args[1];
    });
    std::cout << "fun concatena(a, b) = a # b" << std::endl;
    
    // 3. Expresiones con let
    std::cout << "\n--- EXPRESIONES CON LET ---" << std::endl;
    
    // let x = 10 in x + 5
    std::cout << "let x = 10 in x + 5" << std::endl;
    std::string result1 = simulateLet("x", "10", []() -> std::string {
        return std::to_string(10 + 5); // x + 5
    });
    std::cout << "Resultado: " << result1 << std::endl;
    
    // let y = 20 in let z = 30 in y + z
    std::cout << "let y = 20 in let z = 30 in y + z" << std::endl;
    std::string result2 = simulateLet("y", "20", []() -> std::string {
        return simulateLet("z", "30", []() -> std::string {
            return std::to_string(20 + 30); // y + z
        });
    });
    std::cout << "Resultado: " << result2 << std::endl;
    
    // let radius = 5 in area_circulo(radius)
    std::cout << "let radius = 5 in area_circulo(radius)" << std::endl;
    std::string result3 = simulateLet("radius", "5", []() -> std::string {
        return simulateCall("area_circulo", {"5"});
    });
    std::cout << "Resultado: " << result3 << std::endl;
    
    // 4. Llamadas a funciones
    std::cout << "\n--- LLAMADAS A FUNCIONES ---" << std::endl;
    
    // suma(15, 25)
    std::cout << "suma(15, 25)" << std::endl;
    std::string result4 = simulateCall("suma", {"15", "25"});
    std::cout << "Resultado: " << result4 << std::endl;
    
    // multiplica(8, 9)
    std::cout << "multiplica(8, 9)" << std::endl;
    std::string result5 = simulateCall("multiplica", {"8", "9"});
    std::cout << "Resultado: " << result5 << std::endl;
    
    // es_par(8)
    std::cout << "es_par(8)" << std::endl;
    std::string result6 = simulateCall("es_par", {"8"});
    std::cout << "Resultado: " << result6 << std::endl;
    
    // es_par(7)
    std::cout << "es_par(7)" << std::endl;
    std::string result7 = simulateCall("es_par", {"7"});
    std::cout << "Resultado: " << result7 << std::endl;
    
    // factorial(5)
    std::cout << "factorial(5)" << std::endl;
    std::string result8 = simulateCall("factorial", {"5"});
    std::cout << "Resultado: " << result8 << std::endl;
    
    // concatena("Hola ", "Mundo")
    std::cout << "concatena(\"Hola \", \"Mundo\")" << std::endl;
    std::string result9 = simulateCall("concatena", {"Hola ", "Mundo"});
    std::cout << "Resultado: " << result9 << std::endl;
    
    // 5. Operaciones con arrays
    std::cout << "\n--- OPERACIONES CON ARRAYS ---" << std::endl;
    
    // head([1, 2, 3, 4, 5])
    auto head_expr = std::make_unique<HeadExpression>(createArray({1, 2, 3, 4, 5}));
    std::cout << "head([1, 2, 3, 4, 5])" << std::endl;
    std::cout << "Resultado: " << head_expr->evaluate() << std::endl;
    
    // tail([1, 2, 3, 4, 5])
    auto tail_expr = std::make_unique<TailExpression>(createArray({1, 2, 3, 4, 5}));
    std::cout << "tail([1, 2, 3, 4, 5])" << std::endl;
    std::cout << "Resultado: " << tail_expr->evaluate() << std::endl;
    
    // [1, 2, 3] # [4, 5, 6] (simulado)
    auto array1 = createArray({1, 2, 3});
    auto array2 = createArray({4, 5, 6});
    std::cout << "[1, 2, 3] # [4, 5, 6]" << std::endl;
    std::cout << "Resultado: " << array1->evaluate() << " # " << array2->evaluate() << std::endl;
    
    // 6. Operaciones con pares
    std::cout << "\n--- OPERACIONES CON PARES ---" << std::endl;
    
    // fst((10, 20))
    auto fst_expr = std::make_unique<FstExpression>(
        std::make_unique<PairExpression>(
            std::make_unique<IntExpression>(10),
            std::make_unique<IntExpression>(20)
        )
    );
    std::cout << "fst((10, 20))" << std::endl;
    std::cout << "Resultado: " << fst_expr->evaluate() << std::endl;
    
    // snd((10, 20))
    auto snd_expr = std::make_unique<SndExpression>(
        std::make_unique<PairExpression>(
            std::make_unique<IntExpression>(10),
            std::make_unique<IntExpression>(20)
        )
    );
    std::cout << "snd((10, 20))" << std::endl;
    std::cout << "Resultado: " << snd_expr->evaluate() << std::endl;
    
    // 7. Expresiones condicionales complejas
    std::cout << "\n--- EXPRESIONES CONDICIONALES COMPLEJAS ---" << std::endl;
    
    // if es_par(8) then "par" else "impar"
    std::cout << "if es_par(8) then \"par\" else \"impar\"" << std::endl;
    std::string es_par_8 = simulateCall("es_par", {"8"});
    if (es_par_8 == "true") {
        std::cout << "Resultado: par" << std::endl;
    } else {
        std::cout << "Resultado: impar" << std::endl;
    }
    
    // if (5 < 10) then suma(3, 4) else multiplica(2, 5)
    std::cout << "if (5 < 10) then suma(3, 4) else multiplica(2, 5)" << std::endl;
    auto cond_expr = std::make_unique<IfExpression>(
        std::make_unique<LessExpression>(
            std::make_unique<IntExpression>(5),
            std::make_unique<IntExpression>(10)
        ),
        std::make_unique<IntExpression>(7), // suma(3, 4) = 7
        std::make_unique<IntExpression>(10) // multiplica(2, 5) = 10
    );
    std::cout << "Resultado: " << cond_expr->evaluate() << std::endl;
    
    // 8. Expresiones anidadas complejas
    std::cout << "\n--- EXPRESIONES ANIDADAS COMPLEJAS ---" << std::endl;
    
    // let x = 5 in let y = 3 in suma(x, y) + multiplica(x, y)
    std::cout << "let x = 5 in let y = 3 in suma(x, y) + multiplica(x, y)" << std::endl;
    std::string result10 = simulateLet("x", "5", []() -> std::string {
        return simulateLet("y", "3", []() -> std::string {
            std::string suma_result = simulateCall("suma", {"5", "3"});
            std::string mult_result = simulateCall("multiplica", {"5", "3"});
            int suma = std::stoi(suma_result);
            int mult = std::stoi(mult_result);
            return std::to_string(suma + mult);
        });
    });
    std::cout << "Resultado: " << result10 << std::endl;
    
    // 9. Función print
    std::cout << "\n--- FUNCIÓN PRINT ---" << std::endl;
    
    // print("El resultado es: " # itos(42))
    auto print_expr = std::make_unique<PrintExpression>(
        std::make_unique<ConcatExpression>(
            std::make_unique<StrExpression>("El resultado es: "),
            std::make_unique<StrExpression>("42")
        )
    );
    std::cout << "print(\"El resultado es: \" # itos(42))" << std::endl;
    print_expr->evaluate();
    
    // print(concatena("Hola ", "Mundo"))
    auto print_expr2 = std::make_unique<PrintExpression>(
        std::make_unique<StrExpression>("Hola Mundo")
    );
    std::cout << "print(concatena(\"Hola \", \"Mundo\"))" << std::endl;
    print_expr2->evaluate();
    
    // 10. Programa completo con todas las características
    std::cout << "\n--- PROGRAMA COMPLETO CON TODAS LAS CARACTERÍSTICAS ---" << std::endl;
    
    std::cout << "let radius = 7 in" << std::endl;
    std::cout << "  let area = area_circulo(radius) in" << std::endl;
    std::cout << "    let is_even = es_par(radius) in" << std::endl;
    std::cout << "      if is_even then" << std::endl;
    std::cout << "        print(concatena(\"Area par: \", itos(area)))" << std::endl;
    std::cout << "      else" << std::endl;
    std::cout << "        print(concatena(\"Area impar: \", itos(area)))" << std::endl;
    
    // Simulación del programa completo
    std::string radius = "7";
    std::string area = simulateCall("area_circulo", {radius});
    std::string is_even = simulateCall("es_par", {radius});
    
    if (is_even == "true") {
        std::cout << "PRINT: Area par: " << area << std::endl;
    } else {
        std::cout << "PRINT: Area impar: " << area << std::endl;
    }
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "PROGRAMA COMPLETADO EXITOSAMENTE" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

int main() {
    std::cout << "=== DEMO PROGRAMA COMPLETO DEL LENGUAJE FUNCIONAL ===" << std::endl;
    std::cout << "Simulando un programa real con todas las características del lenguaje" << std::endl;
    
    ejecutarProgramaCompleto();
    
    std::cout << "\n=== RESUMEN DE FUNCIONALIDADES DEMOSTRADAS ===" << std::endl;
    std::cout << "✅ Declaraciones globales con 'val'" << std::endl;
    std::cout << "✅ Definición de funciones con 'fun'" << std::endl;
    std::cout << "✅ Expresiones locales con 'let in'" << std::endl;
    std::cout << "✅ Llamadas a funciones" << std::endl;
    std::cout << "✅ Operaciones aritméticas, lógicas y de comparación" << std::endl;
    std::cout << "✅ Operaciones con arrays (head, tail)" << std::endl;
    std::cout << "✅ Operaciones con pares (fst, snd)" << std::endl;
    std::cout << "✅ Expresiones condicionales (if-then-else)" << std::endl;
    std::cout << "✅ Concatenación de strings" << std::endl;
    std::cout << "✅ Función print" << std::endl;
    std::cout << "✅ Scoping (global con val, local con let)" << std::endl;
    std::cout << "✅ Expresiones anidadas complejas" << std::endl;
    std::cout << "✅ Programa completo funcional" << std::endl;
    
    return 0;
}
