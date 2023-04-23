#ifndef DIOPHANT_EVALUATE
#define DIOPHANT_EVALUATE

#include <Diophant/types.hpp>

namespace Diophant {
    
    struct Expression;
    
    std::ostream &operator << (std::ostream &, Expression);
    std::istream &operator >> (std::istream &, Expression &);
    
    struct Symbol;
    
    Symbol head (Expression);
    
    struct Pattern;
    
    struct machine {
        bool cast (Expression, Expression) const;
        
        Expression evaluate (Expression);
        maybe<map<Symbol, Expression>> match (Expression, Pattern);
        
        declare (const Pattern &);
        define (const Pattern &, Expression);
        
        machine ();
        
        std::map<Symbol, std::list<entry<Pattern, Expression>>> Definitions;
    };
    
}

#endif
