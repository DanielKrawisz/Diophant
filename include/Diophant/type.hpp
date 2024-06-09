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
        
        static Type Void ();
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
        
        static Diophant::symbols *&symbols () {
            static Diophant::symbols *Symbols = nullptr;
            return Symbols;
        }
    };
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
    Type inline type::Void () {
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["Void"])};
    }
    
    Type inline type::Bool () {
        if (symbols == nullptr) throw exception {} << "no global symbols repository has been registered";
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["Bool"])};
    }
    
    Type inline type::N () {
        if (symbols == nullptr) throw exception {} << "no global symbols repository has been registered";
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["N"])};
    }
    
    Type inline type::Z () {
        if (symbols == nullptr) throw exception {} << "no global symbols repository has been registered";
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["Z"])};
    }
    
    Type inline type::Q () {
        if (symbols == nullptr) throw exception {} << "no global symbols repository has been registered";
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["Q"])};
    }
    
    Type inline type::String () {
        if (symbols == nullptr) throw exception {} << "no global symbols repository has been registered";
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["String"])};
    }
    
    Type inline type::Float () {
        if (symbols == nullptr) throw exception {} << "no global symbols repository has been registered";
        return expression {std::static_pointer_cast<const expressions::abstract> ((*symbols ())["Float"])};
    }
    
}

#endif

