#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/expression.hpp>

namespace Diophant {
    
    struct type;
    
    bool operator == (Type &, Type &);
    
    std::ostream &operator << (std::ostream &o, Type &);
    
    intuit cast (Type, Expression);
    
    struct type : expression {
        type (Expression &e) : expression {e} {}
        type () : expression {} {}
    };
    
    bool inline operator == (Type &, Type &) {
        return true;
    }
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
}

#endif

