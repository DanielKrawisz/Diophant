#ifndef DIOPHANT_EXPRESSIONS_CALL
#define DIOPHANT_EXPRESSIONS_CALL

#include <Diophant/expressions/expressions.hpp>

namespace Diophant::expressions {
    
    struct call final: abstract {
        expression Function;
        expression Argument;
        bool operator==(const abstract &) const final override;
        call(Expression f, Expression x): Function{f}, Argument{x} {}
    };
    
}

#endif
