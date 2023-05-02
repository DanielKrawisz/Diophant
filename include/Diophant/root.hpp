#ifndef DIOPHANT_ROOT
#define DIOPHANT_ROOT

#include <Diophant/symbol.hpp>
#include <Diophant/expressions/expressions.hpp>

namespace Diophant {
    
    const expressions::abstract *root (Expression &);
    Symbol &root (Pattern &); 
    
}

#endif

