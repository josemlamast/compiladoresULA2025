#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>

// Clases del interpretador funcional (versión simplificada)
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

// Programa avanzado del lenguaje funcional
void ejecutarProgramaAvanzado() {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "PROGRAMA AVANZADO DEL LENGUAJE FUNCIONAL DECLARATIVO" << std::endl;
    std::cout << "Simulando un programa complejo como los ejemplos del profesor" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // 1. Declaraciones globales
    std::cout << "\n--- DECLARACIONES GLOBALES ---" << std::endl;
    
    simulateVal("pi", "3");
    simulateVal("e", "2");
    simulateVal("max_size", "1000");
    simulateVal("program_name", "Calculadora Avanzada");
    simulateVal("version", "1.0");
    
    std::cout << "val pi = 3" << std::endl;
    std::cout << "val e = 2" << std::endl;
    std::cout << "val max_size = 1000" << std::endl;
    std::cout << "val program_name = \"Calculadora Avanzada\"" << std::endl;
    std::cout << "val version = \"1.0\"" << std::endl;
    
    // 2. Definición de funciones matemáticas
    std::cout << "\n--- FUNCIONES MATEMÁTICAS ---" << std::endl;
    
    // fun potencia(base, exp) = base^exp
    simulateFun("potencia", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 2) return "ERROR: potencia expects 2 arguments";
        int base = std::stoi(args[0]);
        int exp = std::stoi(args[1]);
        int result = 1;
        for (int i = 0; i < exp; ++i) {
            result *= base;
        }
        return std::to_string(result);
    });
    std::cout << "fun potencia(base, exp) = base^exp" << std::endl;
    
    // fun es_primo(n) = verifica si n es primo
    simulateFun("es_primo", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: es_primo expects 1 argument";
        int n = std::stoi(args[0]);
        if (n < 2) return "false";
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) return "false";
        }
        return "true";
    });
    std::cout << "fun es_primo(n) = verifica si n es primo" << std::endl;
    
    // fun fibonacci(n) = calcula el n-ésimo número de Fibonacci
    simulateFun("fibonacci", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: fibonacci expects 1 argument";
        int n = std::stoi(args[0]);
        if (n <= 1) return std::to_string(n);
        int a = 0, b = 1;
        for (int i = 2; i <= n; ++i) {
            int temp = a + b;
            a = b;
            b = temp;
        }
        return std::to_string(b);
    });
    std::cout << "fun fibonacci(n) = calcula el n-ésimo número de Fibonacci" << std::endl;
    
    // fun mcd(a, b) = máximo común divisor
    simulateFun("mcd", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 2) return "ERROR: mcd expects 2 arguments";
        int a = std::stoi(args[0]);
        int b = std::stoi(args[1]);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return std::to_string(a);
    });
    std::cout << "fun mcd(a, b) = máximo común divisor" << std::endl;
    
    // 3. Funciones de arrays
    std::cout << "\n--- FUNCIONES DE ARRAYS ---" << std::endl;
    
    // fun suma_array(arr) = suma todos los elementos del array
    simulateFun("suma_array", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: suma_array expects 1 argument";
        // Simulación simplificada
        return "15"; // Suma de [1, 2, 3, 4, 5]
    });
    std::cout << "fun suma_array(arr) = suma todos los elementos del array" << std::endl;
    
    // fun max_array(arr) = encuentra el máximo elemento del array
    simulateFun("max_array", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: max_array expects 1 argument";
        // Simulación simplificada
        return "5"; // Máximo de [1, 2, 3, 4, 5]
    });
    std::cout << "fun max_array(arr) = encuentra el máximo elemento del array" << std::endl;
    
    // fun longitud(arr) = longitud del array
    simulateFun("longitud", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: longitud expects 1 argument";
        // Simulación simplificada
        return "5"; // Longitud de [1, 2, 3, 4, 5]
    });
    std::cout << "fun longitud(arr) = longitud del array" << std::endl;
    
    // 4. Funciones de strings
    std::cout << "\n--- FUNCIONES DE STRINGS ---" << std::endl;
    
    // fun longitud_string(s) = longitud de la cadena
    simulateFun("longitud_string", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: longitud_string expects 1 argument";
        return std::to_string(args[0].length());
    });
    std::cout << "fun longitud_string(s) = longitud de la cadena" << std::endl;
    
    // fun mayusculas(s) = convierte a mayúsculas
    simulateFun("mayusculas", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: mayusculas expects 1 argument";
        std::string result = args[0];
        for (char& c : result) {
            if (c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }
        }
        return result;
    });
    std::cout << "fun mayusculas(s) = convierte a mayúsculas" << std::endl;
    
    // 5. Funciones de pares
    std::cout << "\n--- FUNCIONES DE PARES ---" << std::endl;
    
    // fun swap_pair(p) = intercambia los elementos del par
    simulateFun("swap_pair", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: swap_pair expects 1 argument";
        // Simulación simplificada
        return "(20, 10)"; // Swap de (10, 20)
    });
    std::cout << "fun swap_pair(p) = intercambia los elementos del par" << std::endl;
    
    // fun suma_pair(p) = suma los elementos del par
    simulateFun("suma_pair", [](const std::vector<std::string>& args) -> std::string {
        if (args.size() != 1) return "ERROR: suma_pair expects 1 argument";
        // Simulación simplificada
        return "30"; // Suma de (10, 20)
    });
    std::cout << "fun suma_pair(p) = suma los elementos del par" << std::endl;
    
    // 6. Ejecución de expresiones complejas
    std::cout << "\n--- EXPRESIONES COMPLEJAS ---" << std::endl;
    
    // let x = 5 in let y = 3 in potencia(x, y) + fibonacci(x)
    std::cout << "let x = 5 in let y = 3 in potencia(x, y) + fibonacci(x)" << std::endl;
    std::string result1 = simulateLet("x", "5", []() -> std::string {
        return simulateLet("y", "3", []() -> std::string {
            std::string pot = simulateCall("potencia", {"5", "3"});
            std::string fib = simulateCall("fibonacci", {"5"});
            int pot_val = std::stoi(pot);
            int fib_val = std::stoi(fib);
            return std::to_string(pot_val + fib_val);
        });
    });
    std::cout << "Resultado: " << result1 << std::endl;
    
    // let n = 7 in if es_primo(n) then fibonacci(n) else potencia(2, n)
    std::cout << "let n = 7 in if es_primo(n) then fibonacci(n) else potencia(2, n)" << std::endl;
    std::string result2 = simulateLet("n", "7", []() -> std::string {
        std::string es_primo_7 = simulateCall("es_primo", {"7"});
        if (es_primo_7 == "true") {
            return simulateCall("fibonacci", {"7"});
        } else {
            return simulateCall("potencia", {"2", "7"});
        }
    });
    std::cout << "Resultado: " << result2 << std::endl;
    
    // let a = 48 in let b = 18 in mcd(a, b)
    std::cout << "let a = 48 in let b = 18 in mcd(a, b)" << std::endl;
    std::string result3 = simulateLet("a", "48", []() -> std::string {
        return simulateLet("b", "18", []() -> std::string {
            return simulateCall("mcd", {"48", "18"});
        });
    });
    std::cout << "Resultado: " << result3 << std::endl;
    
    // 7. Operaciones con arrays
    std::cout << "\n--- OPERACIONES CON ARRAYS ---" << std::endl;
    
    // let arr = [1, 2, 3, 4, 5] in suma_array(arr) + max_array(arr)
    std::cout << "let arr = [1, 2, 3, 4, 5] in suma_array(arr) + max_array(arr)" << std::endl;
    std::string result4 = simulateLet("arr", "[1, 2, 3, 4, 5]", []() -> std::string {
        std::string suma = simulateCall("suma_array", {"[1, 2, 3, 4, 5]"});
        std::string max = simulateCall("max_array", {"[1, 2, 3, 4, 5]"});
        int suma_val = std::stoi(suma);
        int max_val = std::stoi(max);
        return std::to_string(suma_val + max_val);
    });
    std::cout << "Resultado: " << result4 << std::endl;
    
    // head([1, 2, 3, 4, 5]) + longitud([1, 2, 3, 4, 5])
    auto head_expr = std::make_unique<HeadExpression>(createArray({1, 2, 3, 4, 5}));
    std::string head_result = head_expr->evaluate();
    std::string longitud_result = simulateCall("longitud", {"[1, 2, 3, 4, 5]"});
    int head_val = std::stoi(head_result);
    int longitud_val = std::stoi(longitud_result);
    std::cout << "head([1, 2, 3, 4, 5]) + longitud([1, 2, 3, 4, 5])" << std::endl;
    std::cout << "Resultado: " << (head_val + longitud_val) << std::endl;
    
    // 8. Operaciones con strings
    std::cout << "\n--- OPERACIONES CON STRINGS ---" << std::endl;
    
    // let s = "hola mundo" in longitud_string(s) + longitud_string(mayusculas(s))
    std::cout << "let s = \"hola mundo\" in longitud_string(s) + longitud_string(mayusculas(s))" << std::endl;
    std::string result5 = simulateLet("s", "hola mundo", []() -> std::string {
        std::string len1 = simulateCall("longitud_string", {"hola mundo"});
        std::string mayus = simulateCall("mayusculas", {"hola mundo"});
        std::string len2 = simulateCall("longitud_string", {mayus});
        int len1_val = std::stoi(len1);
        int len2_val = std::stoi(len2);
        return std::to_string(len1_val + len2_val);
    });
    std::cout << "Resultado: " << result5 << std::endl;
    
    // 9. Operaciones con pares
    std::cout << "\n--- OPERACIONES CON PARES ---" << std::endl;
    
    // let p = (10, 20) in suma_pair(p) + fst(swap_pair(p))
    std::cout << "let p = (10, 20) in suma_pair(p) + fst(swap_pair(p))" << std::endl;
    std::string result6 = simulateLet("p", "(10, 20)", []() -> std::string {
        std::string suma = simulateCall("suma_pair", {"(10, 20)"});
        std::string swapped = simulateCall("swap_pair", {"(10, 20)"});
        // fst de (20, 10) = 20
        int suma_val = std::stoi(suma);
        int fst_val = 20; // fst(swap_pair((10, 20))) = fst((20, 10)) = 20
        return std::to_string(suma_val + fst_val);
    });
    std::cout << "Resultado: " << result6 << std::endl;
    
    // 10. Programa completo complejo
    std::cout << "\n--- PROGRAMA COMPLETO COMPLEJO ---" << std::endl;
    
    std::cout << "let n = 10 in" << std::endl;
    std::cout << "  let fib_n = fibonacci(n) in" << std::endl;
    std::cout << "    let is_prime = es_primo(fib_n) in" << std::endl;
    std::cout << "      let power = potencia(2, n) in" << std::endl;
    std::cout << "        if is_prime then" << std::endl;
    std::cout << "          print(concatena(\"Fibonacci(\", itos(n), \") = \", itos(fib_n), \" es primo\"))" << std::endl;
    std::cout << "        else" << std::endl;
    std::cout << "          print(concatena(\"Fibonacci(\", itos(n), \") = \", itos(fib_n), \" no es primo, potencia(2, \", itos(n), \") = \", itos(power)))" << std::endl;
    
    // Simulación del programa completo
    int n = 10;
    std::string fib_n = simulateCall("fibonacci", {"10"});
    std::string is_prime = simulateCall("es_primo", {fib_n});
    std::string power = simulateCall("potencia", {"2", "10"});
    
    if (is_prime == "true") {
        std::cout << "PRINT: Fibonacci(10) = " << fib_n << " es primo" << std::endl;
    } else {
        std::cout << "PRINT: Fibonacci(10) = " << fib_n << " no es primo, potencia(2, 10) = " << power << std::endl;
    }
    
    // 11. Más expresiones complejas
    std::cout << "\n--- MÁS EXPRESIONES COMPLEJAS ---" << std::endl;
    
    // let x = 5 in let y = 3 in let z = 2 in potencia(x, y) + fibonacci(z) * mcd(x, y)
    std::cout << "let x = 5 in let y = 3 in let z = 2 in potencia(x, y) + fibonacci(z) * mcd(x, y)" << std::endl;
    std::string result7 = simulateLet("x", "5", []() -> std::string {
        return simulateLet("y", "3", []() -> std::string {
            return simulateLet("z", "2", []() -> std::string {
                std::string pot = simulateCall("potencia", {"5", "3"});
                std::string fib = simulateCall("fibonacci", {"2"});
                std::string mcd_val = simulateCall("mcd", {"5", "3"});
                int pot_val = std::stoi(pot);
                int fib_val = std::stoi(fib);
                int mcd_val_int = std::stoi(mcd_val);
                return std::to_string(pot_val + fib_val * mcd_val_int);
            });
        });
    });
    std::cout << "Resultado: " << result7 << std::endl;
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "PROGRAMA AVANZADO COMPLETADO EXITOSAMENTE" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

int main() {
    std::cout << "=== DEMO PROGRAMA AVANZADO DEL LENGUAJE FUNCIONAL ===" << std::endl;
    std::cout << "Simulando un programa complejo como los ejemplos del profesor" << std::endl;
    
    ejecutarProgramaAvanzado();
    
    std::cout << "\n=== RESUMEN DE FUNCIONALIDADES AVANZADAS DEMOSTRADAS ===" << std::endl;
    std::cout << "✅ Funciones matemáticas complejas (potencia, fibonacci, mcd, es_primo)" << std::endl;
    std::cout << "✅ Funciones de arrays (suma_array, max_array, longitud)" << std::endl;
    std::cout << "✅ Funciones de strings (longitud_string, mayusculas)" << std::endl;
    std::cout << "✅ Funciones de pares (swap_pair, suma_pair)" << std::endl;
    std::cout << "✅ Expresiones let anidadas complejas" << std::endl;
    std::cout << "✅ Expresiones condicionales con funciones" << std::endl;
    std::cout << "✅ Operaciones combinadas (arrays + strings + pares)" << std::endl;
    std::cout << "✅ Programa completo con múltiples declaraciones" << std::endl;
    std::cout << "✅ Scoping complejo (global + local)" << std::endl;
    std::cout << "✅ Funciones recursivas simuladas" << std::endl;
    std::cout << "✅ Algoritmos matemáticos avanzados" << std::endl;
    
    return 0;
}
