#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/expression.hpp>

namespace Diophant {
    
    struct type : expression {
        explicit type (Expression &e) : expression {e} {}
        type () : expression {} {}
        /*
        static Type Null ();
        static Type Bool ();
        static Type N ();
        static Type Z ();
        static Type Q ();
        static Type Float ();
        static Type String ();
        static Type Lambda ();*/
        
    };

    // every expression has a type. 
    Type type_of (Expression &);

    std::partial_ordering inline operator <=> (Type &, Type &) {
        return std::partial_ordering::equivalent;
    }
    
    bool inline operator == (Type &, Type &) {
        return true;
    }
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
}

#endif

