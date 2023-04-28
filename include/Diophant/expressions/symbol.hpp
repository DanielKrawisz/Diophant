#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        std::string Name;
        symbol (const std::string &x) : Name {x} {}
        std::ostream &write (std::ostream &) const override;
        
        explicit symbol (Expression);
    };
    
}

namespace Diophant::make {
    Expression inline symbol (const std::string &x) {
        return Diophant::expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<expressions::symbol> (x))};
    }
}

namespace Diophant::expressions {
    
    std::ostream inline &symbol::write (std::ostream &o) const {
        return o << Name;
    }
    
}

#endif

