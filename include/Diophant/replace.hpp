#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/symbol.hpp>

namespace Diophant {
    
    Expression replace (Expression, list<entry<Symbol, Expression>>);
    
}

#endif
