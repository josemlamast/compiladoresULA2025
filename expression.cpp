#include <expression.hpp>

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

Identifier::Identifier(const std::string& name) noexcept : name{name} {}
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
