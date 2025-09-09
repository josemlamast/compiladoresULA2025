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
    bool right_result = true;
    
    if (this->right_expression != nullptr)
    {
        right_result = this->right_expression->resolve_name(symbol_table);
    }

    return left_result && right_result;
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
    
    if (!left_expr_type.first || !right_expr_type.first)
    {
        if (left_expr_type.second != nullptr)
        {
            left_expr_type.second->destroy();
            delete left_expr_type.second;
        }
        if (right_expr_type.second != nullptr)
        {
            right_expr_type.second->destroy();
            delete right_expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    // Verificar si ambos son enteros
    bool both_integers = left_expr_type.second->is<IntegerDatatype>() &&
                        right_expr_type.second->is<IntegerDatatype>();
    
    // Verificar si ambos son reales
    bool both_reals = left_expr_type.second->is<RealDatatype>() &&
                     right_expr_type.second->is<RealDatatype>();
    
    // Verificar si uno es entero y otro real
    bool mixed_types = (left_expr_type.second->is<IntegerDatatype>() && right_expr_type.second->is<RealDatatype>()) ||
                      (left_expr_type.second->is<RealDatatype>() && right_expr_type.second->is<IntegerDatatype>());

    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!both_integers && !both_reals && !mixed_types)
    {
        return std::make_pair(false, nullptr);
    }

    // Si hay tipos mixtos o ambos son reales, retornar real
    if (both_reals || mixed_types)
    {
        return std::make_pair(true, new RealDatatype{});
    }
    
    // Si ambos son enteros, retornar entero
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

bool ArgExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    // Resolver el argumento izquierdo
    bool left_result = this->left_expression->resolve_name(symbol_table);
    if (!left_result) {
        return false;
    }
    
    // Resolver el argumento derecho si existe
    if (this->right_expression != nullptr) {
        return this->right_expression->resolve_name(symbol_table);
    }
    
    return true;
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

bool CallExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    // Resolver el nombre de la función
    bool left_result = this->left_expression->resolve_name(symbol_table);
    if (!left_result) {
        return false;
    }
    
    // Resolver los argumentos si existen
    if (this->right_expression != nullptr) {
        return this->right_expression->resolve_name(symbol_table);
    }
    
    return true;
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


void LeafExpression::destroy() noexcept {}

bool LeafExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return true;
}

NameExpression::NameExpression(std::string_view _name) noexcept
    : name{_name}, symbol(nullptr) {}

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
    printf("DEBUG: NameExpression::resolve_name for '%s'\n", name.c_str());
    this->symbol = symbol_table.lookup(this->name);
    printf("DEBUG: Variable '%s' lookup: %s\n", name.c_str(), this->symbol != nullptr ? "FOUND" : "NOT FOUND");
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

BooleanExpression::BooleanExpression(bool _value) noexcept
    : value{_value} {}

ASTNodeInterface* BooleanExpression::copy() const noexcept
{
    return new BooleanExpression{this->value};
}

bool BooleanExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<BooleanExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> BooleanExpression::type_check() const noexcept
{
    return std::make_pair(true, new BooleanDatatype{});
}

ArrayExpression::ArrayExpression(const std::vector<Expression*>& _elements) noexcept
    : elements{_elements} {}

void ArrayExpression::destroy() noexcept
{
    for (auto element : this->elements)
    {
        element->destroy();
        delete element;
    }
    this->elements.clear();
}

ASTNodeInterface* ArrayExpression::copy() const noexcept
{
    std::vector<Expression*> copied_elements;
    for (auto element : this->elements)
    {
        copied_elements.push_back(dynamic_cast<Expression*>(element->copy()));
    }
    return new ArrayExpression{copied_elements};
}

bool ArrayExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ArrayExpression*>(other);
    if (other_expr == nullptr || this->elements.size() != other_expr->elements.size())
    {
        return false;
    }
    
    for (size_t i = 0; i < this->elements.size(); ++i)
    {
        if (!this->elements[i]->equal(other_expr->elements[i]))
        {
            return false;
        }
    }
    return true;
}

bool ArrayExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    for (auto element : this->elements)
    {
        if (!element->resolve_name(symbol_table))
        {
            return false;
        }
    }
    return true;
}

std::pair<bool, Datatype*> ArrayExpression::type_check() const noexcept
{
    if (this->elements.empty())
    {
        return std::make_pair(true, new ArrayDatatype{new VoidDatatype{}});
    }
    
    auto first_type = this->elements[0]->type_check();
    if (!first_type.first)
    {
        return std::make_pair(false, nullptr);
    }
    
    for (size_t i = 1; i < this->elements.size(); ++i)
    {
        auto element_type = this->elements[i]->type_check();
        if (!element_type.first || !first_type.second->equal(element_type.second))
        {
            if (element_type.second != nullptr)
            {
                element_type.second->destroy();
                delete element_type.second;
            }
            first_type.second->destroy();
            delete first_type.second;
            return std::make_pair(false, nullptr);
        }
        element_type.second->destroy();
        delete element_type.second;
    }
    
    return std::make_pair(true, new ArrayDatatype{first_type.second});
}

PairExpression::PairExpression(Expression* _first, Expression* _second) noexcept
    : first{_first}, second{_second} {}

void PairExpression::destroy() noexcept
{
    this->first->destroy();
    delete this->first;
    this->first = nullptr;
    
    this->second->destroy();
    delete this->second;
    this->second = nullptr;
}

ASTNodeInterface* PairExpression::copy() const noexcept
{
    return new PairExpression{
        dynamic_cast<Expression*>(this->first->copy()),
        dynamic_cast<Expression*>(this->second->copy())
    };
}

bool PairExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<PairExpression*>(other);
    return other_expr != nullptr && 
           this->first->equal(other_expr->first) &&
           this->second->equal(other_expr->second);
}

bool PairExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->first->resolve_name(symbol_table) && 
           this->second->resolve_name(symbol_table);
}

std::pair<bool, Datatype*> PairExpression::type_check() const noexcept
{
    auto first_type = this->first->type_check();
    auto second_type = this->second->type_check();
    
    if (!first_type.first || !second_type.first)
    {
        if (first_type.second != nullptr)
        {
            first_type.second->destroy();
            delete first_type.second;
        }
        if (second_type.second != nullptr)
        {
            second_type.second->destroy();
            delete second_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    // Crear un PairDatatype con los tipos de los elementos
    return std::make_pair(true, new PairDatatype{first_type.second, second_type.second});
}

// ConcatExpression
ASTNodeInterface* ConcatExpression::copy() const noexcept
{
    return new ConcatExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool ConcatExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ConcatExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> ConcatExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();
    
    bool result = left_type.first && right_type.first &&
                  left_type.second->is<StringDatatype>() &&
                  right_type.second->is<StringDatatype>();
    
    left_type.second->destroy();
    delete left_type.second;
    right_type.second->destroy();
    delete right_type.second;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }
    
    return std::make_pair(true, new StringDatatype{});
}

// XorExpression
ASTNodeInterface* XorExpression::copy() const noexcept
{
    return new XorExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool XorExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<XorExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> XorExpression::type_check() const noexcept
{
    return this->boolean_operation_type_check();
}

// HeadExpression
ASTNodeInterface* HeadExpression::copy() const noexcept
{
    return new HeadExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool HeadExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<HeadExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> HeadExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first || !expr_type.second->is<ArrayDatatype>())
    {
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    auto array_type = dynamic_cast<ArrayDatatype*>(expr_type.second);
    auto result = dynamic_cast<Datatype*>(array_type->get_inner_datatype()->copy());
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, result);
}

// TailExpression
ASTNodeInterface* TailExpression::copy() const noexcept
{
    return new TailExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool TailExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<TailExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> TailExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first || !expr_type.second->is<ArrayDatatype>())
    {
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    // Tail retorna el mismo tipo de array
    return expr_type;
}

// AddArrayExpression
ASTNodeInterface* AddArrayExpression::copy() const noexcept
{
    return new AddArrayExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AddArrayExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AddArrayExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> AddArrayExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();
    
    if (!left_type.first || !right_type.first || !left_type.second->is<ArrayDatatype>())
    {
        if (left_type.second != nullptr)
        {
            left_type.second->destroy();
            delete left_type.second;
        }
        if (right_type.second != nullptr)
        {
            right_type.second->destroy();
            delete right_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    auto array_type = dynamic_cast<ArrayDatatype*>(left_type.second);
    auto inner_type = array_type->get_inner_datatype();
    
    if (!inner_type->equal(right_type.second))
    {
        left_type.second->destroy();
        delete left_type.second;
        right_type.second->destroy();
        delete right_type.second;
        return std::make_pair(false, nullptr);
    }
    
    right_type.second->destroy();
    delete right_type.second;
    
    return left_type; // Retorna el mismo tipo de array
}

// DelArrayExpression
ASTNodeInterface* DelArrayExpression::copy() const noexcept
{
    return new DelArrayExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool DelArrayExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<DelArrayExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> DelArrayExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();
    
    if (!left_type.first || !right_type.first || 
        !left_type.second->is<ArrayDatatype>() || 
        !right_type.second->is<IntegerDatatype>())
    {
        if (left_type.second != nullptr)
        {
            left_type.second->destroy();
            delete left_type.second;
        }
        if (right_type.second != nullptr)
        {
            right_type.second->destroy();
            delete right_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    right_type.second->destroy();
    delete right_type.second;
    
    return left_type; // Retorna el mismo tipo de array
}

// FstExpression
ASTNodeInterface* FstExpression::copy() const noexcept
{
    return new FstExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool FstExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<FstExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> FstExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first)
    {
        return std::make_pair(false, nullptr);
    }
    
    // Verificar que la expresión es un par
    if (!expr_type.second->is<PairDatatype>())
    {
        expr_type.second->destroy();
        delete expr_type.second;
        return std::make_pair(false, nullptr);
    }
    
    // Retornar el tipo del primer elemento del par
    auto pair_type = dynamic_cast<PairDatatype*>(expr_type.second);
    auto first_type = dynamic_cast<Datatype*>(pair_type->get_first_type()->copy());
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, first_type);
}

// SndExpression
ASTNodeInterface* SndExpression::copy() const noexcept
{
    return new SndExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool SndExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<SndExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> SndExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first)
    {
        return std::make_pair(false, nullptr);
    }
    
    // Verificar que la expresión es un par
    if (!expr_type.second->is<PairDatatype>())
    {
        expr_type.second->destroy();
        delete expr_type.second;
        return std::make_pair(false, nullptr);
    }
    
    // Retornar el tipo del segundo elemento del par
    auto pair_type = dynamic_cast<PairDatatype*>(expr_type.second);
    auto second_type = dynamic_cast<Datatype*>(pair_type->get_second_type()->copy());
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, second_type);
}

// PrintExpression
ASTNodeInterface* PrintExpression::copy() const noexcept
{
    return new PrintExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool PrintExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<PrintExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> PrintExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first)
    {
        return std::make_pair(false, nullptr);
    }
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, new VoidDatatype{});
}

// ItosExpression
ASTNodeInterface* ItosExpression::copy() const noexcept
{
    return new ItosExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool ItosExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ItosExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> ItosExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first || !expr_type.second->is<IntegerDatatype>())
    {
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, new StringDatatype{});
}

// RtosExpression
ASTNodeInterface* RtosExpression::copy() const noexcept
{
    return new RtosExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool RtosExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<RtosExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> RtosExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first || !expr_type.second->is<RealDatatype>())
    {
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, new StringDatatype{});
}

// ItorExpression
ASTNodeInterface* ItorExpression::copy() const noexcept
{
    return new ItorExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool ItorExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ItorExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> ItorExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first || !expr_type.second->is<IntegerDatatype>())
    {
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, new RealDatatype{});
}

// RtoiExpression
ASTNodeInterface* RtoiExpression::copy() const noexcept
{
    return new RtoiExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool RtoiExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<RtoiExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> RtoiExpression::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();
    if (!expr_type.first || !expr_type.second->is<RealDatatype>())
    {
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    expr_type.second->destroy();
    delete expr_type.second;
    
    return std::make_pair(true, new IntegerDatatype{});
}

// LetExpression
LetExpression::LetExpression(const std::string& _name, Expression* _value, Expression* _body) noexcept
    : name(_name), value(_value), body(_body)
{
}

LetExpression::~LetExpression() noexcept
{
    if (value != nullptr)
    {
        value->destroy();
        delete value;
    }
    if (body != nullptr)
    {
        body->destroy();
        delete body;
    }
}

void LetExpression::destroy() noexcept
{
    if (value != nullptr)
    {
        value->destroy();
        delete value;
        value = nullptr;
    }
    if (body != nullptr)
    {
        body->destroy();
        delete body;
        body = nullptr;
    }
}

ASTNodeInterface* LetExpression::copy() const noexcept
{
    return new LetExpression{
        this->name,
        dynamic_cast<Expression*>(this->value->copy()),
        dynamic_cast<Expression*>(this->body->copy())
    };
}

bool LetExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<LetExpression*>(other);
    if (other_expr == nullptr)
        return false;
    
    return this->name == other_expr->name &&
           this->value->equal(other_expr->value) &&
           this->body->equal(other_expr->body);
}

bool LetExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    printf("DEBUG: LetExpression::resolve_name for '%s'\n", name.c_str());
    
    // Resolver el valor
    if (!value->resolve_name(symbol_table))
        return false;
    
    // Obtener el tipo del valor
    auto value_type = value->type_check();
    if (!value_type.first)
        return false;
    
    // Crear una copia del tipo para la tabla de símbolos
    auto type_copy = dynamic_cast<Datatype*>(value_type.second->copy());
    
    // Agregar la variable a la tabla de símbolos
    symbol_table.enter_scope();
    symbol_table.bind(name, Symbol::build(type_copy, name));
    printf("DEBUG: Bound variable '%s' in scope\n", name.c_str());
    
    // Resolver el cuerpo
    bool result = body->resolve_name(symbol_table);
    printf("DEBUG: Body resolution for '%s': %s\n", name.c_str(), result ? "SUCCESS" : "FAILED");
    
    // Salir del scope
    symbol_table.exit_scope();
    
    // Limpiar memoria
    value_type.second->destroy();
    delete value_type.second;
    
    return result;
}

std::pair<bool, Datatype*> LetExpression::type_check() const noexcept
{
    // Verificar el tipo del valor
    auto value_type = value->type_check();
    if (!value_type.first)
        return std::make_pair(false, nullptr);
    
    // Verificar el tipo del cuerpo
    auto body_type = body->type_check();
    if (!body_type.first)
    {
        value_type.second->destroy();
        delete value_type.second;
        return std::make_pair(false, nullptr);
    }
    
    // El tipo del let es el tipo del cuerpo
    value_type.second->destroy();
    delete value_type.second;
    
    return body_type;
}

// FunctionExpression
FunctionExpression::FunctionExpression(const std::string& _name, const std::string& _param, Expression* _body) noexcept
    : name(_name), param(_param), body(_body)
{
}

FunctionExpression::~FunctionExpression() noexcept
{
    if (body != nullptr)
    {
        body->destroy();
        delete body;
    }
}

void FunctionExpression::destroy() noexcept
{
    if (body != nullptr)
    {
        body->destroy();
        delete body;
        body = nullptr;
    }
}

ASTNodeInterface* FunctionExpression::copy() const noexcept
{
    return new FunctionExpression{
        this->name,
        this->param,
        dynamic_cast<Expression*>(this->body->copy())
    };
}

bool FunctionExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<FunctionExpression*>(other);
    if (other_expr == nullptr)
        return false;
    
    return this->name == other_expr->name &&
           this->param == other_expr->param &&
           this->body->equal(other_expr->body);
}

bool FunctionExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    // Crear un scope para el parámetro
    symbol_table.enter_scope();
    
    // Agregar el parámetro a la tabla de símbolos (tipo genérico)
    symbol_table.bind(param, Symbol::build(new IntegerDatatype{}, param));
    
    // Resolver el cuerpo
    bool result = body->resolve_name(symbol_table);
    
    // Salir del scope
    symbol_table.exit_scope();
    
    return result;
}

std::pair<bool, Datatype*> FunctionExpression::type_check() const noexcept
{
    // Crear un scope temporal para el type checking
    SymbolTable temp_table;
    temp_table.enter_scope();
    temp_table.bind(param, Symbol::build(new IntegerDatatype{}, param));
    
    // Verificar el tipo del cuerpo
    auto body_type = body->type_check();
    if (!body_type.first)
    {
        temp_table.exit_scope();
        return std::make_pair(false, nullptr);
    }
    
    // Crear el tipo de función
    ParamList params = {{param, new IntegerDatatype{}}};
    auto function_type = new FunctionDatatype{body_type.second, params};
    
    temp_table.exit_scope();
    
    return std::make_pair(true, function_type);
}

// ValExpression
ValExpression::ValExpression(const std::string& _name, Expression* _value) noexcept
    : name{_name}, value{_value} {}

ValExpression::~ValExpression() noexcept
{
    this->destroy();
}

void ValExpression::destroy() noexcept
{
    if (this->value != nullptr)
    {
        this->value->destroy();
        delete this->value;
        this->value = nullptr;
    }
}

ASTNodeInterface* ValExpression::copy() const noexcept
{
    return new ValExpression{this->name, dynamic_cast<Expression*>(this->value->copy())};
}

bool ValExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_val = dynamic_cast<ValExpression*>(other);
    if (other_val == nullptr)
        return false;
    
    return this->name == other_val->name && this->value->equal(other_val->value);
}

bool ValExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    printf("DEBUG: ValExpression::resolve_name for '%s'\n", this->name.c_str());
    
    // Resolver el valor primero
    if (!this->value->resolve_name(symbol_table))
    {
        printf("DEBUG: Value resolution for '%s': FAILED\n", this->name.c_str());
        return false;
    }
    
    // Crear el símbolo con el tipo inferido del valor
    auto value_type = this->value->type_check();
    if (!value_type.first)
    {
        printf("DEBUG: Type checking for value of '%s': FAILED\n", this->name.c_str());
        return false;
    }
    
    // Bind la variable en el scope global
    auto symbol = Symbol::build(value_type.second, this->name);
    if (!symbol_table.bind(this->name, symbol))
    {
        printf("DEBUG: Binding '%s' in global scope: FAILED\n", this->name.c_str());
        value_type.second->destroy();
        delete value_type.second;
        return false;
    }
    
    printf("DEBUG: Bound '%s' in global scope\n", this->name.c_str());
    value_type.second->destroy();
    delete value_type.second;
    
    return true;
}

std::pair<bool, Datatype*> ValExpression::type_check() const noexcept
{
    // Val retorna el tipo del valor asignado
    return this->value->type_check();
}

// IfExpression
IfExpression::IfExpression(Expression* _condition, Expression* _then_expr, Expression* _else_expr) noexcept
    : condition{_condition}, then_expr{_then_expr}, else_expr{_else_expr} {}

IfExpression::~IfExpression() noexcept
{
    this->destroy();
}

void IfExpression::destroy() noexcept
{
    if (this->condition != nullptr)
    {
        this->condition->destroy();
        delete this->condition;
        this->condition = nullptr;
    }
    if (this->then_expr != nullptr)
    {
        this->then_expr->destroy();
        delete this->then_expr;
        this->then_expr = nullptr;
    }
    if (this->else_expr != nullptr)
    {
        this->else_expr->destroy();
        delete this->else_expr;
        this->else_expr = nullptr;
    }
}

ASTNodeInterface* IfExpression::copy() const noexcept
{
    return new IfExpression{
        dynamic_cast<Expression*>(this->condition->copy()),
        dynamic_cast<Expression*>(this->then_expr->copy()),
        dynamic_cast<Expression*>(this->else_expr->copy())
    };
}

bool IfExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_if = dynamic_cast<IfExpression*>(other);
    if (other_if == nullptr)
        return false;
    
    return this->condition->equal(other_if->condition) &&
           this->then_expr->equal(other_if->then_expr) &&
           this->else_expr->equal(other_if->else_expr);
}

bool IfExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    printf("DEBUG: IfExpression::resolve_name\n");
    
    bool condition_result = this->condition->resolve_name(symbol_table);
    bool then_result = this->then_expr->resolve_name(symbol_table);
    bool else_result = this->else_expr->resolve_name(symbol_table);
    
    printf("DEBUG: IfExpression resolution - condition: %s, then: %s, else: %s\n",
           condition_result ? "SUCCESS" : "FAILED",
           then_result ? "SUCCESS" : "FAILED",
           else_result ? "SUCCESS" : "FAILED");
    
    return condition_result && then_result && else_result;
}

std::pair<bool, Datatype*> IfExpression::type_check() const noexcept
{
    printf("DEBUG: IfExpression::type_check\n");
    
    // Verificar que la condición es booleana
    auto condition_type = this->condition->type_check();
    if (!condition_type.first || !condition_type.second->is<BooleanDatatype>())
    {
        printf("DEBUG: IfExpression condition type check: FAILED\n");
        if (condition_type.second != nullptr)
        {
            condition_type.second->destroy();
            delete condition_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    condition_type.second->destroy();
    delete condition_type.second;
    
    // Verificar que ambas ramas tienen el mismo tipo
    auto then_type = this->then_expr->type_check();
    auto else_type = this->else_expr->type_check();
    
    if (!then_type.first || !else_type.first)
    {
        printf("DEBUG: IfExpression branch type check: FAILED\n");
        if (then_type.second != nullptr)
        {
            then_type.second->destroy();
            delete then_type.second;
        }
        if (else_type.second != nullptr)
        {
            else_type.second->destroy();
            delete else_type.second;
        }
        return std::make_pair(false, nullptr);
    }
    
    // Verificar que ambos tipos son iguales
    if (!then_type.second->equal(else_type.second))
    {
        printf("DEBUG: IfExpression type mismatch: FAILED\n");
        then_type.second->destroy();
        delete then_type.second;
        else_type.second->destroy();
        delete else_type.second;
        return std::make_pair(false, nullptr);
    }
    
    printf("DEBUG: IfExpression type check: SUCCESS\n");
    else_type.second->destroy();
    delete else_type.second;
    
    return std::make_pair(true, then_type.second);
}