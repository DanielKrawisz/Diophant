#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/expression.hpp>

namespace Diophant {
    
    struct type;
    
    std::ostream &operator << (std::ostream &o, Type &);
    
    struct Machine;
    intuit cast (Type, Expression, Machine &);

    intuitionistic_partial_ordering compare (Type &, Type &);
    
    struct type : expression {
        type (Expression &e) : expression {e} {}
        type () : expression {} {}
    };
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
}

#endif

