#ifndef DIOPHANT_EXPRESSIONS_EXPRESSIONS
#define DIOPHANT_EXPRESSIONS_EXPRESSIONS

#include <Diophant/types.hpp>

namespace Diophant::expressions {
    
    struct abstract {
        virtual ~abstract() = 0;
        
        virtual uint32 precedence () const {
            return 0;
        }
        
        virtual std::ostream &write (std::ostream &) const = 0;
    };
    
    inline abstract::~abstract() {}
    
}

#endif

