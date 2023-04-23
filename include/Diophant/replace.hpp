#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/evaluate.hpp>

namespace Diophant {
    
    Expression replace (Expression, Symbol, Expression);
    
    bool operator == (Symbol, Symbol);
    
}

#endif
