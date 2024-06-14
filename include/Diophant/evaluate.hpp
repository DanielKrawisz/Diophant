#ifndef DIOPHANT_EVALUATE
#define DIOPHANT_EVALUATE

#include <Diophant/expressions/symbol.hpp>

namespace Diophant {

    struct Machine;

    Expression evaluate (Expression &, Machine &, data::set<symbol> fixed = {});
    
}

#endif
