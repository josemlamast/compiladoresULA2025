#include <sstream>
#include <utils.hpp>
#include "expression.hpp"

// Forward declarations para evitar dependencias circulares
class PairExpression;
class NameExpression;
class SndExpression;
class FstExpression;

Expression::~Expression()
{
    // empty
}

UnaryExpression::UnaryExpression(std::shared_ptr<Expression> _expression) noexcept
    : expression{_expression}
{
    // empty
}

std::shared_ptr<Expression> UnaryExpression::get_expression() const noexcept
{
    return expression;
}

BinaryExpression::BinaryExpression(std::shared_ptr<Expression> _left_expression, std::shared_ptr<Expression> _right_expression) noexcept
    : left_expression{_left_expression}, right_expression{_right_expression}
{
    // empty
}

std::shared_ptr<Expression> BinaryExpression::get_left_expression() const noexcept
{
    return left_expression;
}

std::shared_ptr<Expression> BinaryExpression::get_right_expression() const noexcept
{
    return right_expression;
}

void Environment::add(const std::string& identifier, std::shared_ptr<Expression> expression) noexcept
{
    this->push_front(std::make_pair(identifier, expression));
}


std::shared_ptr<Expression> Environment::lookup(const std::string& identifier) const noexcept
{
    for (const auto& t : *this)
    {
        if (t.first == identifier)
        {
            return t.second;
        }
    }

    return nullptr;
}

std::string Environment::to_string() const noexcept
{
    std::stringstream out;

    out << "(";

    auto it = this->begin();
    if (it != this->end())
    {
        out << it->first << " -> " << it->second->to_string();
        ++it;
    }

    for (; it != this->end(); ++it)
    {
        auto [key, value] = *it;
        out << ", " << key << " -> " << value->to_string();
    }

    out << ")";

    return out.str();
}


Closure::Closure(const Environment& _env, const std::string& _param_name, std::shared_ptr<Expression> _body,
                 Datatype _param_type, Datatype _return_type) noexcept
    : env{_env}, param_name{_param_name}, body{_body}, parameter_type{_param_type}, return_type{_return_type}
{
    // empty
}

const Environment& Closure::get_environment() const noexcept
{
    return env;
}

std::string Closure::get_parameter_name() const noexcept
{
    return param_name;
}

std::shared_ptr<Expression> Closure::get_body_expression() const noexcept
{
    return body;
}

Datatype Closure::get_parameter_type() const noexcept
{
    return parameter_type;
}

Datatype Closure::get_return_type() const noexcept
{
    return return_type;
}

std::shared_ptr<Expression> Closure::eval(Environment&) const
{
    return std::make_shared<Closure>(env, param_name, body, parameter_type, return_type);
}

std::string Closure::to_string() const noexcept
{
    return "(closure" 
        + env.to_string()
        + " " + param_name + " " + body->to_string() + ")";
}

std::pair<bool, Datatype> Closure::type_check(Environment&) const noexcept
{
    // Un closure es una función, devolvemos FunctionType
    return {true, Datatype::FunctionType};
}

// Implementaciones de PairTypePath y funciones relacionadas

PairTypePath::PairTypePath(std::shared_ptr<Expression> expr, Environment& env) {
    if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(expr)) {
        process_pair_expression(pair_expr, env, true);
    } else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(expr)) {
        // Buscar la variable en el entorno
        for (const auto& pair : env) {
            if (pair.first == var_expr->get_name()) {
                if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                    process_pair_expression(stored_pair, env, true);
                }
                break;
            }
        }
        // Buscar en el entorno global si no se encuentra localmente
        extern Environment global_env;
        for (const auto& pair : global_env) {
            if (pair.first == var_expr->get_name()) {
                if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                    process_pair_expression(stored_pair, env, true);
                }
                break;
            }
        }
    } else if (auto snd_expr = std::dynamic_pointer_cast<SndExpression>(expr)) {
        // Para snd(...), necesitamos obtener el tipo del segundo elemento del par
        auto [expr_ok, expr_type] = snd_expr->get_expression()->type_check(env);
        if (expr_ok && expr_type == Datatype::PairType) {
            if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(snd_expr->get_expression())) {
                // Procesar solo el elemento derecho del par
                auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
                if (right_ok) {
                    if (right_type == Datatype::PairType) {
                        if (auto nested_right = std::dynamic_pointer_cast<PairExpression>(pair_expr->get_right_expression())) {
                            process_pair_expression(nested_right, env, true); // Procesar el par anidado completo
                        }
                    } else {
                        // Tipo básico
                        type_sequence.push_back(right_type);
                        is_left_sequence.push_back(false);
                    }
                }
            } else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(snd_expr->get_expression())) {
                // Buscar la variable en el entorno
                for (const auto& pair : env) {
                    if (pair.first == var_expr->get_name()) {
                        if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                            // Procesar solo el elemento derecho del par
                            auto [right_ok, right_type] = stored_pair->get_right_expression()->type_check(env);
                            if (right_ok) {
                                if (right_type == Datatype::PairType) {
                                    if (auto nested_right = std::dynamic_pointer_cast<PairExpression>(stored_pair->get_right_expression())) {
                                        process_pair_expression(nested_right, env, true); // Procesar el par anidado completo
                                    }
                                } else {
                                    // Tipo básico
                                    type_sequence.push_back(right_type);
                                    is_left_sequence.push_back(false);
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    } else if (auto fst_expr = std::dynamic_pointer_cast<FstExpression>(expr)) {
        // Para fst(...), necesitamos obtener el tipo del primer elemento del par
        auto [expr_ok, expr_type] = fst_expr->get_expression()->type_check(env);
        if (expr_ok && expr_type == Datatype::PairType) {
            if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(fst_expr->get_expression())) {
                process_pair_expression(pair_expr, env, true); // true = left side
            } else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(fst_expr->get_expression())) {
                // Buscar la variable en el entorno
                for (const auto& pair : env) {
                    if (pair.first == var_expr->get_name()) {
                        if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                            process_pair_expression(stored_pair, env, true); // true = left side
                        }
                        break;
                    }
                }
            }
        }
    }
}

void PairTypePath::process_pair_expression(std::shared_ptr<PairExpression> pair_expr, Environment& env, bool is_left) {
    // CRITERIO SEMÁNTICO: Parar en tipos básicos del lenguaje
    
    // Procesar elemento izquierdo
    auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
    if (left_ok) {
        if (left_type == Datatype::PairType) {
            // Es un par anidado, continuar recursivamente
            if (auto nested_left = std::dynamic_pointer_cast<PairExpression>(pair_expr->get_left_expression())) {
                process_pair_expression(nested_left, env, true);
            }
        } else {
            // PARAR: Tipo básico encontrado (int, real, string, bool)
            type_sequence.push_back(left_type);
            is_left_sequence.push_back(true);
        }
    }
    
    // Procesar elemento derecho
    auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
    if (right_ok) {
        if (right_type == Datatype::PairType) {
            // Es un par anidado, continuar recursivamente
            if (auto nested_right = std::dynamic_pointer_cast<PairExpression>(pair_expr->get_right_expression())) {
                process_pair_expression(nested_right, env, false);
            }
        } else {
            // PARAR: Tipo básico encontrado (int, real, string, bool)
            type_sequence.push_back(right_type);
            is_left_sequence.push_back(false);
        }
    }
}

Datatype PairTypePath::get_type_at_position(size_t position) const {
    if (position < type_sequence.size()) {
        return type_sequence[position];
    }
    return Datatype::UnknownType;
}

bool PairTypePath::is_left_at_position(size_t position) const {
    if (position < is_left_sequence.size()) {
        return is_left_sequence[position];
    }
    return false;
}

size_t PairTypePath::get_length() const {
    return type_sequence.size();
}

bool PairTypePath::is_valid() const {
    return !type_sequence.empty() && type_sequence.size() == is_left_sequence.size();
}

Datatype PairTypePath::get_element_type(size_t position) const {
    if (position < type_sequence.size()) {
        return type_sequence[position];
    }
    return Datatype::UnknownType;
}

std::string PairTypePath::to_string() const {
    std::string result = "PairTypePath[";
    for (size_t i = 0; i < type_sequence.size(); ++i) {
        if (i > 0) result += ", ";
        result += std::to_string(static_cast<int>(type_sequence[i]));
        result += (is_left_sequence[i] ? "L" : "R");
    }
    result += "]";
    return result;
}

PairTypeInfo get_pair_type_info(std::shared_ptr<Expression> expr, Environment& env) {
    if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(expr)) {
        auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
        auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
        
        if (left_ok && right_ok) {
            // Verificar si alguno de los elementos es un par anidado por estructura
            bool left_is_pair = (left_type == Datatype::PairType);
            bool right_is_pair = (right_type == Datatype::PairType);
            
            // Verificar también por estructura
            if (auto left_pair = std::dynamic_pointer_cast<PairExpression>(pair_expr->get_left_expression())) {
                left_is_pair = true;
                left_type = Datatype::PairType;
            }
            if (auto right_pair = std::dynamic_pointer_cast<PairExpression>(pair_expr->get_right_expression())) {
                right_is_pair = true;
                right_type = Datatype::PairType;
            } else {
                // Si el tipo es PairType pero no es PairExpression por estructura, forzar el tipo
                if (right_type == Datatype::PairType) {
                    right_is_pair = true;
                }
            }
            
            bool is_nested = (left_is_pair || right_is_pair);
            return PairTypeInfo(left_type, right_type, is_nested);
        }
    } else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(expr)) {
        // Buscar la variable en el entorno
        for (const auto& pair : env) {
            if (pair.first == var_expr->get_name()) {
                if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                    return get_pair_type_info(stored_pair, env);
                }
                break;
            }
        }
        // Buscar en el entorno global si no se encuentra localmente
        extern Environment global_env;
        for (const auto& pair : global_env) {
            if (pair.first == var_expr->get_name()) {
                if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                    return get_pair_type_info(stored_pair, env);
                }
                break;
            }
        }
    }
    
    // Si no se puede determinar, devolver tipos desconocidos
    return PairTypeInfo(Datatype::UnknownType, Datatype::UnknownType, false);
}

PairTypePath create_pair_type_path(std::shared_ptr<Expression> expr, Environment& env) {
    return PairTypePath(expr, env);
}

std::pair<bool, Datatype> get_nested_pair_element_type(std::shared_ptr<Expression> expr, Environment& env, size_t position) {
    PairTypePath path = create_pair_type_path(expr, env);
    if (path.is_valid() && position < path.get_length()) {
        return {true, path.get_element_type(position)};
    }
    return {false, Datatype::UnknownType};
}

std::pair<bool, Datatype> infer_nested_pair_type(std::shared_ptr<Expression> expr, Environment& env, bool get_left) {
    PairTypePath path = create_pair_type_path(expr, env);
    if (path.is_valid()) {
        for (size_t i = 0; i < path.get_length(); ++i) {
            bool is_left_position = path.is_left_at_position(i);
            Datatype type_at_pos = path.get_type_at_position(i);
            if ((get_left && is_left_position) || (!get_left && !is_left_position)) {
                return {true, type_at_pos};
            }
        }
    }
    
    // Fallback para casos simples
    if (auto snd_expr = std::dynamic_pointer_cast<SndExpression>(expr)) {
        auto [expr_ok, expr_type] = snd_expr->get_expression()->type_check(env);
        if (expr_ok && expr_type == Datatype::PairType) {
            return {true, Datatype::PairType};
        }
    } else if (auto fst_expr = std::dynamic_pointer_cast<FstExpression>(expr)) {
        auto [expr_ok, expr_type] = fst_expr->get_expression()->type_check(env);
        if (expr_ok && expr_type == Datatype::PairType) {
            return {true, Datatype::PairType};
        }
    }
    
    return {false, Datatype::UnknownType};
}
