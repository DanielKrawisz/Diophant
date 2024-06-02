#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/data.hpp>

namespace Diophant::expressions {
    struct symbol;
}

namespace Diophant {
    using symbol = expressions::symbol;
    using Symbol = const symbol;
    using symbols = std::map<std::string, ptr<Symbol>>;
}

namespace Diophant::expressions {
    std::ostream &operator << (std::ostream &, Symbol);
    std::strong_ordering inline operator <=> (Symbol &a, Symbol &b);
    bool inline operator == (Symbol &a, Symbol &b);
}

#endif

