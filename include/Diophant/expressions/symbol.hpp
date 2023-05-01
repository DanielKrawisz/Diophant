#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        const std::string *Name;
        
        static const ptr<const symbol> &make (const std::string &);
        
        std::ostream &write (std::ostream &) const override;
        
        const abstract *root () const override {
            return this;
        }
        
    private:
        symbol (const std::string &x) : Name {&x} {}
        symbol () : Name {} {}
    };
    
}

namespace Diophant::make {
    Expression inline symbol (const std::string &x) {
        return Diophant::expression {std::static_pointer_cast<const expressions::abstract> (expressions::symbol::make (x))};
    }
}

namespace Diophant::expressions {
    
    std::ostream inline &symbol::write (std::ostream &o) const {
        return o << Name;
    }
    
}

#endif

