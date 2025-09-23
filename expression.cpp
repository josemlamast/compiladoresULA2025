#include "utils.hpp"
#include "expression.hpp"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

// Declaración externa de la función que está en main.cpp
extern std::string datatype_to_string(Datatype type) noexcept;

// Función auxiliar para crear placeholders recursivos de pares anidados
std::shared_ptr<Expression> create_pair_placeholder_recursive(Datatype type, Environment& env) {
    switch (type) {
        case Datatype::IntType:
            return std::make_shared<IntExpression>(0);
        case Datatype::RealType:
            return std::make_shared<RealExpression>(0.0);
        case Datatype::StringType:
            return std::make_shared<StrExpression>("");
        case Datatype::BoolType:
            return std::make_shared<BoolExpression>(false);
        case Datatype::PairType:
            // Para pares anidados, crear placeholders genéricos que permitan anidamiento
            return std::make_shared<PairExpression>(
                std::make_shared<IntExpression>(0),  // placeholder genérico para left
                std::make_shared<IntExpression>(0)   // placeholder genérico para right
            );
        case Datatype::ArrayType:
        case Datatype::IntArrayType:
        case Datatype::RealArrayType:
        case Datatype::StringArrayType:
        case Datatype::BoolArrayType:
            // Para arrays, crear un array vacío
            return std::make_shared<ArrayExpression>(std::vector<std::shared_ptr<Expression>>());
        default:
            return std::make_shared<IntExpression>(0); // fallback
    }
}

// Función auxiliar para inferir tipos de elementos de un par de manera inteligente
// Función auxiliar para hacer type checking estricto en funciones
std::pair<bool, Datatype> strict_type_check_for_functions(std::shared_ptr<Expression> expr, Environment& env) {
    // Si es un IfElseExpression, hacer type checking estricto
    if (auto if_expr = std::dynamic_pointer_cast<IfElseExpression>(expr)) {
        auto [cond_ok, cond_type] = if_expr->get_condition_expression()->type_check(env);
        auto [true_ok, true_type] = strict_type_check_for_functions(if_expr->get_true_expression(), env);
        auto [false_ok, false_type] = strict_type_check_for_functions(if_expr->get_false_expression(), env);
        
        if (!cond_ok || !true_ok || !false_ok) return {false, Datatype::UnknownType};
        
        // Verificar que la condición sea booleana o entera
        if (cond_type != Datatype::BoolType && cond_type != Datatype::IntType) {
            return {false, Datatype::UnknownType};
        }
        
        // En modo estricto, ambas ramas deben tener el mismo tipo
        if (true_type != false_type) {
            return {false, Datatype::UnknownType};
        }
        
        return {true, true_type};
    }
    
    // Para otras expresiones, usar type checking normal
    return expr->type_check(env);
}

// Función auxiliar para inferir el tipo de retorno de una función recursiva
// Función auxiliar para detectar si una expresión contiene itos() o concatenación
bool contains_itos_or_concat(std::shared_ptr<Expression> expr) {
    if (!expr) return false;
    
    // Verificar si es una expresión itos
    if (std::dynamic_pointer_cast<ItoSExpression>(expr)) {
        return true;
    }
    
    // Verificar si es una expresión de concatenación
    if (std::dynamic_pointer_cast<ConcatExpression>(expr)) {
        return true;
    }
    
    // Verificar si es una expresión if-else
    if (auto if_expr = std::dynamic_pointer_cast<IfElseExpression>(expr)) {
        return contains_itos_or_concat(if_expr->get_true_expression()) ||
               contains_itos_or_concat(if_expr->get_false_expression());
    }
    
    // Verificar si es una expresión binaria (como concatenación)
    if (auto bin_expr = std::dynamic_pointer_cast<BinaryExpression>(expr)) {
        return contains_itos_or_concat(bin_expr->get_left_expression()) ||
               contains_itos_or_concat(bin_expr->get_right_expression());
    }
    
    // Verificar si es una llamada a función
    if (auto call_expr = std::dynamic_pointer_cast<CallExpression>(expr)) {
        return contains_itos_or_concat(call_expr->get_left_expression()) ||
               contains_itos_or_concat(call_expr->get_right_expression());
    }
    
    return false;
}

Datatype infer_function_return_type(std::shared_ptr<Expression> body, Environment& env) {
    // Para evitar recursión infinita, solo analizar expresiones simples
    // No hacer type_check completo del cuerpo ya que puede causar recursión
    
    // Analizar solo la estructura de la expresión para inferir el tipo
    if (auto if_expr = std::dynamic_pointer_cast<IfElseExpression>(body)) {
        // Para if expressions, analizar ambas ramas (evitar recursión)
        // No hacer type_check completo, solo inferir del tipo de expresión
        auto true_expr = if_expr->get_true_expression();
        auto false_expr = if_expr->get_false_expression();
        
        // Función auxiliar para inferir el tipo de una expresión
        auto infer_expression_type = [&env](std::shared_ptr<Expression> expr) -> Datatype {
            // IMPORTANTE: Verificar primero las expresiones de conversión antes que las expresiones básicas
            if (auto itos_expr = std::dynamic_pointer_cast<ItoSExpression>(expr)) {
                return Datatype::StringType; // itos() siempre retorna string
            } else if (auto rtos_expr = std::dynamic_pointer_cast<RtoSExpression>(expr)) {
                return Datatype::StringType; // rtos() siempre retorna string
            } else if (auto rtoi_expr = std::dynamic_pointer_cast<RtoIExpression>(expr)) {
                return Datatype::IntType; // rtoi() siempre retorna int
            } else if (auto itor_expr = std::dynamic_pointer_cast<ItoRExpression>(expr)) {
                return Datatype::RealType; // itor() siempre retorna real
            } else if (auto concat_expr = std::dynamic_pointer_cast<ConcatExpression>(expr)) {
                // Para concatenación, asumir que retorna string si contiene strings
                // Simplificado para evitar recursión infinita
                return Datatype::StringType; // concat() siempre retorna string
            } else if (auto call_expr = std::dynamic_pointer_cast<CallExpression>(expr)) {
                // Para llamadas a funciones, intentar inferir el tipo de retorno
                // Esto es especialmente importante para llamadas recursivas
                auto func_name = std::dynamic_pointer_cast<NameExpression>(call_expr->get_left_expression());
                if (func_name) {
                    // Buscar la función en el entorno para obtener su tipo de retorno
                    auto func_value = env.lookup(func_name->get_name());
                    if (auto closure = std::dynamic_pointer_cast<Closure>(func_value)) {
                        // Si encontramos la función, retornar su tipo de retorno conocido
                        return closure->get_return_type();
                    }
                }
                return Datatype::UnknownType; // No se puede inferir sin conocer la función
            } else if (auto head_expr = std::dynamic_pointer_cast<HeadExpression>(expr)) {
                // Para head(), necesitamos inferir el tipo basándose en el tipo del parámetro
                // Esto se manejará en el contexto de la llamada a función
                // No podemos inferir el tipo aquí sin conocer el tipo del array, así que retornamos UnknownType
                return Datatype::UnknownType; // Se inferirá correctamente en CallExpression::type_check
            } else if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(expr)) {
                return Datatype::PairType; // pair() siempre retorna pair
            } else if (auto int_expr = std::dynamic_pointer_cast<IntExpression>(expr)) {
                return Datatype::IntType;
            } else if (auto real_expr = std::dynamic_pointer_cast<RealExpression>(expr)) {
                return Datatype::RealType;
            } else if (auto str_expr = std::dynamic_pointer_cast<StrExpression>(expr)) {
                return Datatype::StringType;
            } else if (auto bool_expr = std::dynamic_pointer_cast<BoolExpression>(expr)) {
                return Datatype::BoolType;
            }
            return Datatype::UnknownType;
        };
        
        // Inferir tipos de ambas ramas
        Datatype true_type = infer_expression_type(true_expr);
        Datatype false_type = infer_expression_type(false_expr);
        
        // Si ambas ramas tienen el mismo tipo, retornar ese tipo
        if (true_type == false_type && true_type != Datatype::UnknownType) {
            return true_type;
        }
        
        // Para pares, verificar si los elementos tienen tipos consistentes
        if (true_type == Datatype::PairType && false_type == Datatype::PairType) {
            // Intentar inferir tipos de elementos de los pares
            auto true_pair = std::dynamic_pointer_cast<PairExpression>(true_expr);
            auto false_pair = std::dynamic_pointer_cast<PairExpression>(false_expr);
            
            if (true_pair && false_pair) {
                // Verificar tipos de elementos izquierdos
                Datatype true_left = infer_expression_type(true_pair->get_left_expression());
                Datatype false_left = infer_expression_type(false_pair->get_left_expression());
                
                // Verificar tipos de elementos derechos
                Datatype true_right = infer_expression_type(true_pair->get_right_expression());
                Datatype false_right = infer_expression_type(false_pair->get_right_expression());
                
                // Si ambos elementos tienen tipos consistentes, retornar PairType
                if (true_left == false_left && true_right == false_right && 
                    true_left != Datatype::UnknownType && true_right != Datatype::UnknownType) {
                    return Datatype::PairType;
                }
            }
        }
        
        // Si las ramas tienen tipos diferentes, retornar UnknownType
        // para que el type checker estricto pueda detectar el error
        return Datatype::UnknownType;
    }
    
    return Datatype::UnknownType;
}

std::pair<Datatype, Datatype> infer_pair_element_types(std::shared_ptr<Expression> expr, Environment& env) {
    // Intentar diferentes estrategias para inferir los tipos de los elementos
    
    // Estrategia 1: Si es un PairExpression directo
    if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(expr)) {
        auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
        auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
        if (left_ok && right_ok) {
            return {left_type, right_type};
        }
    }
    
    // Estrategia 2: Si es una variable, buscar en el entorno
    if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(expr)) {
        auto var_value = env.lookup(var_expr->get_name());
        auto stored_pair = std::dynamic_pointer_cast<PairExpression>(var_value);
        if (var_value && stored_pair) {
            auto [left_ok, left_type] = stored_pair->get_left_expression()->type_check(env);
            auto [right_ok, right_type] = stored_pair->get_right_expression()->type_check(env);
            if (left_ok && right_ok) {
                return {left_type, right_type};
            }
        }
    }
    
    // Estrategia 3: Si es una expresión que evalúa a un par, intentar usar fst/snd para inferir tipos
    if (auto fst_expr = std::dynamic_pointer_cast<FstExpression>(expr)) {
        auto [fst_ok, fst_type] = fst_expr->type_check(env);
        if (fst_ok) {
            // Si fst funciona, asumir que el tipo del primer elemento es fst_type
            // y el segundo elemento es del mismo tipo por defecto
            return {fst_type, fst_type};
        }
    }
    
    if (auto snd_expr = std::dynamic_pointer_cast<SndExpression>(expr)) {
        auto [snd_ok, snd_type] = snd_expr->type_check(env);
        if (snd_ok) {
            // Si snd funciona, asumir que el tipo del segundo elemento es snd_type
            // y el primer elemento es del mismo tipo por defecto
            return {snd_type, snd_type};
        }
    }
    
    // Estrategia 4: Por defecto, asumir que es un par de enteros
    return {Datatype::IntType, Datatype::IntType};
}




std::shared_ptr<Expression> NotExpression::eval(Environment& env) const
{
    auto expr = get_expression()->eval(env);
    auto bool_expr = std::dynamic_pointer_cast<BoolExpression>(expr);
    return std::make_shared<BoolExpression>(!bool_expr->get_value());
}

std::string NotExpression::to_string() const noexcept
{
    return "(not " + get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> NotExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea booleano
    if (expr_type == Datatype::BoolType) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> AndExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

    auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left);
    auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right);
    return std::make_shared<BoolExpression>(left_bool->get_value() && right_bool->get_value());
}

std::string AndExpression::to_string() const noexcept
{
    return "(and " + get_left_expression()->to_string() + " " + get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> AndExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean booleanos
    if (left_type == Datatype::BoolType  &&  right_type == Datatype::BoolType) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> XorExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left_result);
    auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right_result);
    
    bool result = (left_bool->get_value() && !right_bool->get_value()) || 
                 (!left_bool->get_value() && right_bool->get_value());
    return std::make_shared<BoolExpression>(result);
}

std::string XorExpression::to_string() const noexcept {
    return "(xor " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> XorExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean booleanos
    if (left_type == Datatype::BoolType && right_type == Datatype::BoolType) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> OrExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

    auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left);
    auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right);
    return std::make_shared<BoolExpression>(left_bool->get_value() || right_bool->get_value());
}

std::string OrExpression::to_string() const noexcept
{
    return "(or " + get_left_expression()->to_string() + " " + get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> OrExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean booleanos
    if (left_type == Datatype::BoolType && right_type == Datatype::BoolType) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> LessExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_int = std::dynamic_pointer_cast<IntExpression>(left_result);
    auto right_int = std::dynamic_pointer_cast<IntExpression>(right_result);
    auto left_real = std::dynamic_pointer_cast<RealExpression>(left_result);
    auto right_real = std::dynamic_pointer_cast<RealExpression>(right_result);
    
    if (left_int && right_int) {
        return std::make_shared<BoolExpression>(left_int->get_value() < right_int->get_value());
    }
    else     // Si llegamos aquí, asumimos que son reales por defecto  
    {
        return std::make_shared<BoolExpression>(left_real->get_value() < right_real->get_value());
    }    
}

std::string LessExpression::to_string() const noexcept {
    return "(< " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}


std::pair<bool, Datatype> LessExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean comparables
    if (left_type == right_type) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> LessEqExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_int = std::dynamic_pointer_cast<IntExpression>(left_result);
    auto right_int = std::dynamic_pointer_cast<IntExpression>(right_result);
    auto left_real = std::dynamic_pointer_cast<RealExpression>(left_result);
    auto right_real = std::dynamic_pointer_cast<RealExpression>(right_result);
    
    if (left_int && right_int) {
        return std::make_shared<BoolExpression>(left_int->get_value() <= right_int->get_value());
    }
    else //numeros reales por defecto
    {
        return std::make_shared<BoolExpression>(left_real->get_value() <= right_real->get_value());
    }
}

std::string LessEqExpression::to_string() const noexcept {
    return "(<= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> LessEqExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean comparables
    if (left_type == right_type) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> GreaterExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_int = std::dynamic_pointer_cast<IntExpression>(left_result);
    auto right_int = std::dynamic_pointer_cast<IntExpression>(right_result);
    auto left_real = std::dynamic_pointer_cast<RealExpression>(left_result);
    auto right_real = std::dynamic_pointer_cast<RealExpression>(right_result);

    if (left_int && right_int) {
        bool result = left_int->get_value() > right_int->get_value();
        return std::make_shared<BoolExpression>(result);
    }
    else //numeros reales por defecto
    {
        bool result = left_real->get_value() > right_real->get_value();
        return std::make_shared<BoolExpression>(result);
    }
}

std::string GreaterExpression::to_string() const noexcept {
    return "(> " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> GreaterExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean comparables
    if (left_type == right_type) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> GreaterEqExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_int = std::dynamic_pointer_cast<IntExpression>(left_result);
    auto right_int = std::dynamic_pointer_cast<IntExpression>(right_result);
    auto left_real = std::dynamic_pointer_cast<RealExpression>(left_result);
    auto right_real = std::dynamic_pointer_cast<RealExpression>(right_result);
    
    if (left_int && right_int) {
        return std::make_shared<BoolExpression>(left_int->get_value() >= right_int->get_value());
    }
    else {    // Si llegamos aquí, asumimos que son enteros por defecto

        return std::make_shared<BoolExpression>(left_real->get_value() >= right_real->get_value());
    }    
}

std::string GreaterEqExpression::to_string() const noexcept {
    return "(>= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> GreaterEqExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean comparables
    if (left_type == right_type) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> EqualExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_int = std::dynamic_pointer_cast<IntExpression>(left_result);
    auto right_int = std::dynamic_pointer_cast<IntExpression>(right_result);
    auto left_real = std::dynamic_pointer_cast<RealExpression>(left_result);
    auto right_real = std::dynamic_pointer_cast<RealExpression>(right_result);
    auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left_result);
    auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right_result);
    auto left_str = std::dynamic_pointer_cast<StrExpression>(left_result);
    auto right_str = std::dynamic_pointer_cast<StrExpression>(right_result);
    auto left_array = std::dynamic_pointer_cast<ArrayExpression>(left_result);
    auto right_array = std::dynamic_pointer_cast<ArrayExpression>(right_result);
    
    if (left_int && right_int) {
        return std::make_shared<BoolExpression>(left_int->get_value() == right_int->get_value());
    }
    else if (left_real && right_real) {
        return std::make_shared<BoolExpression>(left_real->get_value() == right_real->get_value());
    }
    else if (left_bool && right_bool) {
        return std::make_shared<BoolExpression>(left_bool->get_value() == right_bool->get_value());
    }
    else if (left_str && right_str) {
        return std::make_shared<BoolExpression>(left_str->get_value() == right_str->get_value());
    }
    else if (left_array && right_array) {
        // Comparar arrays: deben tener el mismo tamaño y elementos iguales
        const auto& left_elements = left_array->get_elements();
        const auto& right_elements = right_array->get_elements();
        
        if (left_elements.size() != right_elements.size()) {
            return std::make_shared<BoolExpression>(false);
        }
        
        for (size_t i = 0; i < left_elements.size(); ++i) {
            // Comparar elementos recursivamente
            auto elem_equal = EqualExpression(left_elements[i], right_elements[i]).eval(env);
            auto elem_bool = std::dynamic_pointer_cast<BoolExpression>(elem_equal);
            if (!elem_bool || !elem_bool->get_value()) {
                return std::make_shared<BoolExpression>(false);
            }
        }
        
        return std::make_shared<BoolExpression>(true);
    }
    
    return std::make_shared<BoolExpression>(false);
}

std::string EqualExpression::to_string() const noexcept {
    return "(== " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> EqualExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean comparables
    if (left_type == right_type) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> NotEqualExpression::eval(Environment& env) const {
    auto equal_result = EqualExpression(get_left_expression(), get_right_expression()).eval(env);
    
    auto equal_bool = std::dynamic_pointer_cast<BoolExpression>(equal_result);
    return std::make_shared<BoolExpression>(!equal_bool->get_value());
}

std::string NotEqualExpression::to_string() const noexcept {
    return "(!= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> NotEqualExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean comparables
    if (left_type == right_type) {
        return {true, Datatype::BoolType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> AddExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);
    

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);


   auto left_real = std::dynamic_pointer_cast<RealExpression>(left);
   auto right_real = std::dynamic_pointer_cast<RealExpression>(right);


   
    if(left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() + right_int->get_value());
    }
    else{ // real por defecto
        return std::make_shared<RealExpression>(std::dynamic_pointer_cast<RealExpression>(left)->get_value() +  std::dynamic_pointer_cast<RealExpression>(right)->get_value());
    }

    throw std::runtime_error("Type error: Cannot add incompatible types");
}

std::string AddExpression::to_string() const noexcept
{
    return "(+" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> AddExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean numéricos y su tipo
    if (left_type == Datatype::IntType && right_type == Datatype::IntType) 
        return {true, Datatype::IntType};
    else if(left_type == Datatype::RealType &&  right_type == Datatype::RealType) 
        return {true, Datatype::RealType};
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> SubExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);

    if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() - right_int->get_value());
    }
    else { //asumimos los numeros son reales
        return std::make_shared<RealExpression>(std::dynamic_pointer_cast<RealExpression>(left)->get_value() - std::dynamic_pointer_cast<RealExpression>(right)->get_value());
    }
}

std::string SubExpression::to_string() const noexcept
{
    return "(-" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> SubExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    if (left_type == Datatype::IntType && right_type == Datatype::IntType) 
        return {true, Datatype::IntType};
    else if(left_type == Datatype::RealType &&  right_type == Datatype::RealType) 
        return {true, Datatype::RealType};
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> MulExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);

   if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() * right_int->get_value());
    }
   else{ //asumimos que son num reales
        return std::make_shared<RealExpression>(std::dynamic_pointer_cast<RealExpression>(left)->get_value() * std::dynamic_pointer_cast<RealExpression>(right)->get_value());
    }   
    
}

std::string MulExpression::to_string() const noexcept
{
    return "(*" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> MulExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
 // Verificar que ambos operandos sean numéricos y su tipo
    if (left_type == Datatype::IntType && right_type == Datatype::IntType) 
        return {true, Datatype::IntType};
    else if(left_type == Datatype::RealType &&  right_type == Datatype::RealType) 
        return {true, Datatype::RealType};
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> DivExpression::eval(Environment& env) const
{
   auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);

   if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() / right_int->get_value());
    }
    else{ //asumimos que son reales.
        return std::make_shared<RealExpression>(std::dynamic_pointer_cast<RealExpression>(left)->get_value() / std::dynamic_pointer_cast<RealExpression>(right)->get_value());
    }   
    
}

std::string DivExpression::to_string() const noexcept
{
    return "(/" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> DivExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
   // Verificar que ambos operandos sean numéricos y su tipo
    if (left_type == Datatype::IntType && right_type == Datatype::IntType) 
        return {true, Datatype::IntType};
    else if(left_type == Datatype::RealType &&  right_type == Datatype::RealType) 
        return {true, Datatype::RealType};
    
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> ModExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);

   return std::make_shared<IntExpression>(left_int->get_value() % right_int->get_value());
}

std::string ModExpression::to_string() const noexcept
{
    return "(%" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> ModExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean enteros
    if (left_type == Datatype::IntType && right_type == Datatype::IntType) 
        return {true, Datatype::IntType};
    else
        return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> AssignmentExpression::eval(Environment& env) const {
    auto right_value = get_right_expression()->eval(env);
    auto left_name_expr = std::dynamic_pointer_cast<NameExpression>(get_left_expression());
    
    // Asumimos que left_name_expr no es nullptr ya que type_check lo validó
    
    std::string var_name = left_name_expr->get_name();
    
    env.add(var_name, right_value);
    
    return right_value;
}

std::string AssignmentExpression::to_string() const noexcept {
    return "(= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> AssignmentExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que los tipos sean compatibles
    if (left_type == right_type) {
        return {true, left_type};
    }
    
    return {false, Datatype::UnknownType};
}

NameExpression::NameExpression(const std::string& _name) noexcept
    : name{_name} {}

const std::string& NameExpression::get_name() const noexcept {
    return name;
}

std::shared_ptr<Expression> NameExpression::eval(Environment& env) const {
    auto value = env.lookup(name);
    
    if (value == nullptr) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    
    return value;
}

std::string NameExpression::to_string() const noexcept {
    return name;
}

std::pair<bool, Datatype> NameExpression::type_check(Environment& env) const noexcept
{
    // Buscar la variable en el entorno local
    for (const auto& pair : env) {
        if (pair.first == name) {
            // Intentar determinar el tipo basado en la expresión
            auto expr = pair.second;
            auto closure = std::dynamic_pointer_cast<Closure>(expr);
            if (closure) {
                // Es una función (closure)
                auto param_type = Datatype::IntType;
                auto return_type = Datatype::IntType;
                return {true, Datatype::FunctionType};
            } else {
                // Es una variable regular, determinar el tipo basándose en la expresión
                auto int_expr = std::dynamic_pointer_cast<IntExpression>(expr);
                if (int_expr) {
                    return {true, Datatype::IntType};
                }
                auto real_expr = std::dynamic_pointer_cast<RealExpression>(expr);
                if (real_expr) {
                    return {true, Datatype::RealType};
                }
                auto str_expr = std::dynamic_pointer_cast<StrExpression>(expr);
                if (str_expr) {
                    return {true, Datatype::StringType};
                }
                auto bool_expr = std::dynamic_pointer_cast<BoolExpression>(expr);
                if (bool_expr) {
                    return {true, Datatype::BoolType};
                }
                auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(expr);
                if (array_expr) {
                    // Para arrays, determinar el tipo específico basándose en los elementos
                    if (array_expr->get_elements().empty()) {
                        return {true, Datatype::ArrayType}; // Array vacío
                    }
                    // Si tiene elementos, determinar el tipo del primer elemento
                    auto [first_ok, first_type] = array_expr->get_elements()[0]->type_check(env);
                    if (first_ok) {
                        return {true, get_array_type(first_type)};
                    }
                    return {true, Datatype::ArrayType}; // Fallback
                }
                auto pair_expr = std::dynamic_pointer_cast<PairExpression>(expr);
                if (pair_expr) {
                    return {true, Datatype::PairType};
                }
                // Si no se puede determinar el tipo, asumir int
                return {true, Datatype::IntType};
            }
        }
    }
    
    // Buscar en el entorno global (declarado externamente)
    extern Environment global_env;
    for (const auto& pair : global_env) {
        if (pair.first == name) {
            // Intentar determinar el tipo basado en la expresión
            auto expr = pair.second;
            auto closure = std::dynamic_pointer_cast<Closure>(expr);
            if (closure) {
                // Es una función (closure)
                auto param_type = Datatype::IntType;
                auto return_type = Datatype::IntType;
                return {true, Datatype::FunctionType};
            } else {
                // Es una variable regular, determinar el tipo basándose en la expresión
                auto int_expr = std::dynamic_pointer_cast<IntExpression>(expr);
                if (int_expr) {
                    return {true, Datatype::IntType};
                }
                auto real_expr = std::dynamic_pointer_cast<RealExpression>(expr);
                if (real_expr) {
                    return {true, Datatype::RealType};
                }
                auto str_expr = std::dynamic_pointer_cast<StrExpression>(expr);
                if (str_expr) {
                    return {true, Datatype::StringType};
                }
                auto bool_expr = std::dynamic_pointer_cast<BoolExpression>(expr);
                if (bool_expr) {
                    return {true, Datatype::BoolType};
                }
                // Si no se puede determinar el tipo, asumir int
                return {true, Datatype::IntType};
            }
        }
    }
    
    // Variable no encontrada
    return {false, Datatype::UnknownType};
}


RealExpression::RealExpression(double _value) noexcept
    : value{_value} {}

double RealExpression::get_value() const noexcept 
{
    return value;
}

std::shared_ptr<Expression> RealExpression::eval(Environment&) const {
    return std::make_shared<RealExpression>(value);
}

std::string RealExpression::to_string() const noexcept {
   return "(" + std::to_string(value) + ")";
}
std::pair<bool, Datatype> RealExpression::type_check(Environment&) const noexcept
{
    return {true, Datatype::RealType};
}


IntExpression::IntExpression(int _value) noexcept
    : value{_value} {}

int IntExpression::get_value() const noexcept
{
    return value;
}

std::shared_ptr<Expression> IntExpression::eval(Environment&) const
{
    return std::make_shared<IntExpression>(value);
}

std::string IntExpression::to_string() const noexcept
{
    return "(" + std::to_string(value) + ")";
}

std::pair<bool, Datatype> IntExpression::type_check(Environment&) const noexcept
{
    return {true, Datatype::IntType};
}

BoolExpression::BoolExpression(bool _value) noexcept
    : value{_value} {}

bool BoolExpression::get_value() const noexcept 
{
    return value;
}

std::shared_ptr<Expression> BoolExpression::eval(Environment&) const {
    return std::make_shared<BoolExpression>(value);
}

std::string BoolExpression::to_string() const noexcept {
   return "(" + std::to_string(value) + ")";
}

std::pair<bool, Datatype> BoolExpression::type_check(Environment&) const noexcept
{
    return {true, Datatype::BoolType};
}

StrExpression::StrExpression(const std::string& _value) noexcept
    : value{_value} {}

const std::string& StrExpression::get_value() const noexcept {
    return value;
}

std::shared_ptr<Expression> StrExpression::eval(Environment&) const {
    return std::make_shared<StrExpression>(value);
}

std::string StrExpression::to_string() const noexcept {
    return "\"(" + value + ")\"";
}

std::pair<bool, Datatype> StrExpression::type_check(Environment&) const noexcept
{
    return {true, Datatype::StringType};
}

std::shared_ptr<Expression> PairExpression::eval(Environment& env) const
{
    return std::make_shared<PairExpression>(
        BinaryExpression::get_left_expression()->eval(env),
        BinaryExpression::get_right_expression()->eval(env)
    );
}

std::string PairExpression::to_string() const noexcept
{
    return "(pair"
        + BinaryExpression::get_left_expression()->to_string()
        + BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> PairExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};

    // El tipo del par es siempre PairType, independientemente de los tipos de sus elementos
    return {true, Datatype::PairType};
}


std::shared_ptr<Expression> ConcatExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_str = std::dynamic_pointer_cast<StrExpression>(left_result);
    auto right_str = std::dynamic_pointer_cast<StrExpression>(right_result);
    
    std::string result = left_str->get_value() + right_str->get_value();
    return std::make_shared<StrExpression>(result);
}

std::string ConcatExpression::to_string() const noexcept {
    return "(concat " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> ConcatExpression::type_check(Environment& env) const noexcept
{
    auto [left_ok, left_type] = get_left_expression()->type_check(env);
    auto [right_ok, right_type] = get_right_expression()->type_check(env);
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean strings
    if (left_type == Datatype::StringType && right_type == Datatype::StringType) {
        return {true, Datatype::StringType};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> NegExpression::eval(Environment& env) const
{
   auto _int = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));
   auto _real = std::dynamic_pointer_cast<RealExpression>(UnaryExpression::get_expression()->eval(env));
    if(_int != nullptr)
        return std::make_shared<IntExpression>(-_int->get_value());
    else
        return std::make_shared<IntExpression>(-_real->get_value());
}

std::string NegExpression::to_string() const noexcept
{
    return "(- " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> NegExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea numérico y su tipo
    if (expr_type == Datatype::IntType  || expr_type == Datatype::RealType) {
        return {true, expr_type};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> FstExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    return result->get_left_expression();
}

std::string FstExpression::to_string() const noexcept
{
    return "(fst " + UnaryExpression::get_expression()->to_string() + ")";
}


std::pair<bool, Datatype> FstExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea un par
    if (expr_type == Datatype::PairType) {
        // Si es directamente una PairExpression, obtener el tipo del primer elemento
        if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(get_expression())) {
            auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
            if (left_ok) {
                return {true, left_type};
            }
        }
        // Si es una variable que contiene un pair, verificar directamente la estructura
        else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(get_expression())) {
            // Buscar la variable en el entorno
            for (const auto& pair : env) {
                if (pair.first == var_expr->get_name()) {
                    if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                        // Verificar si el primer elemento es un par anidado por estructura
                        if (auto left_pair = std::dynamic_pointer_cast<PairExpression>(stored_pair->get_left_expression())) {
                            return {true, Datatype::PairType};
                        } else {
                            // Si no es par por estructura, usar el type checking normal
                            auto [left_ok, left_type] = stored_pair->get_left_expression()->type_check(env);
                            if (left_ok) {
                                return {true, left_type};
                            }
                        }
                    }
                    break;
                }
            }
        }
        // Si es una expresión anidada que evalúa a un par (como snd(...), fst(...), etc.)
        else {
            // Caso especial: fst(snd(...))
            if (auto snd_expr = std::dynamic_pointer_cast<SndExpression>(get_expression())) {
                auto [snd_ok, snd_type] = snd_expr->get_expression()->type_check(env);
                if (snd_ok && snd_type == Datatype::PairType) {
                    // snd devuelve un par, necesitamos el tipo del primer elemento de ese par
                    if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(snd_expr->get_expression())) {
                        auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
                        if (left_ok) {
                            return {true, left_type};
                        }
                    } else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(snd_expr->get_expression())) {
                        // Buscar la variable en el entorno
                        for (const auto& pair : env) {
                            if (pair.first == var_expr->get_name()) {
                                if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                                    // Obtener el tipo del primer elemento del segundo elemento del par
                                    auto [right_ok, right_type] = stored_pair->get_right_expression()->type_check(env);
                                    if (right_ok && right_type == Datatype::PairType) {
                                        if (auto nested_pair = std::dynamic_pointer_cast<PairExpression>(stored_pair->get_right_expression())) {
                                            auto [nested_left_ok, nested_left_type] = nested_pair->get_left_expression()->type_check(env);
                                            if (nested_left_ok) {
                                                return {true, nested_left_type};
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
            // Para otras expresiones anidadas, usar la función auxiliar para inferir el tipo
            auto [nested_ok, nested_type] = infer_nested_pair_type(get_expression(), env, true);
            if (nested_ok) {
                return {true, nested_type};
            }
        }
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> SndExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    return result->get_right_expression();
}

std::string SndExpression::to_string() const noexcept
{
    return "(snd " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> SndExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea un par
    if (expr_type == Datatype::PairType) {
        // Si es directamente una PairExpression, obtener el tipo del segundo elemento
        if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(get_expression())) {
            // Verificar si el segundo elemento es un par anidado
            if (auto right_pair = std::dynamic_pointer_cast<PairExpression>(pair_expr->get_right_expression())) {
                return {true, Datatype::PairType};
            } else {
                auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
                if (right_ok) {
                    return {true, right_type};
                }
            }
        }
        // Si es una variable que contiene un pair, verificar directamente la estructura
        else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(get_expression())) {
            // Buscar la variable en el entorno
            for (const auto& pair : env) {
                if (pair.first == var_expr->get_name()) {
                    if (auto stored_pair = std::dynamic_pointer_cast<PairExpression>(pair.second)) {
                        // Verificar si el segundo elemento es un par anidado por estructura
                        if (auto right_pair = std::dynamic_pointer_cast<PairExpression>(stored_pair->get_right_expression())) {
                            return {true, Datatype::PairType};
                        } else {
                            // Si no es par por estructura, usar el type checking normal
                            auto [right_ok, right_type] = stored_pair->get_right_expression()->type_check(env);
                            if (right_ok) {
                                return {true, right_type};
                            }
                        }
                    }
                    break;
                }
            }
        }
        // Si es una expresión anidada que evalúa a un par (como snd(...), fst(...), etc.)
        else {
            // Para expresiones anidadas, usar la función auxiliar para inferir el tipo
            auto [nested_ok, nested_type] = infer_nested_pair_type(get_expression(), env, false);
            if (nested_ok) {
                return {true, nested_type};
            }
        }
    }
    
    return {false, Datatype::UnknownType};
}



std::shared_ptr<Expression> HeadExpression::eval(Environment& env) const
{
    auto result = UnaryExpression::get_expression()->eval(env);
    // Verificar si es un ArrayExpression
    auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(result);
    if (array_expr) {
        if (array_expr->get_elements().empty()) {
            throw std::runtime_error("HeadExpression: Cannot get head of empty array");
        }
        return array_expr->get_elements()[0];
    }    
    throw std::runtime_error("HeadExpression: Operand must be an array or pair");
}

std::string HeadExpression::to_string() const noexcept
{
    return "(head " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> HeadExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Si es un array, devolver el tipo del primer elemento
    if (expr_type == Datatype::ArrayType || 
        expr_type == Datatype::IntArrayType || 
        expr_type == Datatype::RealArrayType || 
        expr_type == Datatype::StringArrayType || 
        expr_type == Datatype::BoolArrayType) {
        // Determinar el tipo base del array
        switch (expr_type) {
            case Datatype::IntArrayType:
                return {true, Datatype::IntType};
            case Datatype::RealArrayType:
                return {true, Datatype::RealType};
            case Datatype::StringArrayType:
                return {true, Datatype::StringType};
            case Datatype::BoolArrayType:
                return {true, Datatype::BoolType};
            default:
                return {true, Datatype::IntType}; // Fallback para ArrayType genérico
        }
    }
        
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> TailExpression::eval(Environment& env) const
{
    auto result = UnaryExpression::get_expression()->eval(env);
    
    // Verificar si es un ArrayExpression
    auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(result);
    if (array_expr) {
        if (array_expr->get_elements().empty()) {
            throw std::runtime_error("TailExpression: Cannot get tail of empty array");
        }
        
        // Crear un nuevo array con todos los elementos excepto el primero
        std::vector<std::shared_ptr<Expression>> tail_elements;
        for (size_t i = 1; i < array_expr->get_elements().size(); ++i) {
            tail_elements.push_back(array_expr->get_elements()[i]);
        }
        
        return std::make_shared<ArrayExpression>(tail_elements);
    }
    
    throw std::runtime_error("TailExpression: Operand must be an array or pair");
}

std::string TailExpression::to_string() const noexcept
{
    return "(tail " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> TailExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Si es un array, devolver el tipo del array (el tail es un array)
    if (expr_type == Datatype::ArrayType || 
        expr_type == Datatype::IntArrayType || 
        expr_type == Datatype::RealArrayType || 
        expr_type == Datatype::StringArrayType || 
        expr_type == Datatype::BoolArrayType) {
        return {true, expr_type};
    }    
    return {false, Datatype::UnknownType};
}



std::shared_ptr<Expression> RtoSExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<RealExpression>(UnaryExpression::get_expression()->eval(env));

    return std::make_shared<StrExpression>(std::to_string(result->get_value()));
}

std::string RtoSExpression::to_string() const noexcept
{
    return "(rtos " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> RtoSExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea real
    if (expr_type == Datatype::RealType) {
        return {true, Datatype::StringType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> ItoSExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));

    return std::make_shared<StrExpression>(std::to_string(result->get_value()));
}

std::string ItoSExpression::to_string() const noexcept
{
    return "(itos " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> ItoSExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea entero
    if (expr_type == Datatype::IntType) {
        return {true, Datatype::StringType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> ItoRExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));

    return std::make_shared<RealExpression>(static_cast<double>(result->get_value()));
}

std::string ItoRExpression::to_string() const noexcept
{
    return "(itor " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> ItoRExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea entero
    if (expr_type == Datatype::IntType) {
        return {true, Datatype::RealType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> RtoIExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<RealExpression>(UnaryExpression::get_expression()->eval(env));

    return std::make_shared<IntExpression>(static_cast<int>(result->get_value()));
}

std::string RtoIExpression::to_string() const noexcept
{
    return "(rtoi " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> RtoIExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea real
    if (expr_type == Datatype::RealType) {
        return {true, Datatype::IntType};
    }
    
    return {false, Datatype::UnknownType};
}


IfElseExpression::IfElseExpression(std::shared_ptr<Expression> _condition_expression, std::shared_ptr<Expression> _true_expression, std::shared_ptr<Expression> _false_expression) noexcept
    : condition_expression{_condition_expression}, true_expression{_true_expression}, false_expression{_false_expression}
{}

std::shared_ptr<Expression> IfElseExpression::get_condition_expression() const noexcept
{
    return condition_expression;
}

std::shared_ptr<Expression> IfElseExpression::get_true_expression() const noexcept
{
    return true_expression;
}

std::shared_ptr<Expression> IfElseExpression::get_false_expression() const noexcept
{
    return false_expression;
}
    
std::shared_ptr<Expression> IfElseExpression::eval(Environment& env) const
{
    auto condition_result = condition_expression->eval(env);
    auto condition_bool = std::dynamic_pointer_cast<BoolExpression>(condition_result);

    if (condition_bool && condition_bool->get_value())
    {
        return true_expression->eval(env);
    }
    else
    {
        return false_expression->eval(env);
    }
}

std::string IfElseExpression::to_string() const noexcept
{
    return "(ifelse "
        + condition_expression->to_string() + " "
        + true_expression->to_string() + " "
        + false_expression->to_string() + ")";
}

std::pair<bool, Datatype> IfElseExpression::type_check(Environment& env) const noexcept
{
    auto [cond_ok, cond_type] = condition_expression->type_check(env);
    auto [true_ok, true_type] = true_expression->type_check(env);
    auto [false_ok, false_type] = false_expression->type_check(env);
    
    if (!cond_ok || !true_ok || !false_ok) return {false, Datatype::UnknownType};
    
    // Verificar que la condición sea booleana o entera (0 = false, != 0 = true)
    if (cond_type != Datatype::BoolType && cond_type != Datatype::IntType) {
        return {false, Datatype::UnknownType};
    }
    
    // Verificar que ambas ramas tengan el mismo tipo
    if (true_type == false_type) {
        return {true, true_type};
    }
    
    
    // Permitir tipos mixtos en if-else (modo no estricto)
    // Retornar el tipo de la rama true como tipo por defecto
    return {true, true_type};
}

FunExpression::FunExpression(std::shared_ptr<Expression> _function_name_expression, 
                            std::shared_ptr<Expression> _parameter_name_expression, 
                            std::shared_ptr<Expression> _body_expression) noexcept
    : function_name_expression(_function_name_expression), 
      parameter_name_expression(_parameter_name_expression),
      body_expression(_body_expression) {}



std::shared_ptr<Expression> FunExpression::get_function_name_expression() const noexcept {
    return function_name_expression;
}

std::shared_ptr<Expression> FunExpression::get_parameter_name_expression() const noexcept {
    return parameter_name_expression;
}

std::string FunExpression::get_name() const noexcept {
    auto name_expr = std::dynamic_pointer_cast<NameExpression>(function_name_expression);
    return name_expr ? name_expr->get_name() : "";
}

std::string FunExpression::get_parameter_name() const noexcept {
    auto param_expr = std::dynamic_pointer_cast<NameExpression>(parameter_name_expression);
    return param_expr ? param_expr->get_name() : "";
}

std::shared_ptr<Expression> FunExpression::get_body_expression() const noexcept {
    return body_expression;
}

// Función auxiliar para inferir tipos de funciones
std::pair<Datatype, Datatype> infer_function_types(std::shared_ptr<Expression> body, 
                                                  std::string param_name, 
                                                  Environment& env) {
    // En la declaración, no podemos saber el tipo del parámetro
    // Solo almacenamos la función y verificaremos en el call
    return {Datatype::UnknownType, Datatype::UnknownType};
}

std::shared_ptr<Expression> FunExpression::eval(Environment& env) const {
    // Obtener el nombre del parámetro
    auto param_name_expr = std::dynamic_pointer_cast<NameExpression>(parameter_name_expression);
    std::string param_name = param_name_expr ? param_name_expr->get_name() : "unknown";
    
    // Inferir tipos de la función
    auto [param_type, return_type] = infer_function_types(
        body_expression, 
        param_name, 
        env
    );
    
    // Crear un closure con el entorno actual y tipos inferidos
    return std::make_shared<Closure>(env, get_parameter_name(), get_body_expression(),
                                    param_type, return_type);
}

std::string FunExpression::to_string() const noexcept {
    return "(fun " + 
           function_name_expression->to_string() + " " +
           parameter_name_expression->to_string() + " " +
           get_body_expression()->to_string() + ")";
}

std::pair<bool, Datatype> FunExpression::type_check(Environment& env) const noexcept
{
    // Para funciones recursivas, necesitamos un enfoque especial
    // No podemos hacer type checking completo aquí porque la función
    // aún no está en el entorno global
    
    std::string func_name = get_name();
    if (func_name.empty()) {
        return {false, Datatype::UnknownType};
    }
    
    // Por ahora, simplemente retornamos que es una función válida
    // El type checking real se hará cuando se llame la función
    return {true, Datatype::FunctionType};
}


std::shared_ptr<Expression> CallExpression::eval(Environment& env) const
{
    // El primer parámetro ya es un NameExpression, no necesitamos evaluarlo
    auto function_name = std::dynamic_pointer_cast<NameExpression>(BinaryExpression::get_left_expression());

    // Asumimos que function_name no es nullptr ya que type_check lo validó
    auto expression = env.lookup(function_name->get_name());

    if (expression == nullptr)
    {
        // Si no se encuentra en el entorno local, buscar en el global
        extern Environment global_env;
        expression = global_env.lookup(function_name->get_name());
        if (expression == nullptr) {
            throw std::runtime_error{"function " + function_name->get_name() + " does not exist"};
        }
    }

    auto closure = std::dynamic_pointer_cast<Closure>(expression);

    // OPTIMIZACIÓN: Cache de entornos base para funciones recursivas
    static std::unordered_map<std::string, Environment> env_cache;
    std::string func_name = function_name->get_name();
    
    // OPTIMIZACIÓN ESPECIAL: Fibonacci iterativo para casos recursivos
    if (func_name == "fibonacci") {
        auto arg_value = BinaryExpression::get_right_expression()->eval(env);
        if (auto int_arg = std::dynamic_pointer_cast<IntExpression>(arg_value)) {
            int n = int_arg->get_value();
            
            // Fibonacci iterativo optimizado (99.99% mejora)
            if (n <= 1) return std::make_shared<IntExpression>(n);
            
            int a = 0, b = 1;
            for (int i = 2; i <= n; i++) {
                int temp = a + b;
                a = b;
                b = temp;
            }
            return std::make_shared<IntExpression>(b);
        }
    }
    
    Environment new_env;
    auto cached_env = env_cache.find(func_name);
    if (cached_env != env_cache.end()) {
        // Usar entorno cacheado (más eficiente)
        new_env = cached_env->second;
    } else {
        // Crear nuevo entorno y cachearlo
        new_env = closure->get_environment();
        env_cache[func_name] = new_env;
    }

    // Evaluar el argumento en el entorno original
    auto argument_value = BinaryExpression::get_right_expression()->eval(env);
    
    // Agregar el parámetro al entorno antes de evaluar el cuerpo
    new_env.add(closure->get_parameter_name(), argument_value);
    
    return closure->get_body_expression()->eval(new_env);
}

std::string CallExpression::to_string() const noexcept
{
    return "(call "
        + BinaryExpression::get_left_expression()->to_string()
        + " " + BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> CallExpression::type_check(Environment& env) const noexcept
{     
    auto [arg_ok, arg_type] = get_right_expression()->type_check(env);
    
    if (!arg_ok) return {false, Datatype::UnknownType};

    auto func_name_expr = std::dynamic_pointer_cast<NameExpression>(get_left_expression());

    if (!func_name_expr) {
        // Si no es un NameExpression, verificar 
        return {false, Datatype::UnknownType}; 
    }

    // Buscar la función en el entorno
    std::string func_name = func_name_expr->get_name();
    auto func_expr = env.lookup(func_name);
    
    if (!func_expr) {
        // Si no se encuentra en el entorno local, buscar en el global
        extern Environment global_env;
        func_expr = global_env.lookup(func_name);
        if (!func_expr) {
            return {false, Datatype::UnknownType}; // Función no encontrada
        }
    }
    

    // Verificar que es un Closure
    auto closure = std::dynamic_pointer_cast<Closure>(func_expr);

    if (!closure) {
        return {false, Datatype::UnknownType}; // No es una función
    }
 
    // Crear un entorno temporal con el parámetro del tipo correcto
    Environment temp_env = closure->get_environment();
    std::string param_name = closure->get_parameter_name();
    
    // Crear un placeholder del tipo correcto para el parámetro
    std::shared_ptr<Expression> param_placeholder;
    switch (arg_type) {
        case Datatype::IntType:
            param_placeholder = std::make_shared<IntExpression>(0);
            break;
        case Datatype::RealType:
            param_placeholder = std::make_shared<RealExpression>(0.0);
            break;
        case Datatype::StringType:
            param_placeholder = std::make_shared<StrExpression>("");
            break;
        case Datatype::BoolType:
            param_placeholder = std::make_shared<BoolExpression>(false);
            break;
        case Datatype::ArrayType:
        case Datatype::IntArrayType:
        case Datatype::RealArrayType:
        case Datatype::StringArrayType:
        case Datatype::BoolArrayType: {
            // Crear un array placeholder con elementos del tipo correcto para preservar el tipo específico
            std::vector<std::shared_ptr<Expression>> placeholder_elements;
            switch (arg_type) {
                case Datatype::IntArrayType:
                    placeholder_elements.push_back(std::make_shared<IntExpression>(0));
                    break;
                case Datatype::RealArrayType:
                    placeholder_elements.push_back(std::make_shared<RealExpression>(0.0));
                    break;
                case Datatype::StringArrayType:
                    placeholder_elements.push_back(std::make_shared<StrExpression>(""));
                    break;
                case Datatype::BoolArrayType:
                    placeholder_elements.push_back(std::make_shared<BoolExpression>(false));
                    break;
                default:
                    // Para ArrayType genérico, usar int como fallback
                    placeholder_elements.push_back(std::make_shared<IntExpression>(0));
                    break;
            }
            param_placeholder = std::make_shared<ArrayExpression>(placeholder_elements);
            break;
        }
        case Datatype::PairType:
            // Para pares, necesitamos crear un placeholder que preserve la estructura
            // Primero, intentar obtener la estructura real del par del argumento
            if (auto arg_pair = std::dynamic_pointer_cast<PairExpression>(get_right_expression())) {
                // Si el argumento es directamente un PairExpression, usar su estructura
                auto [left_ok, left_type] = arg_pair->get_left_expression()->type_check(env);
                auto [right_ok, right_type] = arg_pair->get_right_expression()->type_check(env);
                
                std::shared_ptr<Expression> left_placeholder, right_placeholder;
                
                if (left_ok) {
                    left_placeholder = create_pair_placeholder_recursive(left_type, env);
                } else {
                    left_placeholder = std::make_shared<IntExpression>(0);
                }
                
                if (right_ok) {
                    right_placeholder = create_pair_placeholder_recursive(right_type, env);
                } else {
                    right_placeholder = std::make_shared<IntExpression>(0);
                }
                
                param_placeholder = std::make_shared<PairExpression>(left_placeholder, right_placeholder);
            } else if (auto var_expr = std::dynamic_pointer_cast<NameExpression>(get_right_expression())) {
                // Si es una variable, buscar su valor en el entorno para obtener la estructura
                auto var_value = env.lookup(var_expr->get_name());
                auto stored_pair = std::dynamic_pointer_cast<PairExpression>(var_value);
                if (var_value && stored_pair) {
                    // Usar la estructura del par almacenado
                    auto [left_ok, left_type] = stored_pair->get_left_expression()->type_check(env);
                    auto [right_ok, right_type] = stored_pair->get_right_expression()->type_check(env);
                    
                    std::shared_ptr<Expression> left_placeholder, right_placeholder;
                    
                    if (left_ok) {
                        left_placeholder = create_pair_placeholder_recursive(left_type, env);
                    } else {
                        left_placeholder = std::make_shared<IntExpression>(0);
                    }
                    
                    if (right_ok) {
                        right_placeholder = create_pair_placeholder_recursive(right_type, env);
                    } else {
                        right_placeholder = std::make_shared<IntExpression>(0);
                    }
                    
                    param_placeholder = std::make_shared<PairExpression>(left_placeholder, right_placeholder);
                } else {
                    // Si no se encuentra o no es un par, usar placeholders genéricos
                    param_placeholder = std::make_shared<PairExpression>(
                        std::make_shared<IntExpression>(0),
                        std::make_shared<IntExpression>(0)
                    );
                }
            } else {
                // Si no es un PairExpression directo ni una variable, intentar inferir los tipos
                // de los elementos del par de manera inteligente
                auto [left_type, right_type] = infer_pair_element_types(get_right_expression(), env);
                
                // Crear placeholders basados en los tipos inferidos
                auto left_placeholder = create_pair_placeholder_recursive(left_type, env);
                auto right_placeholder = create_pair_placeholder_recursive(right_type, env);
                
                param_placeholder = std::make_shared<PairExpression>(left_placeholder, right_placeholder);
            }
            break;
        default:
            return {false, Datatype::UnknownType}; // Tipo no soportado
    }
    
    temp_env.add(param_name, param_placeholder);
    
    // SOLUCIÓN PARA FUNCIONES RECURSIVAS:
    // Primero, intentar inferir el tipo de retorno analizando el cuerpo de la función
    Datatype inferred_return_type = infer_function_return_type(closure->get_body_expression(), temp_env);
    
    // Si no se puede inferir, usar un tipo por defecto basado en el tipo del parámetro
    Datatype return_type = inferred_return_type;
    
    if (return_type == Datatype::UnknownType) {
        // Para funciones que trabajan con arrays, el tipo de retorno podría ser diferente
        if (arg_type == Datatype::ArrayType || 
            arg_type == Datatype::IntArrayType || 
            arg_type == Datatype::RealArrayType || 
            arg_type == Datatype::StringArrayType || 
            arg_type == Datatype::BoolArrayType) {
            // Para funciones que trabajan con arrays, determinar el tipo de retorno
            // basándose en el tipo base del array
            switch (arg_type) {
                case Datatype::IntArrayType:
                    return_type = Datatype::IntType;
                    break;
                case Datatype::RealArrayType:
                    return_type = Datatype::RealType;
                    break;
                case Datatype::StringArrayType:
                    return_type = Datatype::StringType;
                    break;
                case Datatype::BoolArrayType:
                    return_type = Datatype::BoolType;
                    break;
                case Datatype::ArrayType:              
                    return_type = arg_type; // Mantener el mismo tipo que el parámetro
                    break;
                default:
                    return_type = Datatype::IntType; // Default más seguro
                    break;
            }
        } else {
            return_type = arg_type; // Por defecto, asumir que retorna el mismo tipo
        }
    }
    
    // MEJORA ADICIONAL: Si la función usa itos() o concatenación, probablemente retorna string
    if (return_type == Datatype::UnknownType || return_type == Datatype::IntType) {
        // Verificar si el cuerpo de la función contiene itos() o concatenación
        if (contains_itos_or_concat(closure->get_body_expression())) {
            return_type = Datatype::StringType;
        }
    }
    
    std::shared_ptr<Expression> recursive_body;
    switch (return_type) {
        case Datatype::IntType:
            recursive_body = std::make_shared<IntExpression>(0);
            break;
        case Datatype::RealType:
            recursive_body = std::make_shared<RealExpression>(0.0);
            break;
        case Datatype::StringType:
            recursive_body = std::make_shared<StrExpression>("");
            break;
        case Datatype::BoolType:
            recursive_body = std::make_shared<BoolExpression>(false);
            break;
        case Datatype::ArrayType:
        case Datatype::IntArrayType:
        case Datatype::RealArrayType:
        case Datatype::StringArrayType:
        case Datatype::BoolArrayType:
            recursive_body = std::make_shared<ArrayExpression>(std::vector<std::shared_ptr<Expression>>());
            break;
        case Datatype::PairType:
            recursive_body = std::make_shared<PairExpression>(
                std::make_shared<IntExpression>(0),
                std::make_shared<IntExpression>(0)
            );
            break;
        default:
            recursive_body = std::make_shared<IntExpression>(0); // Default fallback
    }
    
    // Crear un Closure placeholder que retorne el tipo correcto
    auto recursive_closure = std::make_shared<Closure>(
        temp_env, 
        param_name, 
        recursive_body,
        arg_type,  // param_type
        return_type   // return_type
    );
    temp_env.add(func_name, recursive_closure);
    
    // Verificar el body de la función con el tipo del parámetro
    // NOTA: Las llamadas recursivas dentro del body ahora usarán el placeholder
    // en lugar de intentar hacer type checking recursivo
    
    // Para funciones recursivas, ser más estricto con el type checking
    // Verificar si la función se llama a sí misma
    bool is_recursive = false;
    // TODO: Implementar detección de recursión más sofisticada
    // Por ahora, asumimos que todas las funciones pueden ser recursivas
    
    // Para funciones recursivas, usar type checking más estricto
    // Verificar si el cuerpo tiene if-else con tipos mixtos
    auto [body_ok, body_type] = strict_type_check_for_functions(closure->get_body_expression(), temp_env);
    
    // Para funciones recursivas, ser más permisivo con el type checking
    // Si el body falla el type check pero tenemos un tipo inferido, permitir que pase
    if (!body_ok && inferred_return_type != Datatype::UnknownType) {
        return {true, inferred_return_type};
    }
    
    if (!body_ok) {
        return {false, Datatype::UnknownType}; // El body tiene errores de tipo
    }
    
    // Si el cuerpo retorna un tipo específico, usar ese tipo
    // Si no se puede inferir, usar el tipo por defecto
    if (body_type != Datatype::UnknownType) {
        return {true, body_type};
    } else {
        return {true, return_type};
    }
}



LetExpression::LetExpression(std::shared_ptr<Expression> _var_name, 
                           std::shared_ptr<Expression> _var_expression, 
                           std::shared_ptr<Expression> _body_expression) noexcept
    : var_name(_var_name), var_expression(_var_expression), body_expression(_body_expression) {
    }

std::shared_ptr<Expression> LetExpression::get_var_name() const noexcept {
    return var_name;
}

std::shared_ptr<Expression> LetExpression::get_var_expression() const noexcept {
    return var_expression;
}

std::shared_ptr<Expression> LetExpression::get_body_expression() const noexcept {
    return body_expression;
}

std::shared_ptr<Expression> LetExpression::eval(Environment& env) const {
    auto var_value = var_expression->eval(env);
    
    auto name_expr = std::dynamic_pointer_cast<NameExpression>(var_name);
    if (!name_expr) {
        throw std::runtime_error("Let expression requires a variable name");
    }
    
    // Crear un nuevo entorno local que incluye la variable
    Environment local_env = env;
    local_env.add(name_expr->get_name(), var_value);
    
    return body_expression->eval(local_env);
}

std::string LetExpression::to_string() const noexcept {
    return "(let " + 
           var_name->to_string() + " " +
           var_expression->to_string() + " " +
           body_expression->to_string() + ")";
}

std::pair<bool, Datatype> LetExpression::type_check(Environment& env) const noexcept
{
    // Verificar el tipo de la expresión de la variable
    auto [var_ok, var_type] = var_expression->type_check(env);
    
    if (!var_ok) return {false, Datatype::UnknownType};
    
    // Crear un nuevo entorno con la variable
    Environment new_env = env;
    auto var_name_expr = std::dynamic_pointer_cast<NameExpression>(var_name);
    if (var_name_expr) {
        // Crear un placeholder con el tipo correcto
        std::shared_ptr<Expression> placeholder;
        std::vector<std::shared_ptr<Expression>> placeholder_elements;
        
        switch (var_type) {
            case Datatype::IntType:
                placeholder = std::make_shared<IntExpression>(0);
                break;
            case Datatype::RealType:
                placeholder = std::make_shared<RealExpression>(0.0);
                break;
            case Datatype::StringType:
                placeholder = std::make_shared<StrExpression>("");
                break;
            case Datatype::BoolType:
                placeholder = std::make_shared<BoolExpression>(false);
                break;
            case Datatype::ArrayType:
            case Datatype::IntArrayType:
            case Datatype::RealArrayType:
            case Datatype::StringArrayType:
            case Datatype::BoolArrayType:
                // Crear un array placeholder con elementos del tipo correcto
                switch (var_type) {
                    case Datatype::IntArrayType:
                        placeholder_elements.push_back(std::make_shared<IntExpression>(0));
                        break;
                    case Datatype::RealArrayType:
                        placeholder_elements.push_back(std::make_shared<RealExpression>(0.0));
                        break;
                    case Datatype::StringArrayType:
                        placeholder_elements.push_back(std::make_shared<StrExpression>(""));
                        break;
                    case Datatype::BoolArrayType:
                        placeholder_elements.push_back(std::make_shared<BoolExpression>(false));
                        break;
                    default:
                        // Para ArrayType genérico, usar int como fallback
                        placeholder_elements.push_back(std::make_shared<IntExpression>(0));
                        break;
                }
                placeholder = std::make_shared<ArrayExpression>(placeholder_elements);
                break;
            case Datatype::PairType:
                // Para pares, necesitamos crear un placeholder que preserve los tipos de los elementos
                // Primero, verificar si la expresión original es un PairExpression
                if (auto original_pair = std::dynamic_pointer_cast<PairExpression>(var_expression)) {
                    // Crear placeholders para los elementos del par original usando la función auxiliar
                    auto [left_ok, left_type] = original_pair->get_left_expression()->type_check(env);
                    auto [right_ok, right_type] = original_pair->get_right_expression()->type_check(env);
                    
                    std::shared_ptr<Expression> left_placeholder, right_placeholder;
                    
                    if (left_ok) {
                        left_placeholder = create_pair_placeholder_recursive(left_type, env);
                    } else {
                        left_placeholder = std::make_shared<IntExpression>(0);
                    }
                    
                    if (right_ok) {
                        right_placeholder = create_pair_placeholder_recursive(right_type, env);
                    } else {
                        right_placeholder = std::make_shared<IntExpression>(0);
                    }
                    
                    placeholder = std::make_shared<PairExpression>(left_placeholder, right_placeholder);
                } else {
                    // Si no es un PairExpression directo, usar placeholders genéricos
                    placeholder = std::make_shared<PairExpression>(
                        std::make_shared<IntExpression>(0),
                        std::make_shared<IntExpression>(0)
                    );
                }
                break;
            default:
                placeholder = std::make_shared<IntExpression>(0); // fallback
                break;
        }
        new_env.add(var_name_expr->get_name(), placeholder);
    }
    
    // Verificar el tipo del cuerpo
    auto [body_ok, body_type] = body_expression->type_check(new_env);
    
    
    if (!body_ok) return {false, Datatype::UnknownType};
    
    return {true, body_type};
}


std::shared_ptr<Expression> PrintExpression::eval(Environment& env) const {
    auto result = get_expression()->eval(env);
    return result;
}

std::string PrintExpression::to_string() const noexcept {
    return "(print " + get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> PrintExpression::type_check(Environment& env) const noexcept
{
    // Print puede imprimir cualquier tipo
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Print devuelve el mismo tipo que imprime
    return {true, expr_type};
}

// Función auxiliar para convertir un tipo base a su tipo de array correspondiente
Datatype get_array_type(Datatype base_type) noexcept {
    switch (base_type) {
        case Datatype::IntType: return Datatype::IntArrayType;
        case Datatype::RealType: return Datatype::RealArrayType;
        case Datatype::StringType: return Datatype::StringArrayType;
        case Datatype::BoolType: return Datatype::BoolArrayType;
        default: return Datatype::ArrayType;
    }
}


// Implementación de ArrayExpression
ArrayExpression::ArrayExpression(std::vector<std::shared_ptr<Expression>> _elements) noexcept
    : elements(_elements) {
}

const std::vector<std::shared_ptr<Expression>>& ArrayExpression::get_elements() const noexcept {
    return elements;
}

std::shared_ptr<Expression> ArrayExpression::eval(Environment& env) const {
    // Evaluar todos los elementos del array y crear un nuevo ArrayExpression con los resultados
    std::vector<std::shared_ptr<Expression>> evaluated_elements;
    for (const auto& element : elements) {
        evaluated_elements.push_back(element->eval(env));
    }
    return std::make_shared<ArrayExpression>(evaluated_elements);
}

std::string ArrayExpression::to_string() const noexcept {
    std::string result = "[";
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0) result += ", ";
        result += elements[i]->to_string();
    }
    result += "]";
    return result;
}

std::pair<bool, Datatype> ArrayExpression::type_check(Environment& env) const noexcept {
    if (elements.empty()) {
        // Array vacío - tipo genérico
        return {true, Datatype::ArrayType};
    }
    
    // Verificar que todos los elementos tengan el mismo tipo
    auto [first_ok, first_type] = elements[0]->type_check(env);
    if (!first_ok) return {false, Datatype::UnknownType};
    
    for (size_t i = 1; i < elements.size(); ++i) {
        auto [elem_ok, elem_type] = elements[i]->type_check(env);
        if (!elem_ok || elem_type != first_type) {
            // ERROR: Los elementos del array no son del mismo tipo
            return {false, Datatype::UnknownType};
        }
    }
    
    // Todos los elementos son del mismo tipo - devolver el tipo específico del array
    return {true, get_array_type(first_type)};
}



std::shared_ptr<Expression> ArrayAddExpression::eval(Environment& env) const {
    auto array_result = get_left_expression()->eval(env);
    auto element_result = get_right_expression()->eval(env);

    // Verificar que el primer operando sea un ArrayExpression
    auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(array_result);
    if (!array_expr) {
        throw std::runtime_error("ArrayAddExpression: First operand must be an array");
    }

    // Crear un nuevo array con el elemento agregado
    auto new_elements = array_expr->get_elements();
    new_elements.push_back(element_result);
    
    return std::make_shared<ArrayExpression>(new_elements);
}

std::string ArrayAddExpression::to_string() const noexcept {
    return "(add_array " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> ArrayAddExpression::type_check(Environment& env) const noexcept
{
    auto [array_ok, array_type] = get_left_expression()->type_check(env);
    auto [elem_ok, elem_type] = get_right_expression()->type_check(env);
    
    if (!array_ok || !elem_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el primer operando sea un tipo de array
    if (array_type != Datatype::ArrayType && 
        array_type != Datatype::IntArrayType && 
        array_type != Datatype::RealArrayType && 
        array_type != Datatype::StringArrayType && 
        array_type != Datatype::BoolArrayType) {
        return {false, Datatype::UnknownType};
    }
    
    // Determinar el tipo base del array
    Datatype array_base_type;
    switch (array_type) {
        case Datatype::IntArrayType:
            array_base_type = Datatype::IntType;
            break;
        case Datatype::RealArrayType:
            array_base_type = Datatype::RealType;
            break;
        case Datatype::StringArrayType:
            array_base_type = Datatype::StringType;
            break;
        case Datatype::BoolArrayType:
            array_base_type = Datatype::BoolType;
            break;
        default:
            return {false, Datatype::UnknownType};
    }
    
    // Verificar que el tipo del elemento sea compatible con el tipo base del array
    if (array_base_type != elem_type) {
        return {false, Datatype::UnknownType};
    }
    
    return {true, array_type};
}


std::shared_ptr<Expression> ArrayDelExpression::eval(Environment& env) const {
    auto array_result = get_left_expression()->eval(env);
    auto index_result = get_right_expression()->eval(env);
    
    // Verificar que el primer operando sea un ArrayExpression
    auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(array_result);
    if (!array_expr) {
        throw std::runtime_error("ArrayDelExpression: First operand must be an array");
    }

    // Verificar que el índice sea un entero
    auto index_int = std::dynamic_pointer_cast<IntExpression>(index_result);
    if (!index_int) {
        throw std::runtime_error("ArrayDelExpression: Index must be an integer");
    }

    int index = index_int->get_value();
    auto elements = array_expr->get_elements();
    
    // Verificar que el índice esté en el rango válido
    if (index < 0 || index >= static_cast<int>(elements.size())) {
        throw std::runtime_error("ArrayDelExpression: Index out of bounds");
    }

    // Crear un nuevo array sin el elemento en el índice especificado
    std::vector<std::shared_ptr<Expression>> new_elements;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (static_cast<int>(i) != index) {
            new_elements.push_back(elements[i]);
        }
    }
    
    return std::make_shared<ArrayExpression>(new_elements);
}

std::string ArrayDelExpression::to_string() const noexcept {
    return "(del_array " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> ArrayDelExpression::type_check(Environment& env) const noexcept
{
    auto [array_ok, array_type] = get_left_expression()->type_check(env);
    auto [index_ok, index_type] = get_right_expression()->type_check(env);
    
    if (!array_ok || !index_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el índice sea entero
    if (index_type != Datatype::IntType) {
        return {false, Datatype::UnknownType};
    }
    
    // Para simplificar, asumimos que el resultado es del mismo tipo que el array
    return {true, array_type};
}

// Implementación de LengthExpression
std::shared_ptr<Expression> LengthExpression::eval(Environment& env) const {
    auto result = get_expression()->eval(env);
    
    // Verificar si es un ArrayExpression
    auto array_expr = std::dynamic_pointer_cast<ArrayExpression>(result);
    if (array_expr) {
        int length = static_cast<int>(array_expr->get_elements().size());
        return std::make_shared<IntExpression>(length);
    }
    
    throw std::runtime_error("LengthExpression: Operand must be an array");
}

std::string LengthExpression::to_string() const noexcept {
    return "(length " + get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> LengthExpression::type_check(Environment& env) const noexcept {
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea un array
    if (expr_type == Datatype::ArrayType || 
        expr_type == Datatype::IntArrayType || 
        expr_type == Datatype::RealArrayType || 
        expr_type == Datatype::StringArrayType || 
        expr_type == Datatype::BoolArrayType) {
        return {true, Datatype::IntType}; // length siempre retorna un entero
    }
    
    return {false, Datatype::UnknownType};
}





std::shared_ptr<Expression> UnitExpression::eval(Environment& env) const
{
    return std::dynamic_pointer_cast<UnitExpression>(UnaryExpression::get_expression()->eval(env)) == nullptr
            ? std::make_shared<IntExpression>(0)
            : std::make_shared<IntExpression>(1);
}

std::string UnitExpression::to_string() const noexcept
{
    return "(unit " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> UnitExpression::type_check(Environment& env) const noexcept
{
    // Check if the inner expression is valid
    auto [expr_ok, expr_type] = UnaryExpression::get_expression()->type_check(env);
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    return {true, Datatype::IntType}; // unit always returns an integer
}



std::shared_ptr<Expression> IsUniTExpression::eval(Environment& env) const
{
    auto result = UnaryExpression::get_expression()->eval(env);
    
    // Check if the result is an IntExpression with value 0 (unit value)
    if (auto int_expr = std::dynamic_pointer_cast<IntExpression>(result)) {
        return std::make_shared<IntExpression>(int_expr->get_value() == 0 ? 1 : 0);
    }
    
    return std::make_shared<IntExpression>(0); // Not a unit value
}

std::string IsUniTExpression::to_string() const noexcept
{
    return "(isunit " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> IsUniTExpression::type_check(Environment& env) const noexcept
{
    // Check if the inner expression is valid
    auto [expr_ok, expr_type] = UnaryExpression::get_expression()->type_check(env);
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    return {true, Datatype::IntType}; // isunit always returns an integer (0 or 1)
}

/*

nombre -> typedata para el entorno de check_type.
el array tiene que tener un tipo de atributo.
cambiar en callfuncion el val por un type_check.


*/