#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/expressions/symbol.hpp>

namespace Diophant {
    using Symbol = const expressions::symbol;
}

namespace Diophant::expressions {
    
    std::strong_ordering inline operator <=> (const Symbol &a, const Symbol &b) {
        return a.Name <=> b.Name;
    }
    
    bool inline operator == (const Symbol &a, const Symbol &b) {
        return a.Name == b.Name;
    }
    
}

#endif

