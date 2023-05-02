#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/data.hpp>

namespace Diophant::expressions {
    
    struct symbol;
    
}

namespace Diophant {
    using Symbol = const expressions::symbol;
}

namespace Diophant::expressions {
    
    std::strong_ordering inline operator <=> (const Symbol &a, const Symbol &b);
    
    bool inline operator == (const Symbol &a, const Symbol &b);
    
}

#endif

