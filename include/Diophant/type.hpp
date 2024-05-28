#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/expression.hpp>

namespace Diophant {
    
    struct type;
    
    bool operator == (Type &, Type &);
    
    std::ostream &operator << (std::ostream &o, Type &);
    
    enum intuit : char {
        no = 0, 
        yes = 1, 
        unknown = -1
    };
    
    intuit cast (Type, Expression);
    intuit cast (Type, Type);
    
    struct type : expression {
        explicit type (Expression &e) : expression {e} {}
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

