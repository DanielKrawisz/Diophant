#ifndef DIOPHANT_TYPE
#define DIOPHANT_TYPE

#include <Diophant/expression.hpp>
#include <Diophant/symbol.hpp>

namespace Diophant {
    
    struct type;
    
    std::ostream &operator << (std::ostream &o, Type &);
    
    struct Machine;
    intuit cast (Type, Expression, const Machine &);

    intuitionistic_partial_ordering compare (Type &, Type &);
    
    struct type : expression {
        type (Expression &e) : expression {e} {}
        type (expression &&e) : expression {e} {}
        type () : expression {} {}
        
        static Type Null ();
        static Type Bool ();
        static Type N ();
        static Type Z ();
        static Type Q ();
        static Type Z (const data::N &);
        static Type String ();
        static Type Float ();
        static Type List (Type &);
        static Type Tuple (stack<Type>);
        static Type Array (Type &, uint32 size);
        
    };
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
}

#endif

