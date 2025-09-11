#include "utils.hpp"
#include "expression.hpp"
#include "Exceptions.hpp"
#include <vector>





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
    if (is_compatible_with(expr_type, Datatype::BoolType)) {
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
    if (is_compatible_with(left_type, Datatype::BoolType) && is_compatible_with(right_type, Datatype::BoolType)) {
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
    if (is_compatible_with(left_type, Datatype::BoolType) && is_compatible_with(right_type, Datatype::BoolType)) {
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
    if (is_compatible_with(left_type, Datatype::BoolType) && is_compatible_with(right_type, Datatype::BoolType)) {
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
    else if (left_real && right_real) {
        return std::make_shared<BoolExpression>(left_real->get_value() < right_real->get_value());
    }
    else if (left_int && right_real) {
        return std::make_shared<BoolExpression>(left_int->get_value() < right_real->get_value());
    }
    else if (left_real && right_int) {
        return std::make_shared<BoolExpression>(left_real->get_value() < right_int->get_value());
    }
    
    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<BoolExpression>(left_int->get_value() < right_int->get_value());
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
    if (is_compatible_with(left_type, right_type)) {
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
    else if (left_real && right_real) {
        return std::make_shared<BoolExpression>(left_real->get_value() <= right_real->get_value());
    }
    else if (left_int && right_real) {
        return std::make_shared<BoolExpression>(left_int->get_value() <= right_real->get_value());
    }
    else if (left_real && right_int) {
        return std::make_shared<BoolExpression>(left_real->get_value() <= right_int->get_value());
    }
    
    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<BoolExpression>(left_int->get_value() <= right_int->get_value());
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
    if (is_compatible_with(left_type, right_type)) {
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
        return std::make_shared<BoolExpression>(left_int->get_value() > right_int->get_value());
    }
    else if (left_real && right_real) {
        return std::make_shared<BoolExpression>(left_real->get_value() > right_real->get_value());
    }
    else if (left_int && right_real) {
        return std::make_shared<BoolExpression>(left_int->get_value() > right_real->get_value());
    }
    else if (left_real && right_int) {
        return std::make_shared<BoolExpression>(left_real->get_value() > right_int->get_value());
    }
    
    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<BoolExpression>(left_int->get_value() > right_int->get_value());
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
    if (is_compatible_with(left_type, right_type)) {
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
    else if (left_real && right_real) {
        return std::make_shared<BoolExpression>(left_real->get_value() >= right_real->get_value());
    }
    else if (left_int && right_real) {
        return std::make_shared<BoolExpression>(left_int->get_value() >= right_real->get_value());
    }
    else if (left_real && right_int) {
        return std::make_shared<BoolExpression>(left_real->get_value() >= right_int->get_value());
    }
    
    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<BoolExpression>(left_int->get_value() >= right_int->get_value());
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
    if (is_compatible_with(left_type, right_type)) {
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
    
    if (left_int && right_int) {
        return std::make_shared<BoolExpression>(left_int->get_value() == right_int->get_value());
    }
    else if (left_real && right_real) {
        return std::make_shared<BoolExpression>(left_real->get_value() == right_real->get_value());
    }
    else if (left_int && right_real) {
        return std::make_shared<BoolExpression>(left_int->get_value() == right_real->get_value());
    }
    else if (left_real && right_int) {
        return std::make_shared<BoolExpression>(left_real->get_value() == right_int->get_value());
    }
    else if (left_bool && right_bool) {
        return std::make_shared<BoolExpression>(left_bool->get_value() == right_bool->get_value());
    }
    else if (left_str && right_str) {
        return std::make_shared<BoolExpression>(left_str->get_value() == right_str->get_value());
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
    if (is_compatible_with(left_type, right_type)) {
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
    if (is_compatible_with(left_type, right_type)) {
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

   if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() + right_int->get_value());
    }

    if (left_real && right_real) {
        return std::make_shared<RealExpression>(left_real->get_value() + right_real->get_value());
    }

    // Si llegamos aquí, los tipos no son compatibles para la suma
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
    
    // Verificar que ambos operandos sean numéricos
    if ((left_type == Datatype::IntType || left_type == Datatype::RealType) &&
        (right_type == Datatype::IntType || right_type == Datatype::RealType)) {
        // Si ambos son enteros, el resultado es entero
        if (left_type == Datatype::IntType && right_type == Datatype::IntType) {
            return {true, Datatype::IntType};
        }
        // Si alguno es real, el resultado es real
        return {true, Datatype::RealType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> SubExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);
   auto left_real = std::dynamic_pointer_cast<RealExpression>(left);
   auto right_real = std::dynamic_pointer_cast<RealExpression>(right);

   if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() - right_int->get_value());
    }

    if (left_real && right_real) {
        return std::make_shared<RealExpression>(left_real->get_value() - right_real->get_value());
    }

    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<IntExpression>(left_int->get_value() - right_int->get_value());
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
    
    // Verificar que ambos operandos sean numéricos
    if (is_compatible_with(left_type, Datatype::IntType) || is_compatible_with(left_type, Datatype::RealType)) {
        if (is_compatible_with(right_type, Datatype::IntType) || is_compatible_with(right_type, Datatype::RealType)) {
            // Si ambos son enteros, el resultado es entero
            if (is_compatible_with(left_type, Datatype::IntType) && is_compatible_with(right_type, Datatype::IntType)) {
                return {true, Datatype::IntType};
            }
            // Si alguno es real, el resultado es real
            return {true, Datatype::RealType};
        }
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> MulExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);
   auto left_real = std::dynamic_pointer_cast<RealExpression>(left);
   auto right_real = std::dynamic_pointer_cast<RealExpression>(right);

   if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() * right_int->get_value());
    }

    if (left_real && right_real) {
        return std::make_shared<RealExpression>(left_real->get_value() * right_real->get_value());
    }   
    
    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<IntExpression>(left_int->get_value() * right_int->get_value());

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
    
    if (!left_ok || !right_ok) return {false, Datatype::UnknownType};
    
    // Verificar que ambos operandos sean numéricos
    if (is_compatible_with(left_type, Datatype::IntType) || is_compatible_with(left_type, Datatype::RealType)) {
        if (is_compatible_with(right_type, Datatype::IntType) || is_compatible_with(right_type, Datatype::RealType)) {
            // Si ambos son enteros, el resultado es entero
            if (is_compatible_with(left_type, Datatype::IntType) && is_compatible_with(right_type, Datatype::IntType)) {
                return {true, Datatype::IntType};
            }
            // Si alguno es real, el resultado es real
            return {true, Datatype::RealType};
        }
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> DivExpression::eval(Environment& env) const
{
   auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   auto left_int = std::dynamic_pointer_cast<IntExpression>(left);
   auto right_int = std::dynamic_pointer_cast<IntExpression>(right);
   auto left_real = std::dynamic_pointer_cast<RealExpression>(left);
   auto right_real = std::dynamic_pointer_cast<RealExpression>(right);

   if (left_int && right_int) {
        return std::make_shared<IntExpression>(left_int->get_value() / right_int->get_value());
    }

    if (left_real && right_real) {
        return std::make_shared<RealExpression>(left_real->get_value() / right_real->get_value());
    }   
    
    // Si llegamos aquí, asumimos que son enteros por defecto
    return std::make_shared<IntExpression>(left_int->get_value() / right_int->get_value());}

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
    
    // Verificar que ambos operandos sean numéricos
    if (is_compatible_with(left_type, Datatype::IntType) || is_compatible_with(left_type, Datatype::RealType)) {
        if (is_compatible_with(right_type, Datatype::IntType) || is_compatible_with(right_type, Datatype::RealType)) {
            // La división siempre devuelve real
            return {true, Datatype::RealType};
        }
    }
    
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
    if (is_compatible_with(left_type, Datatype::IntType) && is_compatible_with(right_type, Datatype::IntType)) {
        return {true, Datatype::IntType};
    }
    
    return {false, Datatype::UnknownType};
}

std::shared_ptr<Expression> CallExpression::eval(Environment& env) const
{
    // El primer parámetro ya es un NameExpression, no necesitamos evaluarlo
    auto function_name = std::dynamic_pointer_cast<NameExpression>(BinaryExpression::get_left_expression());

    // Asumimos que function_name no es nullptr ya que type_check lo validó

    auto expression = env.lookup(function_name->get_name());

    if (expression == nullptr)
    {
        throw std::runtime_error{"function " + function_name->get_name() + " does not exist"};
    }

    auto closure = std::dynamic_pointer_cast<Closure>(expression);

    // Asumimos que closure no es nullptr ya que type_check lo validó

    Environment new_env = closure->get_environment();
    auto function = std::dynamic_pointer_cast<FunExpression>(closure->get_function_expression());

    new_env.add(function->get_parameter_name(), BinaryExpression::get_right_expression()->eval(env));
    new_env.add(function->get_name(), closure);
    
    return function->get_body_expression()->eval(new_env);
}

std::string CallExpression::to_string() const noexcept
{
    return "(call "
        + BinaryExpression::get_left_expression()->to_string()
        + " " + BinaryExpression::get_right_expression()->to_string() + ")";
}

std::pair<bool, Datatype> CallExpression::type_check(Environment& env) const noexcept
{
    // Verificar el tipo de la función
    auto [func_ok, func_type] = get_left_expression()->type_check(env);
    auto [arg_ok, arg_type] = get_right_expression()->type_check(env);
    
    if (!func_ok || !arg_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el primer operando sea una función
    if (func_type != Datatype::FunctionType) return {false, Datatype::UnknownType};
    
    // Obtener la closure de la función para verificar compatibilidad de tipos
    try {
        auto func_result = get_left_expression()->eval(env);
        auto closure = std::dynamic_pointer_cast<Closure>(func_result);
        
        if (closure) {
            // Verificar que el tipo del argumento sea compatible con el tipo esperado por la función
            Datatype expected_param_type = closure->get_parameter_type();
            
            if (expected_param_type != Datatype::UnknownType) {
                // Verificar compatibilidad de tipos
                // El argumento debe ser compatible con el tipo esperado por la función
                if (!is_compatible_with(expected_param_type, arg_type)) {
                    return {false, Datatype::UnknownType};
                }
            }
            
            // Si la función pasa el type check, devolver el tipo de retorno
            Datatype return_type = closure->get_return_type();
            if (return_type != Datatype::UnknownType) {
                return {true, return_type};
            }
        }
    } catch (...) {
        // Si hay un error, asumir que no es compatible
        return {false, Datatype::UnknownType};
    }
    
    // Para funciones con tipos desconocidos (como if-else), 
    // asumimos que pueden devolver cualquier tipo
    // El tipo específico se determinará durante la evaluación
    
    // Si no podemos determinar el tipo, asumir que puede devolver cualquier tipo
    return {true, Datatype::UnknownType};
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
    if (is_compatible_with(left_type, right_type)) {
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
                // Es una variable regular, asumir int
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
                // Es una variable regular, asumir int
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
    
    // Un par puede tener elementos de diferentes tipos
    // No validamos que tengan el mismo tipo base
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
    if (is_compatible_with(left_type, Datatype::StringType) && is_compatible_with(right_type, Datatype::StringType)) {
        return {true, Datatype::StringType};
    }
    
    return {false, Datatype::UnknownType};
}


std::shared_ptr<Expression> NegExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));

    return std::make_shared<IntExpression>(-result->get_value());
}

std::string NegExpression::to_string() const noexcept
{
    return "(- " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> NegExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Verificar que el operando sea numérico
    if (is_compatible_with(expr_type, Datatype::IntType) || is_compatible_with(expr_type, Datatype::RealType)) {
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
        // Para determinar el tipo del primer elemento, necesitamos hacer type_check
        // del primer elemento del par si es una PairExpression
        if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(get_expression())) {
            auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
            if (left_ok) {
                return {true, left_type};
            }
        }
        // Si no podemos determinar el tipo específico, intentar evaluar el par
        // para obtener el tipo real del primer elemento
        try {
            auto pair_result = get_expression()->eval(env);
            if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(pair_result)) {
                auto [left_ok, left_type] = pair_expr->get_left_expression()->type_check(env);
                if (left_ok) {
                    return {true, left_type};
                }
            }
        } catch (...) {
            // Si hay error en la evaluación, asumir tipo desconocido
        }
        // Si no podemos determinar el tipo específico, devolver tipo desconocido
        return {true, Datatype::UnknownType};
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
        // Para determinar el tipo del segundo elemento, necesitamos hacer type_check
        // del segundo elemento del par si es una PairExpression
        if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(get_expression())) {
            auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
            if (right_ok) {
                return {true, right_type};
            }
        }
        // Si no podemos determinar el tipo específico, intentar evaluar el par
        // para obtener el tipo real del segundo elemento
        try {
            auto pair_result = get_expression()->eval(env);
            if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(pair_result)) {
                auto [right_ok, right_type] = pair_expr->get_right_expression()->type_check(env);
                if (right_ok) {
                    return {true, right_type};
                }
            }
        } catch (...) {
            // Si hay error en la evaluación, asumir tipo desconocido
        }
        // Si no podemos determinar el tipo específico, devolver tipo desconocido
        return {true, Datatype::UnknownType};
    }
    
    return {false, Datatype::UnknownType};
}



std::shared_ptr<Expression> HeadExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    return result->get_left_expression();
}

std::string HeadExpression::to_string() const noexcept
{
    return "(head " + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> HeadExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Para simplificar, asumimos que el head de cualquier tipo es int
    return {true, Datatype::IntType};
}

std::shared_ptr<Expression> TailExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    return result->get_right_expression();
}

std::string TailExpression::to_string() const noexcept
{
    return "(tail " + UnaryExpression::get_expression()->to_string() + ")";
}
std::pair<bool, Datatype> TailExpression::type_check(Environment& env) const noexcept
{
    auto [expr_ok, expr_type] = get_expression()->type_check(env);
    
    if (!expr_ok) return {false, Datatype::UnknownType};
    
    // Para simplificar, asumimos que cualquier tipo puede ser el tail de un array
    return {true, expr_type};
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
    if (is_compatible_with(expr_type, Datatype::RealType)) {
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
    if (is_compatible_with(expr_type, Datatype::IntType)) {
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
    if (is_compatible_with(expr_type, Datatype::IntType)) {
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
    if (is_compatible_with(expr_type, Datatype::RealType)) {
        return {true, Datatype::IntType};
    }
    
    return {false, Datatype::UnknownType};
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
        case Datatype::ArrayType:
            // Para arrays genéricos, permitir cualquier tipo
            return {true, array_type};
        default:
            return {false, Datatype::UnknownType};
    }
    
    // Verificar que el tipo del elemento sea compatible con el tipo base del array
    if (!is_compatible_with(array_base_type, elem_type)) {
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
    if (!is_compatible_with(index_type, Datatype::IntType)) {
        return {false, Datatype::UnknownType};
    }
    
    // Para simplificar, asumimos que el resultado es del mismo tipo que el array
    return {true, array_type};
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
    
    // Verificar que la condición sea booleana
    if (!is_compatible_with(cond_type, Datatype::BoolType)) {
        return {false, Datatype::UnknownType};
    }
    
    // Verificar que ambas ramas tengan el mismo tipo
    if (is_compatible_with(true_type, false_type)) {
        return {true, true_type};
    }
    
    return {false, Datatype::UnknownType};
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

std::shared_ptr<Expression> FunExpression::eval(Environment& env) const {
    // Para funciones con if-else que pueden devolver diferentes tipos,
    // no podemos inferir un tipo específico en la declaración
    // El tipo se determinará durante la evaluación de la llamada
    return std::make_shared<Closure>(env, std::make_shared<FunExpression>(*this),
                                    Datatype::UnknownType, Datatype::UnknownType);
}

std::string FunExpression::to_string() const noexcept {
    return "(fun " + 
           function_name_expression->to_string() + " " +
           parameter_name_expression->to_string() + " " +
           get_body_expression()->to_string() + ")";
}

std::pair<bool, Datatype> FunExpression::type_check(Environment& env) const noexcept
{
    // Por simplicidad, asumimos que las funciones pueden tomar cualquier tipo
    // El type checking real se hará durante la evaluación de la llamada
    return {true, Datatype::FunctionType};
}


LetExpression::LetExpression(std::shared_ptr<Expression> _var_name, 
                           std::shared_ptr<Expression> _var_expression, 
                           std::shared_ptr<Expression> _body_expression) noexcept
    : var_name(_var_name), var_expression(_var_expression), body_expression(_body_expression) {}

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
    
    env.add(name_expr->get_name(), var_value);
    
    return body_expression->eval(env);
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
        new_env.add(var_name_expr->get_name(), std::make_shared<IntExpression>(0));
    }
    
    // Verificar el tipo del cuerpo
    auto [body_ok, body_type] = body_expression->type_check(new_env);
    
    if (!body_ok) return {false, Datatype::UnknownType};
    
    return {true, body_type};
}

std::shared_ptr<Expression> PrintExpression::eval(Environment& env) const {
    auto result = get_expression()->eval(env);
    printf("Print: %s\n", result->to_string().c_str());
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






std::string datatype_to_string(Datatype type) noexcept {
    switch (type) {
        case Datatype::IntType: return "int";
        case Datatype::RealType: return "real";
        case Datatype::StringType: return "string";
        case Datatype::BoolType: return "bool";
        case Datatype::PairType: return "pair";
        case Datatype::ArrayType: return "array";
        case Datatype::IntArrayType: return "int_array";
        case Datatype::RealArrayType: return "real_array";
        case Datatype::StringArrayType: return "string_array";
        case Datatype::BoolArrayType: return "bool_array";
        case Datatype::FunctionType: return "function";
        case Datatype::UnknownType: return "unknown";
        default: return "unknown";
    }
}

// Función auxiliar para determinar el tipo base de una expresión
// Para pares anidados, determina el tipo base de los elementos
Datatype get_base_type(std::shared_ptr<Expression> expr, Environment& env) {
    auto [ok, type] = expr->type_check(env);
    if (!ok) return Datatype::UnknownType;
    
    if (type == Datatype::PairType) {
        // Si es un par, determinar el tipo base del primer elemento
        if (auto pair_expr = std::dynamic_pointer_cast<PairExpression>(expr)) {
            return get_base_type(pair_expr->get_left_expression(), env);
        }
    }
    
    return type;
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

bool is_compatible_with(Datatype type1, Datatype type2) noexcept {
    // IntType es compatible con IntType
    if (type1 == Datatype::IntType) {
        return type2 == Datatype::IntType;
    }
    
    // RealType es compatible con IntType y RealType (los enteros se pueden convertir a reales)
    if (type1 == Datatype::RealType) {
        return type2 == Datatype::IntType || type2 == Datatype::RealType;
    }
    
    // BoolType solo es compatible con BoolType
    if (type1 == Datatype::BoolType) {
        return type2 == Datatype::BoolType;
    }
    
    // StringType solo es compatible con StringType
    if (type1 == Datatype::StringType) {
        return type2 == Datatype::StringType;
    }
    
    // PairType es compatible con PairType (acepta cualquier tipo interno)
    if (type1 == Datatype::PairType) {
        return type2 == Datatype::PairType;
    }
    
    // ArrayType solo es compatible con ArrayType
    if (type1 == Datatype::ArrayType) {
        return type2 == Datatype::ArrayType;
    }
    
    // Tipos específicos de arrays solo son compatibles con el mismo tipo específico
    if (type1 == Datatype::IntArrayType) {
        return type2 == Datatype::IntArrayType;
    }
    
    if (type1 == Datatype::RealArrayType) {
        return type2 == Datatype::RealArrayType;
    }
    
    if (type1 == Datatype::StringArrayType) {
        return type2 == Datatype::StringArrayType;
    }
    
    if (type1 == Datatype::BoolArrayType) {
        return type2 == Datatype::BoolArrayType;
    }
    
    // FunctionType solo es compatible con FunctionType
    if (type1 == Datatype::FunctionType) {
        return type2 == Datatype::FunctionType;
    }
    
    // UnknownType es compatible con cualquier tipo
    if (type1 == Datatype::UnknownType) {
        return true;
    }
    
    return false;
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
