#ifndef DIOPHANT_EVALUATE
#define DIOPHANT_EVALUATE

#include <Diophant/types.hpp>

namespace Diophant {
    
    struct expression;
    using Expression = const expression;
    
    bool cast(Expression, Expression);
    Expression evaluate(Expression);
    Expression call(Expression, Expression);
    
    bool operator==(Expression, Expression);
    
    std::ostream &operator<<(std::ostream &, Expression);
    std::istream &operator>>(std::istream &, expression &);
    
}

#endif
