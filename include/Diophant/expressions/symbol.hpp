#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/expressions/expressions.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        std::string Name;
        symbol(const std::string &x) : Name{x} {}
    };
    
    bool inline symbol::operator==(const abstract &x) const {
        auto p = dynamic_cast<const symbol *>(&x);
        return p == nullptr ? false : Name == p->Name;
    }
}

#endif

