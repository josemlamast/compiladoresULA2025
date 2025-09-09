#pragma once

#include <memory>

#include <ast_node_interface.hpp>

class SymbolTable;

class Expression : public ASTNodeInterface
{
protected:
    std::shared_ptr<Symbol> symbol{nullptr};
};

class UnaryExpression : public Expression
{
public:
    UnaryExpression(Expression* expr) noexcept;

    void destroy() noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    Expression* get_expression() const noexcept;

protected:
    Expression* expression;
};

class NotExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};



class BinaryExpression : public Expression
{
public:
    BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept;

    void destroy() noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;
    
    std::pair<bool, Datatype*> boolean_operation_type_check() const noexcept;

    std::pair<bool, Datatype*> comparison_type_check() const noexcept;

    std::pair<bool, Datatype*> arithmetic_operation_type_check() const noexcept;

    Expression* get_left_expression() const noexcept;

    Expression* get_right_expression() const noexcept;

protected:
    Expression* left_expression;
    Expression* right_expression;
};

class AndExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class OrExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class LessExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class LessEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class GreaterExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class GreaterEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class EqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class NotEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class AddExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class SubExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class MulExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class DivExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class ModExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class ArgExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class CallExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class SubscriptExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};


class LeafExpression : public Expression
{
public:
    void destroy() noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;
};

class NameExpression : public LeafExpression
{
public:
    NameExpression(std::string_view _name) noexcept;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    std::string name;
    std::shared_ptr<Symbol> symbol;
};

class IntExpression : public LeafExpression
{
public:
    IntExpression(int _value) noexcept;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    int value;
};

class StrExpression : public LeafExpression
{
public:
    StrExpression(std::string_view _value) noexcept;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    std::string value;
};

class RealExpression : public LeafExpression
{
public:
    RealExpression(double _value) noexcept;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    double value;
};

class BooleanExpression : public LeafExpression
{
public:
    BooleanExpression(bool _value) noexcept;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    bool value;
};

class ArrayExpression : public Expression
{
public:
    ArrayExpression(const std::vector<Expression*>& _elements) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    std::vector<Expression*> elements;
};

class PairExpression : public Expression
{
public:
    PairExpression(Expression* _first, Expression* _second) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

private:
    Expression* first;
    Expression* second;
};

class ConcatExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class XorExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class HeadExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class TailExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class AddArrayExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class DelArrayExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class FstExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class SndExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class PrintExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class ItosExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class RtosExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class ItorExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class RtoiExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class LetExpression : public Expression
{
public:
    LetExpression(const std::string& _name, Expression* _value, Expression* _body) noexcept;
    ~LetExpression() noexcept override;
    void destroy() noexcept override;
    
    ASTNodeInterface* copy() const noexcept override;
    bool equal(ASTNodeInterface* other) const noexcept override;
    bool resolve_name(SymbolTable& symbol_table) noexcept override;
    std::pair<bool, Datatype*> type_check() const noexcept override;
    
private:
    std::string name;
    Expression* value;
    Expression* body;
};

class FunctionExpression : public Expression
{
public:
    FunctionExpression(const std::string& _name, const std::string& _param, Expression* _body) noexcept;
    ~FunctionExpression() noexcept override;
    void destroy() noexcept override;
    
    ASTNodeInterface* copy() const noexcept override;
    bool equal(ASTNodeInterface* other) const noexcept override;
    bool resolve_name(SymbolTable& symbol_table) noexcept override;
    std::pair<bool, Datatype*> type_check() const noexcept override;
    
private:
    std::string name;
    std::string param;
    Expression* body;
};