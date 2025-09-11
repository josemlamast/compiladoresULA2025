#include "utils.hpp"
#include "expression.hpp"
#include "Exceptions.hpp"





std::shared_ptr<Expression> NotExpression::eval(Environment& env) const
{
    auto expr = get_expression()->eval(env);
    if (auto bool_expr = std::dynamic_pointer_cast<BoolExpression>(expr)) {
        return std::make_shared<BoolExpression>(!bool_expr->get_value());
    }
    throw TypeError("NotExpression: type error");
}

std::string NotExpression::to_string() const noexcept
{
    return "(not " + get_expression()->to_string() + ")";
}



std::shared_ptr<Expression> AndExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

    if (auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left)) {
        if (auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right)) {
            return std::make_shared<BoolExpression>(left_bool->get_value() && right_bool->get_value());
        }
    }

    throw TypeError("AndExpression: type error");
}

std::string AndExpression::to_string() const noexcept
{
    return "(and " + get_left_expression()->to_string() + " " + get_right_expression()->to_string() + ")";
}

std::shared_ptr<Expression> XorExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left_result);
    auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right_result);
    
    if (left_bool && right_bool) {
        bool result = (left_bool->get_value() && !right_bool->get_value()) || 
                     (!left_bool->get_value() && right_bool->get_value());
        return std::make_shared<BoolExpression>(result);
    }
    
    throw TypeError{"XOR operation requires boolean operands"};
}

std::string XorExpression::to_string() const noexcept {
    return "(xor " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}

std::shared_ptr<Expression> OrExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

    if (auto left_bool = std::dynamic_pointer_cast<BoolExpression>(left)) {
        if (auto right_bool = std::dynamic_pointer_cast<BoolExpression>(right)) {
            return std::make_shared<BoolExpression>(left_bool->get_value() || right_bool->get_value());
        }
    }

    throw TypeError("OrExpression: type error");
}

std::string OrExpression::to_string() const noexcept
{
    return "(or " + get_left_expression()->to_string() + " " + get_right_expression()->to_string() + ")";
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
    
    throw TypeError{"Less than operation requires numeric operands"};
}

std::string LessExpression::to_string() const noexcept {
    return "(< " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
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
    
    throw TypeError{"LessEq than operation requires numeric operands"};
}

std::string LessEqExpression::to_string() const noexcept {
    return "(<= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
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
    
    throw TypeError{"Greater than operation requires numeric operands"};
}

std::string GreaterExpression::to_string() const noexcept {
    return "(> " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
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
    
    throw TypeError{"GreaterEq than operation requires numeric operands"};
}

std::string GreaterEqExpression::to_string() const noexcept {
    return "(>= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
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

std::shared_ptr<Expression> NotEqualExpression::eval(Environment& env) const {
    auto equal_result = EqualExpression(get_left_expression(), get_right_expression()).eval(env);
    
    if (auto equal_bool = std::dynamic_pointer_cast<BoolExpression>(equal_result)) {
        return std::make_shared<BoolExpression>(!equal_bool->get_value());
    }
    
    throw TypeError{"Internal error in not equal comparison"};
}

std::string NotEqualExpression::to_string() const noexcept {
    return "(!= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}


std::shared_ptr<Expression> AddExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   if (auto left_int = std::dynamic_pointer_cast<IntExpression>(left)) {
        if (auto right_int = std::dynamic_pointer_cast<IntExpression>(right)) {
            return std::make_shared<IntExpression>(left_int->get_value() + right_int->get_value());
        }
    }

    if (auto left_real = std::dynamic_pointer_cast<RealExpression>(left)) {
        if (auto right_real = std::dynamic_pointer_cast<RealExpression>(right)) {
            return std::make_shared<RealExpression>(left_real->get_value() + right_real->get_value());
        }
    }

    throw TypeError("AddExpression: type error");
}

std::string AddExpression::to_string() const noexcept
{
    return "(+" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::shared_ptr<Expression> SubExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   if (auto left_int = std::dynamic_pointer_cast<IntExpression>(left)) {
        if (auto right_int = std::dynamic_pointer_cast<IntExpression>(right)) {
            return std::make_shared<IntExpression>(left_int->get_value() - right_int->get_value());
        }
    }

    if (auto left_real = std::dynamic_pointer_cast<RealExpression>(left)) {
        if (auto right_real = std::dynamic_pointer_cast<RealExpression>(right)) {
            return std::make_shared<RealExpression>(left_real->get_value() - right_real->get_value());
        }
    }

    throw TypeError("SubExpression: type error");
}

std::string SubExpression::to_string() const noexcept
{
    return "(-" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::shared_ptr<Expression> MulExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   if (auto left_int = std::dynamic_pointer_cast<IntExpression>(left)) {
        if (auto right_int = std::dynamic_pointer_cast<IntExpression>(right)) {
            return std::make_shared<IntExpression>(left_int->get_value() * right_int->get_value());
        }
    }

    if (auto left_real = std::dynamic_pointer_cast<RealExpression>(left)) {
        if (auto right_real = std::dynamic_pointer_cast<RealExpression>(right)) {
            return std::make_shared<RealExpression>(left_real->get_value() * right_real->get_value());
        }
    }   
    throw TypeError("MulExpression: type error");

}

std::string MulExpression::to_string() const noexcept
{
    return "(*" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::shared_ptr<Expression> DivExpression::eval(Environment& env) const
{
   auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   if (auto left_int = std::dynamic_pointer_cast<IntExpression>(left)) {
        if (auto right_int = std::dynamic_pointer_cast<IntExpression>(right)) {
            return std::make_shared<IntExpression>(left_int->get_value() / right_int->get_value());
        }
    }

    if (auto left_real = std::dynamic_pointer_cast<RealExpression>(left)) {
        if (auto right_real = std::dynamic_pointer_cast<RealExpression>(right)) {
            return std::make_shared<RealExpression>(left_real->get_value() / right_real->get_value());
        }
    }   
    throw TypeError("DivExpression: type error");}

std::string DivExpression::to_string() const noexcept
{
    return "(/" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}

std::shared_ptr<Expression> ModExpression::eval(Environment& env) const
{
    auto left = get_left_expression()->eval(env);
    auto right = get_right_expression()->eval(env);

   if (auto left_int = std::dynamic_pointer_cast<IntExpression>(left)) {
        if (auto right_int = std::dynamic_pointer_cast<IntExpression>(right)) {
            return std::make_shared<IntExpression>(left_int->get_value() % right_int->get_value());
        }
    }

    throw TypeError("ModExpression: type error - modulo only works with integers");
}

std::string ModExpression::to_string() const noexcept
{
    return "(%" +
        BinaryExpression::get_left_expression()->to_string() +
        BinaryExpression::get_right_expression()->to_string() + ")";
}




std::shared_ptr<Expression> CallExpression::eval(Environment& env) const
{
    // El primer parámetro ya es un NameExpression, no necesitamos evaluarlo
    auto function_name = std::dynamic_pointer_cast<NameExpression>(BinaryExpression::get_left_expression());

    if (function_name == nullptr)
    {
        throw TypeError{"call applied to non-identifier"};
    }

    auto expression = env.lookup(function_name->get_name());

    if (expression == nullptr)
    {
        throw std::runtime_error{"function " + function_name->get_name() + " does not exist"};
    }

    auto closure = std::dynamic_pointer_cast<Closure>(expression);

    if (closure == nullptr)
    {
        throw TypeError{"call applied to non-closure"};
    }

    Environment new_env = closure->get_environment();
    auto function = std::dynamic_pointer_cast<FunExpression>(closure->get_function_expression());

    new_env.add(function->get_parameter_name(), BinaryExpression::get_right_expression()->eval(env));
    new_env.add(function->get_name(), closure);
    
    return function->get_body_expression()->eval(new_env);
}

std::string CallExpression::to_string() const noexcept
{
    return "(call"
        + BinaryExpression::get_left_expression()->to_string()
        + BinaryExpression::get_right_expression()->to_string() + ")";
}


std::shared_ptr<Expression> AssignmentExpression::eval(Environment& env) const {
    auto right_value = get_right_expression()->eval(env);
    auto left_name_expr = std::dynamic_pointer_cast<NameExpression>(get_left_expression());
    
    if (!left_name_expr) {
        throw TypeError{"Left side of assignment must be a variable name"};
    }
    
    std::string var_name = left_name_expr->get_name();
    
    env.add(var_name, right_value);
    
    return right_value;
}

std::string AssignmentExpression::to_string() const noexcept {
    return "(= " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
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

std::shared_ptr<Expression> ConcatExpression::eval(Environment& env) const {
    auto left_result = get_left_expression()->eval(env);
    auto right_result = get_right_expression()->eval(env);
    
    auto left_str = std::dynamic_pointer_cast<StrExpression>(left_result);
    auto right_str = std::dynamic_pointer_cast<StrExpression>(right_result);
    
    if (left_str && right_str) {
        std::string result = left_str->get_value() + right_str->get_value();
        return std::make_shared<StrExpression>(result);
    }
    
    throw TypeError{"String concatenation requires both operands to be strings"};
}

std::string ConcatExpression::to_string() const noexcept {
    return "(concat " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}


std::shared_ptr<Expression> NegExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"negation applied to non-int"};
    }

    return std::make_shared<IntExpression>(-result->get_value());
}

std::string NegExpression::to_string() const noexcept
{
    return "(-" + UnaryExpression::get_expression()->to_string() + ")";
}

std::shared_ptr<Expression> FstExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"fst applied to non-pair"};
    }

    return result->get_left_expression();
}

std::string FstExpression::to_string() const noexcept
{
    return "(fst" + UnaryExpression::get_expression()->to_string() + ")";
}

std::shared_ptr<Expression> SndExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"snd applied to non-pair"};
    }

    return result->get_right_expression();
}

std::string SndExpression::to_string() const noexcept
{
    return "(snd" + UnaryExpression::get_expression()->to_string() + ")";
}



std::shared_ptr<Expression> HeadExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"head applied to non-pair"};
    }

    return result->get_left_expression();
}

std::string HeadExpression::to_string() const noexcept
{
    return "(head" + UnaryExpression::get_expression()->to_string() + ")";
}

std::shared_ptr<Expression> TailExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<PairExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"tail applied to non-pair"};
    }

    return result->get_right_expression();
}

std::string TailExpression::to_string() const noexcept
{
    return "(tail" + UnaryExpression::get_expression()->to_string() + ")";
}



std::shared_ptr<Expression> RtoSExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<RealExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"rtos applied to non-real"};
    }

    return std::make_shared<StrExpression>(std::to_string(result->get_value()));
}

std::string RtoSExpression::to_string() const noexcept
{
    return "(rtos" + UnaryExpression::get_expression()->to_string() + ")";
}


std::shared_ptr<Expression> ItoSExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"itos applied to non-int"};
    }

    return std::make_shared<StrExpression>(std::to_string(result->get_value()));
}

std::string ItoSExpression::to_string() const noexcept
{
    return "(itos" + UnaryExpression::get_expression()->to_string() + ")";
}


std::shared_ptr<Expression> ItoRExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<IntExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"itor applied to non-int"};
    }

    return std::make_shared<RealExpression>(static_cast<double>(result->get_value()));
}

std::string ItoRExpression::to_string() const noexcept
{
    return "(itor" + UnaryExpression::get_expression()->to_string() + ")";
}


std::shared_ptr<Expression> RtoIExpression::eval(Environment& env) const
{
    auto result = std::dynamic_pointer_cast<RealExpression>(UnaryExpression::get_expression()->eval(env));

    if (result == nullptr)
    {
        throw TypeError{"rtoi applied to non-real"};
    }

    return std::make_shared<IntExpression>(static_cast<int>(result->get_value()));
}

std::string RtoIExpression::to_string() const noexcept
{
    return "(rtoi" + UnaryExpression::get_expression()->to_string() + ")";
}



std::shared_ptr<Expression> ArrayAddExpression::eval(Environment& env) const {
    auto array_result = get_left_expression()->eval(env);
    auto element_result = get_right_expression()->eval(env);

    return array_result;
}

std::string ArrayAddExpression::to_string() const noexcept {
    return "(add_array " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
}


std::shared_ptr<Expression> ArrayDelExpression::eval(Environment& env) const {
    auto array_result = get_left_expression()->eval(env);
    auto index_result = get_right_expression()->eval(env);
    
    return array_result;
}

std::string ArrayDelExpression::to_string() const noexcept {
    return "(del_array " + 
           get_left_expression()->to_string() + " " +
           get_right_expression()->to_string() + ")";
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

    if (condition_bool == nullptr)
    {
        throw TypeError{"ifelse condition must be boolean"};
    }
    
    if (condition_bool->get_value())
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

FunExpression::FunExpression(std::shared_ptr<Expression> _function_name_expression, 
                            std::shared_ptr<Expression> _parameter_name_expression, 
                            std::shared_ptr<Expression> _body_expression) noexcept
    : UnaryExpression(_body_expression), 
      function_name_expression(_function_name_expression), 
      parameter_name_expression(_parameter_name_expression) {}

std::shared_ptr<Expression> FunExpression::get_function_name_expression() const noexcept {
    return function_name_expression;
}

std::shared_ptr<Expression> FunExpression::get_parameter_name_expression() const noexcept {
    return parameter_name_expression;
}

const std::string& FunExpression::get_name() const noexcept {
    auto name_expr = std::dynamic_pointer_cast<NameExpression>(function_name_expression);
    return name_expr ? name_expr->get_name() : static_cast<const std::string&>("");
}

const std::string& FunExpression::get_parameter_name() const noexcept {
    auto param_expr = std::dynamic_pointer_cast<NameExpression>(parameter_name_expression);
    return param_expr ? param_expr->get_name() : static_cast<const std::string&>("");
}

std::shared_ptr<Expression> FunExpression::get_body_expression() const noexcept {
    return UnaryExpression::get_expression();
}

std::shared_ptr<Expression> FunExpression::eval(Environment& env) const {
    return std::make_shared<Closure>(env, std::make_shared<FunExpression>(*this));
}

std::string FunExpression::to_string() const noexcept {
    return "(fun " + 
           function_name_expression->to_string() + " " +
           parameter_name_expression->to_string() + " " +
           get_body_expression()->to_string() + ")";
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

std::shared_ptr<Expression> PrintExpression::eval(Environment& env) const {
    auto result = get_expression()->eval(env);
    printf("Print: %s\n", result->to_string().c_str());
    return result;
}

std::string PrintExpression::to_string() const noexcept {
    return "(print " + get_expression()->to_string() + ")";
}
