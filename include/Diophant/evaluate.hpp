#ifndef DIOPHANT_EVALUATE
#define DIOPHANT_EVALUATE

#include <Diophant/types.hpp>

namespace Diophant {
    
    struct expression;
    using Expression = const expression;

    bool operator == (Expression &, Expression &);
    
    std::ostream &operator << (std::ostream &, Expression &);
    std::istream &operator >> (std::istream &, expression &);
    
    struct Symbol;

    std::strong_ordering operator <=> (const Symbol &, const Symbol &);
    
    Symbol head (Expression &);
    
    struct pattern;
    using Pattern = const pattern;

    bool operator == (Pattern &, Pattern &);
    std::partial_ordering operator <=> (const Pattern &, const Pattern &);

    struct Machine;

    Expression evaluate (Expression &, Machine &);
    
    
}

#endif
