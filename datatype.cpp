#include <datatype.hpp>

std::pair<bool, Datatype*> Datatype::type_check() const noexcept
{
    return std::make_pair(true, nullptr);
}

void BasicDatatype::destroy() noexcept {}

bool BasicDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    return true;
}

ASTNodeInterface* VoidDatatype::copy() const noexcept 
{
    return new VoidDatatype{};
}

bool VoidDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<VoidDatatype*>(other) != nullptr;
}

ASTNodeInterface* BooleanDatatype::copy() const noexcept 
{
    return new BooleanDatatype{};
}

bool BooleanDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<BooleanDatatype*>(other) != nullptr;
}

ASTNodeInterface* CharacterDatatype::copy() const noexcept 
{
    return new CharacterDatatype{};
}

bool CharacterDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<CharacterDatatype*>(other) != nullptr;
}

ASTNodeInterface* IntegerDatatype::copy() const noexcept 
{
    return new IntegerDatatype{};
}

bool IntegerDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<IntegerDatatype*>(other) != nullptr;
}

ASTNodeInterface* StringDatatype::copy() const noexcept 
{
    return new StringDatatype{};
}

bool StringDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<StringDatatype*>(other) != nullptr;
}

ASTNodeInterface* RealDatatype::copy() const noexcept 
{
    return new RealDatatype{};
}

bool RealDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<RealDatatype*>(other) != nullptr;
}

ArrayDatatype::ArrayDatatype(Datatype* _inner_datatype) noexcept
    : inner_datatype{_inner_datatype} {}

void ArrayDatatype::destroy() noexcept
{
    this->inner_datatype->destroy();
    delete this->inner_datatype;
    this->inner_datatype = nullptr;
}

ASTNodeInterface* ArrayDatatype::copy() const noexcept 
{
    return new ArrayDatatype{dynamic_cast<Datatype*>(this->inner_datatype->copy())};
}

bool ArrayDatatype::equal(ASTNodeInterface* other) const noexcept
{
    auto other_array = dynamic_cast<ArrayDatatype*>(other);
    return other != nullptr && this->inner_datatype->equal(other_array->inner_datatype);
}

bool ArrayDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->inner_datatype->resolve_name(symbol_table);
}

Datatype* ArrayDatatype::get_inner_datatype() const noexcept
{
    return this->inner_datatype;
}

FunctionDatatype::FunctionDatatype(Datatype* ret_type, const ParamList& params) noexcept
    : return_type{ret_type}, parameters{params} {}

void FunctionDatatype::destroy() noexcept
{
    this->return_type->destroy();
    delete this->return_type;
    this->return_type = nullptr;
    destroy_param_list(this->parameters);
}

ASTNodeInterface* FunctionDatatype::copy() const noexcept 
{
    return new FunctionDatatype{
        dynamic_cast<Datatype*>(this->return_type->copy()),
        copy_param_list(this->parameters)
    };
}

bool FunctionDatatype::equal(ASTNodeInterface* other) const noexcept
{
    auto other_function = dynamic_cast<FunctionDatatype*>(other);
    return other_function != nullptr && this->return_type->equal(other_function->return_type) &&
        equal_param_list(this->parameters, other_function->parameters);
}

bool FunctionDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool result = this->return_type->resolve_name(symbol_table);
    return result && resolve_name_param_list(this->parameters, symbol_table);
}

Datatype* FunctionDatatype::get_return_type() const noexcept
{
    return this->return_type;
}

const ParamList& FunctionDatatype::get_parameters() const noexcept
{
    return this->parameters;
}

// PairDatatype
PairDatatype::PairDatatype(Datatype* _first_type, Datatype* _second_type) noexcept
    : first_type{_first_type}, second_type{_second_type} {}

void PairDatatype::destroy() noexcept
{
    if (this->first_type != nullptr)
    {
        this->first_type->destroy();
        delete this->first_type;
        this->first_type = nullptr;
    }
    if (this->second_type != nullptr)
    {
        this->second_type->destroy();
        delete this->second_type;
        this->second_type = nullptr;
    }
}

ASTNodeInterface* PairDatatype::copy() const noexcept
{
    return new PairDatatype{
        dynamic_cast<Datatype*>(this->first_type->copy()),
        dynamic_cast<Datatype*>(this->second_type->copy())
    };
}

bool PairDatatype::equal(ASTNodeInterface* other) const noexcept
{
    auto other_pair = dynamic_cast<PairDatatype*>(other);
    if (other_pair == nullptr)
        return false;
    
    return this->first_type->equal(other_pair->first_type) &&
           this->second_type->equal(other_pair->second_type);
}

bool PairDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool first_result = this->first_type->resolve_name(symbol_table);
    bool second_result = this->second_type->resolve_name(symbol_table);
    return first_result && second_result;
}

Datatype* PairDatatype::get_first_type() const noexcept
{
    return this->first_type;
}

Datatype* PairDatatype::get_second_type() const noexcept
{
    return this->second_type;
}