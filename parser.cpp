#pragma once

#include <string>
#include <vector>

#include "lexer.cpp"

// Base class - defines a function that must exist
class ExpressionAST
{
public:
    virtual ~ExpressionAST() = default;
};

// Base class for the expression abstract tree - works with numbers
class NumberExpressionAST : public ExpressionAST
{
    double _value;

public:
    NumberExpressionAST(double value)
    {
        _value = value;
    };
};

// Variable expression tree - used to reference a variable
class VariableExpressionAST : public ExpressionAST
{
    std::string _name;

public:
    VariableExpressionAST(const std::string &name)
    {
        _name = name;
    };
};

// Binary expression tree - class for a binary operator
class BinaryExpressionAST : public ExpressionAST
{
    char _op;
    std::unique_ptr<ExpressionAST> _LHS, _RHS;

public:
    BinaryExpressionAST(char op,
                        std::unique_ptr<ExpressionAST> LHS,
                        std::unique_ptr<ExpressionAST> RHS)
    {
        _op = op;
        _LHS = std::move(LHS);
        _RHS = std::move(RHS);
    }
};

// Class used to actually call an ExpressionAST
class CallExpressionAST : public ExpressionAST
{
    std::string _callee;
    std::vector<std::unique_ptr<ExpressionAST>> _args;

public:
    CallExpressionAST(const std::string callee,
                      std::vector<std::unique_ptr<ExpressionAST>> args)
    {
        _callee = callee;
        _args = args;
    }
};

// Used for function prototypes/declarations - allows for forward declaring?
class PrototypeAST
{
    std::string _name;
    std::vector<std::string> _args;

public:
    PrototypeAST(const std::string name,
                 std::vector<std::string> args)
    {
        _name = name;
        _args = args;
    }
};

// Class for a function tree - will be used to compute return value
class FunctionAST
{
    std::unique_ptr<PrototypeAST> _prototype;
    std::unique_ptr<ExpressionAST> _body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> prototype,
                std::unique_ptr<ExpressionAST> body)
    {
        _prototype = std::move(prototype);
        _body = std::move(body);
    }
};

static int current_token;
static int get_next_token() {
    return current_token = get_token();
}

std::unique_ptr<ExpressionAST> log_error(const char* error_string) {
    fprintf(stderr, "error: %s\n", error_string);
    return NULL;
}

std::unique_ptr<PrototypeAST> log_error_proto(const char* error_string) {
    log_error(error_string);
    return NULL;
}

// To be called whenever we see a number
static std::unique_ptr<ExpressionAST>