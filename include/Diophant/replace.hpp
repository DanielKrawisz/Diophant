#ifndef DIOPHANT_REPLACE
#define DIOPHANT_REPLACE

#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expression.hpp>

namespace Diophant {
    
    using replacement = entry<expressions::symbol, Expression>;
    struct replacements : maybe<map<expressions::symbol, Expression>> {
        using maybe<map<expressions::symbol, Expression>>::maybe;
        replacements insert (const expressions::symbol &x, Expression &e) {
            if (!bool (*this) || (*this)->contains (x)) return {};
            return (*this)->insert (x, e);
        }
    };
    
    Expression replace (Expression, replacements);
    
}

namespace Diophant::make {

    Expression inline symbol (const std::string &x) {
        return Diophant::expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<expressions::symbol> (x))};
    }

}

#endif
