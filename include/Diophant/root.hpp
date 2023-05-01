#ifndef DIOPHANT_ROOT
#define DIOPHANT_ROOT

#include <Diophant/expressions/expressions.hpp>

namespace Diophant {
    
    struct pattern;
    using Pattern = const pattern;

    struct expression;
    using Expression = const expression;
    
    const expressions::abstract *root (Pattern &); 
    const expressions::abstract &root (Expression &);
    
}

#endif

