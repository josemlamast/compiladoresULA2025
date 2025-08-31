#pragma once
#include <variant>
#include <string>
#include <map>

class Environment;
using Value_t = std::variant<int, double, bool, std::string>;
class Expression
{
public:
    virtual ~Expression() = default;
    virtual void destroy() noexcept = 0;
    virtual Value_t eval(Environment* env = nullptr) noexcept = 0;
    virtual std::string to_string() const noexcept = 0;
    virtual std::string get_type() const noexcept = 0;
};

class UnaryOperation : public Expression
{
public:
    explicit UnaryOperation(Expression* expr) noexcept;
    void destroy() noexcept override;
    std::string to_string() const noexcept override;
    virtual std::string operand_str() const noexcept = 0;

protected:
    Expression* expression;
};

class BinaryOperation : public Expression
{
public:
    BinaryOperation(Expression* e1, Expression* e2) noexcept;
    void destroy() noexcept override;
    std::string to_string() const noexcept override;
    virtual std::string operand_str() const noexcept = 0;

protected:
    Expression* left_expression;
    Expression* right_expression;
};


class IntegerValue : public Expression
{
public:
    explicit IntegerValue(int val) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;

private:
    int value;
};

class RealValue : public Expression
{
public:
    explicit RealValue(double val) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;

private:
    double value;
};

class BooleanValue : public Expression
{
public:
    explicit BooleanValue(bool val) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;

private:
    bool value;
};

class StringValue : public Expression
{
public:
    explicit StringValue(const std::string& val) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;

private:
    std::string value;
};

class Identifier : public Expression
{
public:
    explicit Identifier(const std::string& name) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;
    const std::string& get_name() const noexcept;

private:
    std::string name;
};



class Addition : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string operand_str() const noexcept override;
    std::string get_type() const noexcept override;
};

class Subtraction : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string operand_str() const noexcept override;
    std::string get_type() const noexcept override;
};

class Multiplication : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string operand_str() const noexcept override;
    std::string get_type() const noexcept override;
};

class Division : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string operand_str() const noexcept override;
    std::string get_type() const noexcept override;
};

class Modulo : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string operand_str() const noexcept override;
    std::string get_type() const noexcept override;
};



class FunctionDefinition : public Expression
{
public:
    FunctionDefinition(Expression* name, Expression* param, Expression* body) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;

public:
    Expression* function_name;
    Expression* parameter;
    Expression* function_body;
};

class FunctionCall : public Expression
{
public:
    FunctionCall(Expression* func, Expression* arg) noexcept;
    void destroy() noexcept override;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string to_string() const noexcept override;
    std::string get_type() const noexcept override;

private:
    Expression* function;
    Expression* argument;
};

class PrintExpression : public UnaryOperation
{
public:
    using UnaryOperation::UnaryOperation;
    Value_t eval(Environment* env = nullptr) noexcept override;
    std::string operand_str() const noexcept override;
    std::string get_type() const noexcept override;
};

class Environment
{
public:
    Environment(Environment* parent = nullptr) noexcept;
    ~Environment() = default;
    void bind(const std::string& name, Value_t value) noexcept;
    void bind_function(const std::string& name, FunctionDefinition* func) noexcept;
    Value_t lookup(const std::string& name) const noexcept;
    FunctionDefinition* lookup_function(const std::string& name) const noexcept;
    bool exists(const std::string& name) const noexcept;
    Environment* create_child() noexcept;

private:
    std::map<std::string, Value_t> bindings;
    std::map<std::string, FunctionDefinition*> functions;
    Environment* parent_env;
};