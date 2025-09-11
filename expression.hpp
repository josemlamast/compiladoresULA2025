
#pragma once

#include "utils.hpp"




class NotExpression : public UnaryExpression
{
public:
     using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};




class ConcatExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class AndExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class XorExpression : public BinaryExpression {
public:
   using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class OrExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};


class LessExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class LessEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class GreaterExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class GreaterEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class EqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class NotEqualExpression : public BinaryExpression
{
public:
   using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};


class AddExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class SubExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class MulExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class DivExpression : public BinaryExpression
{
public:
   using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class ModExpression : public BinaryExpression
{
public:
  using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};




class CallExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};



class AssignmentExpression : public BinaryExpression
{
public:
   using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class FunExpression  : public UnaryExpression
{
public:
    FunExpression(std::shared_ptr<Expression> _function_name_expression, std::shared_ptr<Expression> _parameter_name_expression, std::shared_ptr<Expression> _body_expression) noexcept;

    std::shared_ptr<Expression> get_function_name_expression() const noexcept;

    std::shared_ptr<Expression> get_parameter_name_expression() const noexcept;

    const std::string& get_name() const noexcept;

    const std::string& get_parameter_name() const noexcept;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;

    std::shared_ptr<Expression> get_body_expression() const noexcept;

private:
    std::shared_ptr<Expression> function_name_expression;
    std::shared_ptr<Expression> parameter_name_expression;
};

class LetExpression : public Expression
{
public:
    LetExpression(std::shared_ptr<Expression> _var_name, std::shared_ptr<Expression> _var_expression, std::shared_ptr<Expression> _body_expression) noexcept;

    std::shared_ptr<Expression> get_var_name() const noexcept;
    std::shared_ptr<Expression> get_var_expression() const noexcept;
    std::shared_ptr<Expression> get_body_expression() const noexcept;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;

private:
    std::shared_ptr<Expression> var_name;
    std::shared_ptr<Expression> var_expression;
    std::shared_ptr<Expression> body_expression;
};

class NameExpression : public Expression
{
public:
    NameExpression(const std::string& _name) noexcept;

    const std::string& get_name() const noexcept;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;

private:
    std::string name;
};

class RealExpression : public Expression{
public:
     RealExpression(double _value) noexcept;

    double get_value() const noexcept;

    std::shared_ptr<Expression> eval(Environment&) const override;

    std::string to_string() const noexcept override;
    
    
private:
    double value;
};

class IntExpression : public Expression
{
public:
    IntExpression(int _value) noexcept;

    int get_value() const noexcept;

    std::shared_ptr<Expression> eval(Environment&) const override;

    std::string to_string() const noexcept override;

private:
    int value;
};

class BoolExpression : public Expression {
public:
     BoolExpression(bool _value) noexcept;

    bool get_value() const noexcept;

    std::shared_ptr<Expression> eval(Environment&) const override;

    std::string to_string() const noexcept override;

    
private:
    bool value;
};

class StrExpression : public Expression
{
public:
     StrExpression(const std::string& _value) noexcept;

    const std::string& get_value() const noexcept;

    std::shared_ptr<Expression> eval(Environment&) const override;

    std::string to_string() const noexcept override;

private:
    std::string value;
};

class PairExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class NegExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};


class FstExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class SndExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class HeadExpression : public UnaryExpression {
public:
   using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class TailExpression : public UnaryExpression {
public:
     using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};


class RtoSExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class ItoSExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class ItoRExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class RtoIExpression : public UnaryExpression {
public:
     using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};


class ArrayAddExpression : public BinaryExpression {
public:
   using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class ArrayDelExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};

class IfElseExpression : public Expression {
public:
    IfElseExpression(std::shared_ptr<Expression> _condition_expression, std::shared_ptr<Expression> _true_expression, std::shared_ptr<Expression> _false_expression) noexcept;

    std::shared_ptr<Expression> get_condition_expression() const noexcept;
    
    std::shared_ptr<Expression> get_true_expression() const noexcept;

    std::shared_ptr<Expression> get_false_expression() const noexcept;
        
    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;

private:
    std::shared_ptr<Expression> condition_expression;
    std::shared_ptr<Expression> true_expression;
    std::shared_ptr<Expression> false_expression;
};

class PrintExpression : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    std::shared_ptr<Expression> eval(Environment& env) const override;

    std::string to_string() const noexcept override;
};
