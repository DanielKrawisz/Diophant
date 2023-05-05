#ifndef DIOPHANT_EXPRESSIONS_PATTERN
#define DIOPHANT_EXPRESSIONS_PATTERN

#include <Diophant/machine.hpp>

namespace Diophant::expressions {
    
    struct pattern final: abstract {
        symbol var;
        Type type;
        
        static const ptr<const symbol> &make (const std::string &);
        
        std::ostream &write (std::ostream &) const override;
        
        pattern (const symbol &x) : var {x}, type {} {}
        pattern (const symbol &x, const Type &t) : var {x}, type {t} {}
        
        maybe<replacements> match (Expression &e, const Machine &m) const;
        
    };
    
}

namespace Diophant::make {
    Expression inline pattern (Symbol &x, const Type &t) {
        return expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::pattern> (x, t))};
    }
}

namespace Diophant::expressions {
        
    maybe<replacements> inline pattern::match (Expression &e, const Machine &m) const {
        return m.cast (type, e) ? maybe<replacements> {replacements {replacement {var, e}}} : maybe<replacements> {};
    }
}

#endif

