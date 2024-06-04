#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/data.hpp>

namespace Diophant::expressions {
    struct symbol;
}

namespace Diophant {
    using symbol = ptr<const expressions::symbol>;
    using Symbol = const symbol;
    using symbols = std::map<std::string, symbol>;

    std::ostream &operator << (std::ostream &, Symbol);
    std::strong_ordering operator <=> (Symbol a, Symbol b);
    bool operator == (Symbol a, Symbol b);
}

#endif

