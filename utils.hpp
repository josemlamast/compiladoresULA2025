#pragma once

#include <algorithm>
#include <forward_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

#ifdef DEBUG
#else
#define LOG(...)
#endif

class Environment;
enum class Datatype;
class PairExpression;
class Expression;

class Expression
{
public:
    virtual ~Expression();

    virtual std::shared_ptr<Expression> eval(Environment&) const = 0;

    virtual std::string to_string() const noexcept = 0;
    
    virtual std::pair<bool, Datatype> type_check(Environment&) const noexcept = 0;
};

class UnaryExpression : public Expression
{
public:
    UnaryExpression(std::shared_ptr<Expression> _expression) noexcept;

    std::shared_ptr<Expression> get_expression() const noexcept;

private:
    std::shared_ptr<Expression> expression;
};

class BinaryExpression : public Expression
{
public:
    BinaryExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) noexcept;

    std::shared_ptr<Expression> get_left_expression() const noexcept;

    std::shared_ptr<Expression> get_right_expression() const noexcept;

private:
    std::shared_ptr<Expression> left_expression;
    std::shared_ptr<Expression> right_expression;
};

using VarList = std::forward_list<std::pair<std::string, std::shared_ptr<Expression>>>;

class Environment : public VarList
{
public:
    using VarList::VarList;

    void add(const std::string& identifier, std::shared_ptr<Expression> expression) noexcept;
    
    std::shared_ptr<Expression> lookup(const std::string& identifier) const noexcept;

    std::string to_string() const noexcept;
};



class Closure : public Expression
{
public:
    Closure(const Environment& _env, const std::string& _param_name, std::shared_ptr<Expression> _body,
            Datatype _param_type, Datatype _return_type) noexcept;

    const Environment& get_environment() const noexcept;

    std::string get_parameter_name() const noexcept;
    std::shared_ptr<Expression> get_body_expression() const noexcept;
    
    Datatype get_parameter_type() const noexcept;
    Datatype get_return_type() const noexcept;

    std::shared_ptr<Expression> eval(Environment&) const override;

    std::string to_string() const noexcept override;
    
    std::pair<bool, Datatype> type_check(Environment&) const noexcept override;

private:
    Environment env;
    std::string param_name;
    std::shared_ptr<Expression> body;
    Datatype parameter_type;
    Datatype return_type;
};

// Estructura para almacenar información de tipos de pares
struct PairTypeInfo {
    Datatype left_type;
    Datatype right_type;
    bool is_nested;

    PairTypeInfo(Datatype left, Datatype right, bool nested) 
        : left_type(left), right_type(right), is_nested(nested) {}

    Datatype get_left_type() const { return left_type; }
    Datatype get_right_type() const { return right_type; }
    bool get_is_nested() const { return is_nested; }
};

// Estructura para rastrear tipos de elementos en pares anidados
struct PairTypePath {
    std::vector<Datatype> type_sequence;  // Secuencia de tipos: [int, real, string, real]
    std::vector<bool> is_left_sequence;   // Secuencia de posiciones: [true, false, true, false]

    PairTypePath() = default;

    // Constructor para crear un path desde una expresión
    PairTypePath(std::shared_ptr<Expression> expr, Environment& env);

    // Obtener el tipo en una posición específica del path
    Datatype get_type_at_position(size_t position) const;

    // Obtener la posición (izquierda/derecha) en una posición específica del path
    bool is_left_at_position(size_t position) const;

    // Obtener la longitud del path
    size_t get_length() const;

    // Verificar si el path es válido
    bool is_valid() const;

    // Obtener el tipo del elemento en la posición especificada
    // position: 0 = primer elemento, 1 = segundo elemento, etc.
    Datatype get_element_type(size_t position) const;

    // Convertir a string para debugging
    std::string to_string() const;

private:
    // Método auxiliar para procesar expresiones de par recursivamente
    void process_pair_expression(std::shared_ptr<PairExpression> pair_expr, Environment& env, bool is_left);
};

// Funciones auxiliares para manejo de pares anidados
PairTypeInfo get_pair_type_info(std::shared_ptr<Expression> expr, Environment& env);

// Función para crear un path de tipos desde una expresión de par anidado
PairTypePath create_pair_type_path(std::shared_ptr<Expression> expr, Environment& env);

// Función para obtener el tipo de un elemento en una posición específica usando fst/snd
std::pair<bool, Datatype> get_nested_pair_element_type(std::shared_ptr<Expression> expr, Environment& env, size_t position);

// Función auxiliar para inferir el tipo de expresiones anidadas sin evaluar
std::pair<bool, Datatype> infer_nested_pair_type(std::shared_ptr<Expression> expr, Environment& env, bool get_left);