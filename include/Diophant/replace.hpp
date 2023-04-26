#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/expression.hpp>

namespace Diophant {
    
    Expression replace (Expression, data::list<data::entry<Symbol, Expression>>);
    
}

#endif
