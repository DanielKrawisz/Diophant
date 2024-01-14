#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expression.hpp>

namespace Diophant {

    struct pattern : expression {
        pattern (Expression &ex) : expression {ex} {}
        pattern () : expression {} {}
    };
    
    using replacement = entry<Symbol, Expression>;
    using replacements = map<Symbol, Expression>;
    
    Expression replace (Expression, replacements);
    
}

namespace Diophant::make {

    Expression inline symbol (const std::string &x, symbols &X) {
        return expressions::symbol::make (x, X);
    }

}

#endif

