#include <expression.hpp>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

using namespace std::literals;

namespace {
    std::string value_to_string(const Value_t& val) noexcept {
        return std::visit([](const auto& v) -> std::string {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, bool>) {
                return v ? "true" : "false";
            } else if constexpr (std::is_same_v<T, std::string>) {
                return "\"" + v + "\"";
            } else {
                return std::to_string(v);
            }
        }, val);
    }

    bool value_to_bool(const Value_t& val) noexcept {
        return std::visit([](const auto& v) -> bool {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, bool>) {
                return v;
            } else if constexpr (std::is_same_v<T, int>) {
                return v != 0;
            } else if constexpr (std::is_same_v<T, double>) {
                return v != 0.0;
            } else {
                return !v.empty();
            }
        }, val);
    }
}

Environment::Environment(Environment* parent) noexcept 
    : parent_env(parent) {}

void Environment::bind(const std::string& name, Value_t value) noexcept {
    bindings[name] = std::move(value);
}

void Environment::bind_function(const std::string& name, FunctionDefinition* func) noexcept {
    functions[name] = func;
}

Value_t Environment::lookup(const std::string& name) const noexcept {
    auto it = bindings.find(name);
    if (it != bindings.end()) {
        return it->second;
    }
    if (parent_env) {
        return parent_env->lookup(name);
    }
    return 0;
}

FunctionDefinition* Environment::lookup_function(const std::string& name) const noexcept {
    auto it = functions.find(name);
    if (it != functions.end()) {
        return it->second;
    }
    if (parent_env) {
        return parent_env->lookup_function(name);
    }
    return nullptr;
}

bool Environment::exists(const std::string& name) const noexcept {
    if (bindings.find(name) != bindings.end()) {
        return true;
    }
    return parent_env ? parent_env->exists(name) : false;
}

Environment* Environment::create_child() noexcept {
    return new Environment(this);
}

Value_t PrintExpression::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    std::string output = value_to_string(val);
    if (output.front() == '"' && output.back() == '"') {
        output = output.substr(1, output.length() - 2);
    }
    std::cout << output << std::endl;
    return val;
}
std::string PrintExpression::operand_str() const noexcept {
    return "print";
}

std::string PrintExpression::get_type() const noexcept {
    return "print_expression";
}


UnaryOperation::UnaryOperation(Expression* expr) noexcept : expression{expr} {}
void UnaryOperation::destroy() noexcept {
    if (expression) {
        expression->destroy();
        delete expression;
        expression = nullptr;
    }
}
std::string UnaryOperation::to_string() const noexcept {
    return operand_str() + "(" + expression->to_string() + ")";
}

Value_t StringConcat::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return value_to_string(left_val) + value_to_string(right_val);
}
std::string StringConcat::operand_str() const noexcept {
    return " # ";
}
std::string StringConcat::get_type() const noexcept {
    return "string_concat";
}

Value_t ConvertIntToReal::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return std::visit([](const auto& v) -> Value_t {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, int>) {
            return static_cast<double>(v);
        }
        return 0.0;
    }, val);
}
std::string ConvertIntToReal::operand_str() const noexcept {
    return "itor";
}
std::string ConvertIntToReal::get_type() const noexcept {
    return "convert_int_to_real";
}

Value_t ConvertRealToInt::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return std::visit([](const auto& v) -> Value_t {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, double>) {
            return static_cast<int>(v);
        }
        return 0;
    }, val);
}
std::string ConvertRealToInt::operand_str() const noexcept {
    return "rtoi";
}
std::string ConvertRealToInt::get_type() const noexcept {
    return "convert_real_to_int";
}

Value_t ConvertIntToString::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return std::visit([](const auto& v) -> Value_t {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, int>) {
            return std::to_string(v);
        }
        return std::string("");
    }, val);
}
std::string ConvertIntToString::operand_str() const noexcept {
    return "itos";
}
std::string ConvertIntToString::get_type() const noexcept {
    return "convert_int_to_string";
}

Value_t ConvertRealToString::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return std::visit([](const auto& v) -> Value_t {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, double>) {
            return std::to_string(v);
        }
        return std::string("");
    }, val);
}
std::string ConvertRealToString::operand_str() const noexcept {
    return "rtos";
}
std::string ConvertRealToString::get_type() const noexcept {
    return "convert_real_to_string";
}


IfExpression::IfExpression(Expression* condition, Expression* then_expr, Expression* else_expr) noexcept
    : condition_expr{condition}, then_expr{then_expr}, else_expr{else_expr} {}
void IfExpression::destroy() noexcept {
    if (condition_expr) {
        condition_expr->destroy();
        delete condition_expr;
        condition_expr = nullptr;
    }
    if (then_expr) {
        then_expr->destroy();
        delete then_expr;
        then_expr = nullptr;
    }
    if (else_expr) {
        else_expr->destroy();
        delete else_expr;
        else_expr = nullptr;
    }
}
Value_t IfExpression::eval(Environment* env) noexcept {
    auto condition_val = condition_expr->eval(env);
    
    if (value_to_bool(condition_val)) {
        return then_expr->eval(env);
    } else if (else_expr) {
        return else_expr->eval(env);
    }
    
    return 0;
}
std::string IfExpression::to_string() const noexcept {
    std::string result = "if(" + condition_expr->to_string() + ", " + then_expr->to_string();
    if (else_expr) {
        result += ", " + else_expr->to_string();
    }
    result += ")";
    return result;
}
std::string IfExpression::get_type() const noexcept {
    return "if_expression";
}
LetExpression::LetExpression(Expression* var, Expression* value, Expression* body) noexcept
    : variable{var}, value_expr{value}, body_expr{body} {}
void LetExpression::destroy() noexcept {
    if (variable) {
        variable->destroy();
        delete variable;
        variable = nullptr;
    }
    if (value_expr) {
        value_expr->destroy();
        delete value_expr;
        value_expr = nullptr;
    }
    if (body_expr) {
        body_expr->destroy();
        delete body_expr;
        body_expr = nullptr;
    }
}
Value_t LetExpression::eval(Environment* env) noexcept {
    Environment* local_env = env ? env->create_child() : new Environment();
    
    auto val = value_expr->eval(env);
    
    if (auto* id = dynamic_cast<Identifier*>(variable)) {
        local_env->bind(id->get_name(), val);
    }
    
    auto result = body_expr->eval(local_env);
    
    if (!env) {
        delete local_env;
    }
    
    return result;
}
std::string LetExpression::to_string() const noexcept {
    return "let(" + variable->to_string() + ", " + value_expr->to_string() + ", " + body_expr->to_string() + ")";
}
std::string LetExpression::get_type() const noexcept {
    return "let_expression";
}



Array::Array() noexcept : head_expr{nullptr}, tail_array{nullptr}, empty{true} {}
Array::Array(Expression* head, Array* tail) noexcept 
    : head_expr{head}, tail_array{tail}, empty{false} {}
void Array::destroy() noexcept {
    if (head_expr) {
        head_expr->destroy();
        delete head_expr;
        head_expr = nullptr;
    }
    if (tail_array) {
        tail_array->destroy();
        delete tail_array;
        tail_array = nullptr;
    }
}
Value_t Array::eval(Environment* env) noexcept {
    if (empty) {
        return std::string("[]");
    }
    
    std::string result = "[";
    Array* current = this;
    bool first = true;
    
    while (current && !current->is_empty()) {
        if (!first) result += ", ";
        result += value_to_string(current->head_expr->eval(env));
        current = current->tail_array;
        first = false;
    }
    
    result += "]";
    return result;
}
std::string Array::to_string() const noexcept {
    if (empty) {
        return "[]";
    }
    return "[" + head_expr->to_string() + " | " + (tail_array ? tail_array->to_string() : "[]") + "]";
}
std::string Array::get_type() const noexcept {
    return "array";
}
bool Array::is_empty() const noexcept {
    return empty;
}
Expression* Array::get_head() const noexcept {
    return head_expr;
}
Array* Array::get_tail() const noexcept {
    return tail_array;
}

EmptyArray::EmptyArray() noexcept {}
void EmptyArray::destroy() noexcept {}
Value_t EmptyArray::eval(Environment* env) noexcept {
    return std::string("[]");
}
std::string EmptyArray::to_string() const noexcept {
    return "empty";
}
std::string EmptyArray::get_type() const noexcept {
    return "empty_array";
}

Value_t Head::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return val;
}
std::string Head::operand_str() const noexcept {
    return "head";
}
std::string Head::get_type() const noexcept {
    return "head";
}
Value_t Tail::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return val;
}
std::string Tail::operand_str() const noexcept {
    return "tail";
}
std::string Tail::get_type() const noexcept {
    return "tail";
}
Value_t InArray::eval(Environment* env) noexcept {
    auto element = left_expression->eval(env);
    auto array_val = right_expression->eval(env);
    
    std::string element_str = value_to_string(element);
    std::string array_str = value_to_string(array_val);
    
    return array_str.find(element_str) != std::string::npos;
}
std::string InArray::operand_str() const noexcept {
    return " in ";
}
std::string InArray::get_type() const noexcept {
    return "in_array";
}


IntegerValue::IntegerValue(int val) noexcept : value{val} {}
void IntegerValue::destroy() noexcept {}
Value_t IntegerValue::eval(Environment* env) noexcept {
    return value;
}
std::string IntegerValue::to_string() const noexcept {
    return std::to_string(value);
}
std::string IntegerValue::get_type() const noexcept {
    return "int";
}


RealValue::RealValue(double val) noexcept : value{val} {}
void RealValue::destroy() noexcept {}
Value_t RealValue::eval(Environment* env) noexcept {
    return value;
}
std::string RealValue::to_string() const noexcept {
    return std::to_string(value);
}
std::string RealValue::get_type() const noexcept {
    return "real";
}


BooleanValue::BooleanValue(bool val) noexcept : value{val} {}
void BooleanValue::destroy() noexcept {}
Value_t BooleanValue::eval(Environment* env) noexcept {
    return value;
}
std::string BooleanValue::to_string() const noexcept {
    return value ? "true" : "false";
}
std::string BooleanValue::get_type() const noexcept {
    return "bool";
}

StringValue::StringValue(const std::string& val) noexcept : value{val} {}
void StringValue::destroy() noexcept {}
Value_t StringValue::eval(Environment* env) noexcept {
    return value;
}
std::string StringValue::to_string() const noexcept {
    return "\"" + value + "\"";
}
std::string StringValue::get_type() const noexcept {
    return "string";
}

Identifier::Identifier(const char* name) noexcept : name{name ? name : ""} {}
void Identifier::destroy() noexcept {}
Value_t Identifier::eval(Environment* env) noexcept {
    if (env) {
        return env->lookup(name);
    }
    return 0;
}
std::string Identifier::to_string() const noexcept {
    return name;
}
std::string Identifier::get_type() const noexcept {
    return "identifier";
}
const std::string& Identifier::get_name() const noexcept {
    return name;
}

BinaryOperation::BinaryOperation(Expression* e1, Expression* e2) noexcept
    : left_expression{e1}, right_expression{e2} {}
void BinaryOperation::destroy() noexcept {
    if (left_expression) {
        left_expression->destroy();
        delete left_expression;
        left_expression = nullptr;
    }
    if (right_expression) {
        right_expression->destroy();
        delete right_expression;
        right_expression = nullptr;
    }
}
std::string BinaryOperation::to_string() const noexcept {
    return "("s + left_expression->to_string() + operand_str() + right_expression->to_string() + ")"s;
}


Value_t Addition::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> Value_t {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l + r;
        } else if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
            return l + r;
        } else {
            return 0;
        }
    }, left_val, right_val);
}
std::string Addition::operand_str() const noexcept {
    return " + ";
}
std::string Addition::get_type() const noexcept {
    return "addition";
}

Value_t Subtraction::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> Value_t {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l - r;
        } else {
            return 0;
        }
    }, left_val, right_val);
}
std::string Subtraction::operand_str() const noexcept {
    return " - ";
}
std::string Subtraction::get_type() const noexcept {
    return "subtraction";
}

Value_t Multiplication::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> Value_t {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l * r;
        } else {
            return 0;
        }
    }, left_val, right_val);
}
std::string Multiplication::operand_str() const noexcept {
    return " * ";
}
std::string Multiplication::get_type() const noexcept {
    return "multiplication";
}

Value_t Division::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> Value_t {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            if (r != 0) {
                return l / r;
            }
        }
        return 0;
    }, left_val, right_val);
}
std::string Division::operand_str() const noexcept {
    return " / ";
}
std::string Division::get_type() const noexcept {
    return "division";
}

Value_t Modulo::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> Value_t {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_same_v<L, int> && std::is_same_v<R, int>) {
            if (r != 0) {
                return l % r;
            }
        }
        return 0;
    }, left_val, right_val);
}
std::string Modulo::operand_str() const noexcept {
    return " % ";
}
std::string Modulo::get_type() const noexcept {
    return "modulo";
}


Value_t LogicalAnd::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return value_to_bool(left_val) && value_to_bool(right_val);
}
std::string LogicalAnd::operand_str() const noexcept {
    return " and ";
}
std::string LogicalAnd::get_type() const noexcept {
    return "logical_and";
}

Value_t LogicalOr::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return value_to_bool(left_val) || value_to_bool(right_val);
}
std::string LogicalOr::operand_str() const noexcept {
    return " or ";
}
std::string LogicalOr::get_type() const noexcept {
    return "logical_or";
}

Value_t LogicalXor::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return value_to_bool(left_val) != value_to_bool(right_val);
}
std::string LogicalXor::operand_str() const noexcept {
    return " xor ";
}
std::string LogicalXor::get_type() const noexcept {
    return "logical_xor";
}

Value_t LogicalNot::eval(Environment* env) noexcept {
    auto val = expression->eval(env);
    return !value_to_bool(val);
}
std::string LogicalNot::operand_str() const noexcept {
    return "not";
}
std::string LogicalNot::get_type() const noexcept {
    return "logical_not";
}

ValExpression::ValExpression(Expression* var, Expression* value) noexcept
    : variable{var}, value_expr{value} {}
void ValExpression::destroy() noexcept {
    if (variable) {
        variable->destroy();
        delete variable;
        variable = nullptr;
    }
    if (value_expr) {
        value_expr->destroy();
        delete value_expr;
        value_expr = nullptr;
    }
}
Value_t ValExpression::eval(Environment* env) noexcept {
    auto val = value_expr->eval(env);
    
    if (auto* id = dynamic_cast<Identifier*>(variable)) {
        if (env) {
            env->bind(id->get_name(), val);
        }
    }
    
    return val;
}
std::string ValExpression::to_string() const noexcept {
    return "val(" + variable->to_string() + ", " + value_expr->to_string() + ")";
}
std::string ValExpression::get_type() const noexcept {
    return "val_expression";
}


Pair::Pair(Expression* first, Expression* second) noexcept 
    : first_expr{first}, second_expr{second} {}
void Pair::destroy() noexcept {
    if (first_expr) {
        first_expr->destroy();
        delete first_expr;
        first_expr = nullptr;
    }
    if (second_expr) {
        second_expr->destroy();
        delete second_expr;
        second_expr = nullptr;
    }
}
Value_t Pair::eval(Environment* env) noexcept {
    auto first_val = first_expr->eval(env);
    auto second_val = second_expr->eval(env);
    return "(" + value_to_string(first_val) + ", " + value_to_string(second_val) + ")";
}
std::string Pair::to_string() const noexcept {
    return "pair(" + first_expr->to_string() + ", " + second_expr->to_string() + ")";
}
std::string Pair::get_type() const noexcept {
    return "pair";
}
Expression* Pair::get_first() const noexcept {
    return first_expr;
}
Expression* Pair::get_second() const noexcept {
    return second_expr;
}

Value_t First::eval(Environment* env) noexcept {
    if (auto* pair = dynamic_cast<Pair*>(expression)) {
        return pair->first_expr->eval(env);
    }
    
    auto val = expression->eval(env);
    std::string str = value_to_string(val);
    if (str.length() > 2 && str[0] == '(' && str.back() == ')') {
        size_t comma = str.find(", ");
        if (comma != std::string::npos) {
            std::string first_part = str.substr(1, comma - 1);
            try {
                return std::stoi(first_part);
            } catch (...) {
                return first_part;
            }
        }
    }
    
    return val;
}
std::string First::operand_str() const noexcept {
    return "fst";
}
std::string First::get_type() const noexcept {
    return "first";
}

Value_t Second::eval(Environment* env) noexcept {
    if (auto* pair = dynamic_cast<Pair*>(expression)) {
        return pair->second_expr->eval(env);
    }
    
    auto val = expression->eval(env);
    std::string str = value_to_string(val);
    if (str.length() > 2 && str[0] == '(' && str.back() == ')') {
        size_t comma = str.find(", ");
        if (comma != std::string::npos) {
            std::string second_part = str.substr(comma + 2, str.length() - comma - 3);
            try {
                return std::stoi(second_part);
            } catch (...) {
                return second_part;
            }
        }
    }
    
    return val;
}
std::string Second::operand_str() const noexcept {
    return "snd";
}
std::string Second::get_type() const noexcept {
    return "second";
}


Value_t Equal::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return left_val == right_val;
}
std::string Equal::operand_str() const noexcept {
    return " == ";
}
std::string Equal::get_type() const noexcept {
    return "equal";
}

Value_t NotEqual::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return left_val != right_val;
}
std::string NotEqual::operand_str() const noexcept {
    return " != ";
}
std::string NotEqual::get_type() const noexcept {
    return "not_equal";
}

Value_t LessThan::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> bool {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l < r;
        } else if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
            return l < r;
        }
        return false;
    }, left_val, right_val);
}
std::string LessThan::operand_str() const noexcept {
    return " < ";
}
std::string LessThan::get_type() const noexcept {
    return "less_than";
}

Value_t GreaterThan::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> bool {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l > r;
        } else if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
            return l > r;
        }
        return false;
    }, left_val, right_val);
}
std::string GreaterThan::operand_str() const noexcept {
    return " > ";
}
std::string GreaterThan::get_type() const noexcept {
    return "greater_than";
}

Value_t LessEqual::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> bool {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l <= r;
        } else if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
            return l <= r;
        }
        return false;
    }, left_val, right_val);
}
std::string LessEqual::operand_str() const noexcept {
    return " <= ";
}
std::string LessEqual::get_type() const noexcept {
    return "less_equal";
}

Value_t GreaterEqual::eval(Environment* env) noexcept {
    auto left_val = left_expression->eval(env);
    auto right_val = right_expression->eval(env);
    
    return std::visit([](const auto& l, const auto& r) -> bool {
        using L = std::decay_t<decltype(l)>;
        using R = std::decay_t<decltype(r)>;
        
        if constexpr (std::is_arithmetic_v<L> && std::is_arithmetic_v<R>) {
            return l >= r;
        } else if constexpr (std::is_same_v<L, std::string> && std::is_same_v<R, std::string>) {
            return l >= r;
        }
        return false;
    }, left_val, right_val);
}
std::string GreaterEqual::operand_str() const noexcept {
    return " >= ";
}
std::string GreaterEqual::get_type() const noexcept {
    return "greater_equal";
}


FunctionDefinition::FunctionDefinition(Expression* name, Expression* param, Expression* body) noexcept
    : function_name{name}, parameter{param}, function_body{body} {}
void FunctionDefinition::destroy() noexcept {}
Value_t FunctionDefinition::eval(Environment* env) noexcept {
    if (auto* id = dynamic_cast<Identifier*>(function_name)) {
        if (env) {
            env->bind_function(id->get_name(), this);
        }
    }
    
    return std::string("function_defined");
}
std::string FunctionDefinition::to_string() const noexcept {
    return "fun(" + function_name->to_string() + ", " + parameter->to_string() + ", " + function_body->to_string() + ")";
}
std::string FunctionDefinition::get_type() const noexcept {
    return "function_definition";
}

FunctionCall::FunctionCall(Expression* func, Expression* arg) noexcept
    : function{func}, argument{arg} {}
void FunctionCall::destroy() noexcept {
    if (function) {
        function->destroy();
        delete function;
        function = nullptr;
    }
    if (argument) {
        argument->destroy();
        delete argument;
        argument = nullptr;
    }
}
Value_t FunctionCall::eval(Environment* env) noexcept {
    if (auto* id = dynamic_cast<Identifier*>(function)) {
        if (env) {
            auto* func_def = env->lookup_function(id->get_name());
            if (func_def) {
                Environment* func_env = env->create_child();
                
                auto arg_val = argument->eval(env);
                if (auto* param_id = dynamic_cast<Identifier*>(func_def->parameter)) {
                    func_env->bind(param_id->get_name(), arg_val);
                }
                
                auto result = func_def->function_body->eval(func_env);
                delete func_env;
                return result;
            }
        }
    }
    
    return 0;
}
std::string FunctionCall::to_string() const noexcept {
    return function->to_string() + "(" + argument->to_string() + ")";
}
std::string FunctionCall::get_type() const noexcept {
    return "function_call";
}

