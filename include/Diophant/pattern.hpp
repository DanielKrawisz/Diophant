#ifndef DIOPHANT_PATTERN
#define DIOPHANT_PATTERN

#include <Diophant/data.hpp>

namespace Diophant {

    struct pattern;
    using Pattern = const pattern;

    bool operator == (Pattern &, Pattern &);
    std::partial_ordering operator <=> (const Pattern &, const Pattern &);
    
    std::ostream &operator << (std::ostream &, Pattern &);
    
}

#endif

