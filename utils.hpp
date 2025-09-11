#pragma once

#include <algorithm>
#include <forward_list>
#include <memory>
#include <string>

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__); printf("\n");
#else
#define LOG(...)
#endif

class Environment;

class Expression
{
public:
    virtual ~Expression();

    virtual std::shared_ptr<Expression> eval(Environment&) const = 0;

    virtual std::string to_string() const noexcept = 0;
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



class Closure : public UnaryExpression
{
public:
    Closure(const Environment& _env, std::shared_ptr<Expression> _function) noexcept;

    const Environment& get_environment() const noexcept;

    std::shared_ptr<Expression> get_function_expression() const noexcept;

    std::shared_ptr<Expression> eval(Environment&) const override;

    std::string to_string() const noexcept override;

private:
    Environment env;
};