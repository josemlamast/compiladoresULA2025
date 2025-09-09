#include <symbol_table.hpp>

std::shared_ptr<Symbol> Symbol::build(Datatype* type, std::string_view name) noexcept
{
    auto symbol = std::make_shared<Symbol>();
    symbol->type = type;
    symbol->name = name;
    return symbol;
}

SymbolTable::SymbolTable() noexcept
{
    this->enter_scope();
}

SymbolTable::~SymbolTable() noexcept
{
    this->exit_scope();
}

void SymbolTable::enter_scope() noexcept
{
    this->scopes.push_back(TableType{});
    printf("DEBUG: Entered scope, now at level %zu\n", this->scopes.size());
}

bool SymbolTable::exit_scope() noexcept
{
    if (this->scopes.empty())
    {
        return false;
    }

    printf("DEBUG: Exiting scope, was at level %zu\n", this->scopes.size());
    this->scopes.pop_back();
    return true;
}

SymbolTable::TableType::size_type SymbolTable::scope_level() const noexcept
{
    return this->scopes.size();
}

bool SymbolTable::bind(const std::string& name, std::shared_ptr<Symbol> symbol) noexcept
{
    if (this->scopes.empty())
    {
        return false;
    }

    TableType& current_scope = this->scopes.back();

    if (SymbolTable::find_in_scope(name, current_scope) != nullptr)
    {
        return false;
    }

    current_scope.emplace(name, symbol);
    printf("DEBUG: Bound '%s' at scope level %zu\n", name.c_str(), this->scopes.size());

    return true;
}

std::shared_ptr<Symbol> SymbolTable::lookup(const std::string& name) noexcept
{
    printf("DEBUG: Looking up '%s' in %zu scopes\n", name.c_str(), this->scopes.size());
    for (auto it = this->scopes.rbegin(); it != this->scopes.rend(); ++it)
    {
        auto found = SymbolTable::find_in_scope(name, *it);

        if (found != nullptr)
        {
            printf("DEBUG: Found '%s' in scope\n", name.c_str());
            return found;
        }
    }

    printf("DEBUG: '%s' not found in any scope\n", name.c_str());
    return nullptr;
}

std::shared_ptr<Symbol> SymbolTable::current_scope_lookup(const std::string& name) noexcept
{
    if (this->scopes.empty())
    {
        return nullptr;
    }
    
    return SymbolTable::find_in_scope(name, this->scopes.back());
}

std::shared_ptr<Symbol> SymbolTable::find_in_scope(const std::string& name, const TableType& scope) noexcept
{
    auto found_it = scope.find(name);
        
    if (found_it != scope.end())
    {
        return found_it->second;
    }

    return nullptr;
}