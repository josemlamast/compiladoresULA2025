#pragma once

#include <stdexcept>

class TypeError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class NotFoundError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class BadProgram : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class OperationError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
