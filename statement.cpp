#include <statement.hpp>

#include <datatype.hpp>
#include <declaration.hpp>
#include <expression.hpp>
#include <symbol_table.hpp>

DeclarationStatement::DeclarationStatement(Declaration* decl) noexcept
    : declaration{decl} {}

void DeclarationStatement::destroy() noexcept
{
    this->declaration->destroy();
    delete this->declaration;
    this->declaration = nullptr;
}

ASTNodeInterface* DeclarationStatement::copy() const noexcept
{
    return new DeclarationStatement{dynamic_cast<Declaration*>(this->declaration->copy())};
}

bool DeclarationStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<DeclarationStatement*>(other);
    return other_stmt != nullptr && this->declaration->equal(other_stmt->declaration);
}

std::pair<bool, Datatype*> DeclarationStatement::type_check() const noexcept
{
    return this->declaration->type_check();
}

bool DeclarationStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->declaration->resolve_name(symbol_table);
}

ExpressionStatement::ExpressionStatement(Expression* expr) noexcept
    : expression{expr} {}

void ExpressionStatement::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
    this->expression = nullptr;
}

ASTNodeInterface* ExpressionStatement::copy() const noexcept
{
    return new ExpressionStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

bool ExpressionStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<ExpressionStatement*>(other);
    return other_stmt != nullptr && this->expression->equal(other_stmt->expression);
}

std::pair<bool, Datatype*> ExpressionStatement::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();

    if (expr_type.second != nullptr)
    {
        delete expr_type.second;
        expr_type.second = nullptr;
    }
    return expr_type;
}

bool ExpressionStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->expression->resolve_name(symbol_table);
}

IfElseStatement::IfElseStatement(Expression* ctrl_expr, const Body& _body, const Body& _else_body) noexcept
    : ExpressionStatement{ctrl_expr}, body{_body}, else_body{_else_body} {}

void IfElseStatement::destroy() noexcept
{
    ExpressionStatement::destroy();
    destroy_body(this->body);
    destroy_body(this->else_body);
}

ASTNodeInterface* IfElseStatement::copy() const noexcept
{
    return new IfElseStatement{
        dynamic_cast<Expression*>(this->expression->copy()),
        copy_body(this->body) ,
        copy_body(this->else_body)
    };
}

bool IfElseStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<IfElseStatement*>(other);
    return other_stmt != nullptr && ExpressionStatement::equal(other) &&
        equal_body(this->body, other_stmt->body) && equal_body(this->else_body, other_stmt->else_body);
}

std::pair<bool, Datatype*> IfElseStatement::type_check() const noexcept
{
    auto result = ExpressionStatement::type_check().first &&
        body_type_check(this->body).first && 
        body_type_check(this->else_body).first;
    return std::make_pair(result, nullptr);
}

bool IfElseStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool expr_result = ExpressionStatement::resolve_name(symbol_table);

    symbol_table.enter_scope();
    bool body_result = resolve_name_body(this->body, symbol_table);
    symbol_table.exit_scope();

    symbol_table.enter_scope();
    bool else_body_result = resolve_name_body(this->else_body, symbol_table);
    symbol_table.exit_scope();

    return expr_result && body_result && else_body_result;
}


BlockStatement::BlockStatement(const Body& _body) noexcept
    : body{_body} {}

void BlockStatement::destroy() noexcept
{
    destroy_body(this->body);
}

ASTNodeInterface* BlockStatement::copy() const noexcept
{
    return new BlockStatement{copy_body(this->body)};
}

bool BlockStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<BlockStatement*>(other);
    return other_stmt != nullptr && equal_body(this->body, other_stmt->body);
}

std::pair<bool, Datatype*> BlockStatement::type_check() const noexcept
{
    return body_type_check(this->body);
}

bool BlockStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    symbol_table.enter_scope();
    bool result = resolve_name_body(this->body, symbol_table);
    symbol_table.exit_scope();
    return result;
}

ASTNodeInterface* PrintStatement::copy() const noexcept
{
    return new PrintStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

bool PrintStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<PrintStatement*>(other);
    return other_stmt != nullptr && ExpressionStatement::equal(other);
}
