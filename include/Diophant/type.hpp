#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/evaluate.hpp>

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
    
    // throws an exception if the cast is not valid. 
    Expression inline cast (const Type &, Expression &x);

    std::partial_ordering inline operator <=> (const Type &, const Type &) {
        return std::partial_ordering::equivalent;
    }
    
    bool inline operator == (const Type &, const Type &) {
        return true;
    }
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
    // For now every cast is valid.
    Expression inline cast (const Type &, Expression &x) {
        return x;
    }
    
}

#endif

