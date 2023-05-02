#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/expressions/symbol.hpp>

namespace Diophant {
    
    using replacement = entry<Symbol, Expression>;
    using replacements = map<Symbol, Expression>;
    
    Expression replace (Expression, replacements);
    
}

#endif
