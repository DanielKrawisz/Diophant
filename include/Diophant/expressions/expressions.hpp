#ifndef DIOPHANT_EXPRESSIONS_EXPRESSIONS
#define DIOPHANT_EXPRESSIONS_EXPRESSIONS

#include <Diophant/precedence.hpp>

namespace Diophant::expressions {
    
    struct abstract {
        virtual ~abstract () = 0;
        
        virtual Diophant::precedence precedence () const {
            return 0;
        }
        
        virtual std::ostream &write (std::ostream &) const = 0;

        virtual bool operator == (const abstract &) const = 0;
        
    };
    
    inline abstract::~abstract () {}
    
    std::ostream inline &write_parens (std::ostream &o, const abstract &x, bool w) {
        return w ? x.write (o << "(") << ")" : x.write (o);
    }
    
}

#endif

