#include <datatype.hpp>
#include <expression.hpp>
#include <symbol_table.hpp>

UnaryExpression::UnaryExpression(Expression* expr) noexcept
    : expression{expr} {}

void UnaryExpression::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
    this->expression = nullptr;
}

bool UnaryExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<UnaryExpression*>(other);
    return other_expr != nullptr && this->expression->equal(other_expr->expression);
}

bool UnaryExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->expression->resolve_name(symbol_table);
}

Expression* UnaryExpression::get_expression() const noexcept
{
    return this->expression;
}


ASTNodeInterface* NotExpression::copy() const noexcept
{
    return new NotExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool NotExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<NotExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> NotExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    bool result = expr_type.first && expr_type.second->is<BooleanDatatype>();
    expr_type.second->destroy();
    delete expr_type.second;
    expr_type.second = nullptr;

    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}


////////////////////////////////////////////////////////////
ASTNodeInterface* IncrementExpression::copy() const noexcept
{
    return new IncrementExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool IncrementExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<IncrementExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> IncrementExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    bool result = expr_type.second->is<IntegerDatatype>();
    expr_type.second->destroy();
    delete expr_type.second;
    expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new IntegerDatatype{});
}


ASTNodeInterface* DecrementExpression::copy() const noexcept
{
    return new DecrementExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool DecrementExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<DecrementExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> DecrementExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    bool result = expr_type.second->is<IntegerDatatype>();
    expr_type.second->destroy();
    delete expr_type.second;
    expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new IntegerDatatype{});
}

////////////////////////////////////////////////////////////

BinaryExpression::BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept
    : left_expression{left_expr}, right_expression{right_expr} {}

void BinaryExpression::destroy() noexcept
{
    this->left_expression->destroy();
    delete this->left_expression;
    this->left_expression = nullptr;

    if (this->right_expression != nullptr)
    {
        this->right_expression->destroy();
        delete this->right_expression;
        this->right_expression = nullptr;
    }
}

bool BinaryExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<BinaryExpression*>(other);
    auto res = other_expr!= nullptr && this->left_expression->equal(other_expr->left_expression);

    if (res && this->right_expression == nullptr)
    {
        return other_expr->right_expression == nullptr;
    }

    return res && this->right_expression->equal(other_expr->right_expression);
}

bool BinaryExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool left_result = this->left_expression->resolve_name(symbol_table);

    if (left_result && this->right_expression != nullptr)
    {
        return this->right_expression->resolve_name(symbol_table);
    }

    return left_result;
}

std::pair<bool, Datatype*> BinaryExpression::boolean_operation_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();
    bool result = left_expr_type.first &&
        right_expr_type.first &&
        left_expr_type.second->is<BooleanDatatype>() &&
        right_expr_type.second->is<BooleanDatatype>();

    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}

std::pair<bool, Datatype*> BinaryExpression::comparison_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();

    if (!left_expr_type.first || !right_expr_type.first)
    {
        if (left_expr_type.second != nullptr)
        {
            left_expr_type.second->destroy();
            delete left_expr_type.second;
            left_expr_type.second = nullptr;
        }

        if (right_expr_type.second != nullptr)
        {
            right_expr_type.second->destroy();
            delete right_expr_type.second;
            right_expr_type.second = nullptr;
        }

        return std::make_pair(false, nullptr);
    }

    if (left_expr_type.second->is<VoidDatatype>() || left_expr_type.second->is<ArrayDatatype>() ||
        left_expr_type.second->is<FunctionDatatype>() || right_expr_type.second->is<VoidDatatype>() ||
        right_expr_type.second->is<ArrayDatatype>() || right_expr_type.second->is<FunctionDatatype>())
    {
        left_expr_type.second->destroy();
        delete left_expr_type.second;
        left_expr_type.second = nullptr;

        right_expr_type.second->destroy();
        delete right_expr_type.second;
        right_expr_type.second = nullptr;

        return std::make_pair(false, nullptr);    
    }

    bool result = left_expr_type.second->equal(right_expr_type.second);
    
    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}

std::pair<bool, Datatype*> BinaryExpression::arithmetic_operation_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();
    bool result = left_expr_type.first &&
        right_expr_type.first &&
        left_expr_type.second->is<IntegerDatatype>() &&
        right_expr_type.second->is<IntegerDatatype>();

    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new IntegerDatatype{});
}

Expression* BinaryExpression::get_left_expression() const noexcept
{
    return this->left_expression;
}

Expression* BinaryExpression::get_right_expression() const noexcept
{
    return this->right_expression;
}

ASTNodeInterface* AndExpression::copy() const noexcept
{
    return new AndExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AndExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AndExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> AndExpression::type_check() const noexcept
{
    return this->boolean_operation_type_check();
}


ASTNodeInterface* XorExpression::copy() const noexcept {
    return new XorExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool XorExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<XorExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> XorExpression::type_check() const noexcept {
    return this->boolean_operation_type_check();
}

ASTNodeInterface* OrExpression::copy() const noexcept
{
    return new OrExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool OrExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<OrExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> OrExpression::type_check() const noexcept
{
    return this->boolean_operation_type_check();
}

ASTNodeInterface* LessExpression::copy() const noexcept
{
    return new LessExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool LessExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<LessExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> LessExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* LessEqExpression::copy() const noexcept
{
    return new LessEqExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool LessEqExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<LessEqExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> LessEqExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* GreaterExpression::copy() const noexcept
{
    return new GreaterExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool GreaterExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<GreaterExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> GreaterExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* GreaterEqExpression::copy() const noexcept
{
    return new GreaterEqExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool GreaterEqExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<GreaterEqExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> GreaterEqExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* EqualExpression::copy() const noexcept
{
    return new EqualExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool EqualExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<EqualExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> EqualExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* NotEqualExpression::copy() const noexcept
{
    return new NotEqualExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool NotEqualExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<NotEqualExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> NotEqualExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* AddExpression::copy() const noexcept
{
    return new AddExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AddExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AddExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> AddExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* SubExpression::copy() const noexcept
{
    return new SubExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool SubExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<SubExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> SubExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* MulExpression::copy() const noexcept
{
    return new MulExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool MulExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<MulExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> MulExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* DivExpression::copy() const noexcept
{
    return new DivExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

std::pair<bool, Datatype*> DivExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

bool DivExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<DivExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

ASTNodeInterface* ModExpression::copy() const noexcept
{
    return new ModExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool ModExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ModExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> ModExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* ArgExpression::copy() const noexcept
{
    Expression* left_copy = dynamic_cast<Expression*>(this->left_expression->copy());
    Expression* right_copy = nullptr;

    if (this->right_expression != nullptr)
    {
        right_copy = dynamic_cast<Expression*>(this->right_expression->copy());
    }

    return new ArgExpression{left_copy, right_copy};
}

bool ArgExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ArgExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> ArgExpression::type_check() const noexcept
{
    return this->left_expression->type_check();
}

ASTNodeInterface* CallExpression::copy() const noexcept
{
    Expression* left_copy = dynamic_cast<Expression*>(this->left_expression->copy());
    Expression* right_copy = nullptr;

    if (this->right_expression != nullptr)
    {
        right_copy = dynamic_cast<Expression*>(this->right_expression->copy());
    }

    return new CallExpression{left_copy, right_copy};
}

bool CallExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<CallExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> CallExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();

    if (!left_type.first || !left_type.second->is<FunctionDatatype>())
    {
        if (left_type.second != nullptr)
        {
            left_type.second->destroy();
            delete left_type.second;
            left_type.second = nullptr;
        }
        return std::make_pair(false, nullptr);
    }

    auto fct_type = dynamic_cast<FunctionDatatype*>(left_type.second);
    auto arg_expr = dynamic_cast<ArgExpression*>(this->right_expression);

    for (const auto& param: fct_type->get_parameters())
    {
        if (arg_expr == nullptr)
        {
            left_type.second->destroy();
            delete left_type.second;
            left_type.second = nullptr;
            return std::make_pair(false, nullptr);
        }
    
        auto arg_type = arg_expr->get_left_expression()->type_check();

        if (!arg_type.second->equal(param.second))
        {
            arg_type.second->destroy();
            delete arg_type.second;
            arg_type.second = nullptr;

            left_type.second->destroy();
            delete left_type.second;
            left_type.second = nullptr;

            return std::make_pair(false, nullptr);
        }

        arg_type.second->destroy();
        delete arg_type.second;
        arg_type.second = nullptr;

        arg_expr = dynamic_cast<ArgExpression*>(arg_expr->get_right_expression());
    }

    auto result = dynamic_cast<Datatype*>(fct_type->get_return_type()->copy());

    left_type.second->destroy();
    delete left_type.second;
    left_type.second = nullptr;

    if (arg_expr != nullptr)
    {
        result->destroy();
        delete result;
        result = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, result);    
}

ASTNodeInterface* SubscriptExpression::copy() const noexcept
{
    return new SubscriptExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool SubscriptExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<SubscriptExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> SubscriptExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();

    if (!left_type.second->is<ArrayDatatype>() || !right_type.second->is<IntegerDatatype>())
    {
        left_type.second->destroy();
        delete left_type.second;
        left_type.second = nullptr;
        
        right_type.second->destroy();
        delete right_type.second;
        right_type.second = nullptr;

        delete right_type.second;
        return std::make_pair(false, nullptr);
    }

    auto result_type = dynamic_cast<Datatype*>(dynamic_cast<ArrayDatatype*>(left_type.second)->get_inner_datatype()->copy());

    left_type.second->destroy();
    delete left_type.second;
    left_type.second = nullptr;
    
    right_type.second->destroy();
    delete right_type.second;
    right_type.second = nullptr;

    return std::make_pair(true, result_type);
}

ASTNodeInterface* AssignmentExpression::copy() const noexcept
{
    return new AssignmentExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AssignmentExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AssignmentExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other_expr);
}

std::pair<bool, Datatype*> AssignmentExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();
    bool result = left_type.first &&
        right_type.first &&
        left_type.second->equal(right_type.second);

    right_type.second->destroy();
    delete right_type.second;
    right_type.second = nullptr;

    if (!result)
    {
        left_type.second->destroy();
        delete left_type.second;
        left_type.second = nullptr;
        left_type.first = false;
    }

    return left_type;
}

void LeafExpression::destroy() noexcept {}

bool LeafExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return true;
}

NameExpression::NameExpression(std::string_view _name) noexcept
    : name{_name} {}

ASTNodeInterface* NameExpression::copy() const noexcept
{
    return new NameExpression{this->name};
}

bool NameExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<NameExpression*>(other);
    return other_expr != nullptr && this->name == other_expr->name;
}

bool NameExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = symbol_table.lookup(this->name);
    return this->symbol != nullptr;
}

std::pair<bool, Datatype*> NameExpression::type_check() const noexcept
{
    if (symbol == nullptr)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, dynamic_cast<Datatype*>(this->symbol->type->copy()));    
}


RealExpression::RealExpression(double _value) noexcept
    : value{_value} {}

ASTNodeInterface* RealExpression::copy() const noexcept
{
    return new RealExpression{this->value};
}

bool RealExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<RealExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> RealExpression::type_check() const noexcept
{
    return std::make_pair(true, new RealDatatype{});
}


IntExpression::IntExpression(int _value) noexcept
    : value{_value} {}

ASTNodeInterface* IntExpression::copy() const noexcept
{
    return new IntExpression{this->value};
}

bool IntExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<IntExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> IntExpression::type_check() const noexcept
{
    return std::make_pair(true, new IntegerDatatype{});
}


BoolExpression::BoolExpression(bool _value) noexcept  : value{_value} {}

ASTNodeInterface* BoolExpression::copy() const noexcept
{
    return new BoolExpression{this->value};
}

bool BoolExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<BoolExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> BoolExpression::type_check() const noexcept
{
    return std::make_pair(true, new BooleanDatatype{});
}



StrExpression::StrExpression(std::string_view _value) noexcept
    : value{_value} {}

ASTNodeInterface* StrExpression::copy() const noexcept
{
    return new StrExpression{this->value};
}

bool StrExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<StrExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> StrExpression::type_check() const noexcept
{
    return std::make_pair(true, new StringDatatype{});    
}


ASTNodeInterface* ConcatExpression::copy() const noexcept {
    return new ConcatExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool ConcatExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<ConcatExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> ConcatExpression::type_check() const noexcept {
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();


    bool result = left_type.first && right_type.first &&
                 left_type.second->is<StringDatatype>() && 
                 right_type.second->is<StringDatatype>();
    
    left_type.second->destroy();
    delete left_type.second;
    right_type.second->destroy();
    delete right_type.second;
    
    if (!result) {
        return std::make_pair(false, nullptr);
    }    
    return std::make_pair(true, new StringDatatype{});
}


ASTNodeInterface* NegExpression::copy() const noexcept {
    return new NegExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool NegExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<NegExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> NegExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    bool result = expr_type.first && expr_type.second->is<RealDatatype>();
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    if (!result) {
        return std::make_pair(false, nullptr);
    }
    
    return std::make_pair(true, new RealDatatype{});
}


ASTNodeInterface* PrintExpression::copy() const noexcept {
    return new PrintExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool PrintExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<PrintExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> PrintExpression::type_check() const noexcept {
    // Print can accept any type, so we just check that the expression is valid
    auto expr_type = this->expression->type_check();
    bool result = expr_type.first;
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, new VoidDatatype());
}



ASTNodeInterface* FstExpression::copy() const noexcept {
    return new FstExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool FstExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<FstExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> FstExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is a pair type
    // For now, we'll assume it returns the first element's type
    // You'll need to implement proper pair type checking
    
    bool result = expr_type.first;
    Datatype* return_type = nullptr;
    
    if (result) {
        // This is a placeholder - you need to implement proper pair type checking
        return_type = new IntegerDatatype(); // Default return type
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}


ASTNodeInterface* SndExpression::copy() const noexcept {
    return new SndExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool SndExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<SndExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> SndExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    bool result = expr_type.first;
    Datatype* return_type = nullptr;
    
    if (result) {
        // This is a placeholder - you need to implement proper pair type checking
        return_type = new IntegerDatatype(); // Default return type
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}


ASTNodeInterface* HeadExpression::copy() const noexcept {
    return new HeadExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool HeadExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<HeadExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> HeadExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is an array type
    bool result = expr_type.first && expr_type.second->is<HeadExpression>();
    Datatype* return_type = nullptr;
    
    if (result) {
        auto array_type = dynamic_cast<ArrayDatatype*>(expr_type.second);
        return_type = dynamic_cast<Datatype*>(array_type->get_inner_datatype()->copy());
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}


ASTNodeInterface* TailExpression::copy() const noexcept {
    return new TailExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool TailExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<TailExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> TailExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is an array type
    bool result = expr_type.first && expr_type.second->is<TailExpression>();
    Datatype* return_type = nullptr;
    
    if (result) {
        // Tail returns the same array type without the first element
        return_type = dynamic_cast<Datatype*>(expr_type.second->copy());
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}



ASTNodeInterface* RtoSExpression::copy() const noexcept {
    return new RtoSExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool RtoSExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<RtoSExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> RtoSExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is an array type
    bool result = expr_type.first && expr_type.second->is<RealDatatype>();
    Datatype* return_type = nullptr;
    
    if (result) {
        // Tail returns the same array type without the first element
        return_type = dynamic_cast<Datatype*>(expr_type.second->copy());
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}


ASTNodeInterface* ItoSExpression::copy() const noexcept {
    return new ItoSExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool ItoSExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<ItoSExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> ItoSExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is an array type
    bool result = expr_type.first && expr_type.second->is<IntegerDatatype>();
    Datatype* return_type = nullptr;
    
    if (result) {
        // Tail returns the same array type without the first element
        return_type = dynamic_cast<Datatype*>(expr_type.second->copy());
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}


ASTNodeInterface* ItoRExpression::copy() const noexcept {
    return new ItoRExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool ItoRExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<ItoRExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> ItoRExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is an array type
    bool result = expr_type.first && expr_type.second->is<IntegerDatatype>();
    Datatype* return_type = nullptr;
    
    if (result) {
        // Tail returns the same array type without the first element
        return_type = dynamic_cast<Datatype*>(expr_type.second->copy());
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}


ASTNodeInterface* RtoIExpression::copy() const noexcept {
    return new ItoSExpression(dynamic_cast<Expression*>(this->expression->copy()));
}

bool RtoIExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<RtoIExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> RtoIExpression::type_check() const noexcept {
    auto expr_type = this->expression->type_check();
    
    // Check if the expression is an array type
    bool result = expr_type.first && expr_type.second->is<RealDatatype>();
    Datatype* return_type = nullptr;
    
    if (result) {
        // Tail returns the same array type without the first element
        return_type = dynamic_cast<Datatype*>(expr_type.second->copy());
    }
    
    if (expr_type.second != nullptr) {
        expr_type.second->destroy();
        delete expr_type.second;
    }
    
    return std::make_pair(result, return_type);
}






ASTNodeInterface* ArrayAddExpression::copy() const noexcept {
    return new ArrayAddExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

std::pair<bool, Datatype*> ArrayAddExpression::type_check() const noexcept {
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();

    // Verificar que el izquierdo sea un array y el derecho sea del tipo interno del array
    bool result = left_type.first && right_type.first &&
                 left_type.second->is<ArrayDatatype>() &&
                 dynamic_cast<ArrayDatatype*>(left_type.second)->get_inner_datatype()->equal(right_type.second);

    if (result) {
        // Retornar el mismo tipo de array
        Datatype* return_type = dynamic_cast<Datatype*>(left_type.second->copy());
        left_type.second->destroy();
        delete left_type.second;
        right_type.second->destroy();
        delete right_type.second;
        return std::make_pair(true, return_type);
    }

    // Limpiar memoria en caso de error
    if (left_type.second != nullptr) {
        left_type.second->destroy();
        delete left_type.second;
    }
    if (right_type.second != nullptr) {
        right_type.second->destroy();
        delete right_type.second;
    }
    return std::make_pair(false, nullptr);
}

bool ArrayAddExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<ArrayAddExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

ASTNodeInterface* ArrayDelExpression::copy() const noexcept {
    return new ArrayDelExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

std::pair<bool, Datatype*> ArrayDelExpression::type_check() const noexcept {
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();

    // Verificar que el izquierdo sea un array y el derecho sea entero (índice)
    bool result = left_type.first && right_type.first &&
                 left_type.second->is<ArrayDatatype>() &&
                 right_type.second->is<IntegerDatatype>();

    if (result) {
        // Retornar el mismo tipo de array
        Datatype* return_type = dynamic_cast<Datatype*>(left_type.second->copy());
        left_type.second->destroy();
        delete left_type.second;
        right_type.second->destroy();
        delete right_type.second;
        return std::make_pair(true, return_type);
    }

    // Limpiar memoria en caso de error
    if (left_type.second != nullptr) {
        left_type.second->destroy();
        delete left_type.second;
    }
    if (right_type.second != nullptr) {
        right_type.second->destroy();
        delete right_type.second;
    }
    return std::make_pair(false, nullptr);
}

bool ArrayDelExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<ArrayDelExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}


IfElseExpression::IfElseExpression(Expression* cond, Expression* true_expr, Expression* false_expr) noexcept
    : condition{cond}, true_expression{true_expr}, false_expression{false_expr} {}

void IfElseExpression::destroy() noexcept {
    this->condition->destroy();
    delete this->condition;
    this->condition = nullptr;

    this->true_expression->destroy();
    delete this->true_expression;
    this->true_expression = nullptr;

    if (this->false_expression != nullptr) {
        this->false_expression->destroy();
        delete this->false_expression;
        this->false_expression = nullptr;
    }
}

ASTNodeInterface* IfElseExpression::copy() const noexcept {
    return new IfElseExpression{
        dynamic_cast<Expression*>(this->condition->copy()),
        dynamic_cast<Expression*>(this->true_expression->copy()),
        this->false_expression == nullptr ? nullptr : dynamic_cast<Expression*>(this->false_expression->copy())
    };
}

bool IfElseExpression::equal(ASTNodeInterface* other) const noexcept {
    auto other_expr = dynamic_cast<IfElseExpression*>(other);
    if (other_expr == nullptr) return false;
    
    bool cond_equal = this->condition->equal(other_expr->condition);
    bool true_equal = this->true_expression->equal(other_expr->true_expression);
    bool false_equal = (this->false_expression == nullptr && other_expr->false_expression == nullptr) ||
                       (this->false_expression != nullptr && other_expr->false_expression != nullptr &&
                        this->false_expression->equal(other_expr->false_expression));
    
    return cond_equal && true_equal && false_equal;
}

std::pair<bool, Datatype*> IfElseExpression::type_check() const noexcept {
    auto cond_type = this->condition->type_check();
    if (!cond_type.first || !cond_type.second->is<BooleanDatatype>()) {
        if (cond_type.second != nullptr) {
            cond_type.second->destroy();
            delete cond_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    cond_type.second->destroy();
    delete cond_type.second;

    auto true_type = this->true_expression->type_check();
    if (!true_type.first) {
        return std::make_pair(false, nullptr);
    }

    if (this->false_expression == nullptr) {
        // If without else returns void
        if (true_type.second != nullptr) {
            true_type.second->destroy();
            delete true_type.second;
        }
        return std::make_pair(true, new VoidDatatype());
    }

    auto false_type = this->false_expression->type_check();
    if (!false_type.first) {
        if (true_type.second != nullptr) {
            true_type.second->destroy();
            delete true_type.second;
        }
        return std::make_pair(false, nullptr);
    }

    // Both branches should have the same type
    if (!true_type.second->equal(false_type.second)) {
        true_type.second->destroy();
        delete true_type.second;
        false_type.second->destroy();
        delete false_type.second;
        return std::make_pair(false, nullptr);
    }

    false_type.second->destroy();
    delete false_type.second;
    return std::make_pair(true, true_type.second);
}

bool IfElseExpression::resolve_name(SymbolTable& symbol_table) noexcept {
    bool cond_res = this->condition->resolve_name(symbol_table);
    bool true_res = this->true_expression->resolve_name(symbol_table);
    bool false_res = this->false_expression == nullptr ? true : this->false_expression->resolve_name(symbol_table);
    return cond_res && true_res && false_res;
}