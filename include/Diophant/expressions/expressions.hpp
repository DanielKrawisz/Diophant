#ifndef DIOPHANT_EXPRESSIONS_EXPRESSIONS
#define DIOPHANT_EXPRESSIONS_EXPRESSIONS

#include <Diophant/data.hpp>

namespace Diophant::expressions {
    
    struct abstract {
        virtual ~abstract() = 0;
        
        virtual uint32 precedence () const {
            return 0;
        }
        
        virtual std::ostream &write (std::ostream &) const = 0;
        
        virtual const abstract *root () const {
            return nullptr;
        }
        
    };
    
    inline abstract::~abstract() {}
    
    std::ostream inline &write_parens (std::ostream &o, const abstract &x, bool w) {
        return w ? x.write (o << "(") << ")" : x.write (o);
    }
    
}

#endif

