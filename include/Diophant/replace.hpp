#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/symbol.hpp>

namespace Diophant {
    
    using replacement = entry<Symbol, Expression>;
    using replacements = list<replacement>;
    
    Expression replace (Expression, replacements);
    
}

#endif
