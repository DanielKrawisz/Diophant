#ifndef DIOPHANT_EXPRESSIONS_LAMBDA
#define DIOPHANT_EXPRESSIONS_LAMBDA

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/lambda.hpp>

namespace Diophant::expressions {
    
    struct lambda final : abstract {
        Diophant::symbol Argument;
        expression Body;
        bool operator==(const abstract &) const final override;
        lambda(Symbol a, Expression b): Argument{a}, Body{b} {}
    };
}

namespace Diophant {
    
    Lambda inline lambda::operator()(Lambda x) const {
        return lambda(replace(_.get()->Body, _->Argument, x));
    }
    
    std::istream &operator>>(std::istream &i, lambda &l);
    
}

#endif

