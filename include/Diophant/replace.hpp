#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expression.hpp>

namespace Diophant {
    
    using replacement = entry<Symbol, Expression>;
    struct replacements : maybe<map<Symbol, Expression>> {
        using maybe<map<Symbol, Expression>>::maybe;
        replacements insert (Symbol &x, Expression &e) {
            if (!bool (*this) || (*this)->contains (x)) return {};
            return (*this)->insert (x, e);
        }
    };
    
    Expression replace (Expression, replacements);
    
}

namespace Diophant::make {

    Expression inline symbol (const std::string &x, symbols &X) {
        return expressions::symbol::make (x, X);
    }

}

#endif
