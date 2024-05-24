#ifndef DIOPHANT_VALUE
#define DIOPHANT_VALUE

#include <Diophant/expression.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/lambda.hpp>

namespace Diophant {

    struct value : expression {
        // a value is valid if it is a valid expression that contains no unevaluated symbols.
        bool valid () const;
        using expression::expression;
    };
    
    value run (const value &);
    
}

#endif

