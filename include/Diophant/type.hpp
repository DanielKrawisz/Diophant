#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/expression.hpp>

namespace Diophant {
    
    struct Type;

    std::partial_ordering operator <=> (const Type &, const Type &);
    bool operator == (const Type &, const Type &);
    
    std::ostream &operator << (std::ostream &, Type &);

    // every expression has a type. 
    Type type (Expression &);

    struct Type {
        
        Type () {}
        
        explicit Type (Expression) {}
        
    };

    std::partial_ordering inline operator <=> (const Type &, const Type &) {
        return std::partial_ordering::equivalent;
    }
    
    bool inline operator == (const Type &, const Type &) {
        return true;
    }
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
}

#endif

