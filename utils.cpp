#include <sstream>
#include <utils.hpp>
#include "expression.hpp"

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

Closure::Closure(const Environment& _env, std::shared_ptr<Expression> _function, 
                 Datatype _param_type, Datatype _return_type) noexcept
    : UnaryExpression{_function}, env{_env}, parameter_type{_param_type}, return_type{_return_type}
{
    // empty
}

const Environment& Closure::get_environment() const noexcept
{
    return env;
}

std::shared_ptr<Expression> Closure::get_function_expression() const noexcept
{
    return UnaryExpression::get_expression();
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
    return std::make_shared<Closure>(env, UnaryExpression::get_expression(), parameter_type, return_type);
}

std::string Closure::to_string() const noexcept
{
    return "(closure" 
        + env.to_string()
        + UnaryExpression::get_expression()->to_string() + ")";
}

std::pair<bool, Datatype> Closure::type_check(Environment&) const noexcept
{
    // Un closure es una función, devolvemos FunctionType
    return {true, Datatype::FunctionType};
}
