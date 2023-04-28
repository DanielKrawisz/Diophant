#ifndef DIOPHANT_PATTERN
#define DIOPHANT_PATTERN

#include <Diophant/symbol.hpp>

namespace Diophant {
    
    struct pattern {
        pattern (Expression &);

        // attempt to match against the expression. Return a list of replacements if match is successful.
        maybe<list<entry<Symbol, Expression>>> match (Expression &) const;
    };

    maybe<list<entry<Symbol, Expression>>> inline match (Expression &e, Pattern &p) {
        return p.match (e);
    }
    
}

#endif

