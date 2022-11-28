#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/evaluate.hpp>

namespace Diophant {
    struct symbol;
    using Symbol = const symbol;
    
    Expression replace(Expression, Symbol, Expression);
    
    bool operator==(Symbol, Symbol);
    
}

#endif
