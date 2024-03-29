#ifndef DIOPHANT_EXPRESSIONS_PATTERN
#define DIOPHANT_EXPRESSIONS_PATTERN

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/type.hpp>

namespace Diophant::expressions {
    
    struct typed final: abstract {
        symbol var;
        Type type;
        
        static const ptr<const symbol> &make (const std::string &);
        
        std::ostream &write (std::ostream &) const override {
            throw method::unimplemented {"typed write"};
        }
        
        typed (const symbol &x) : var {x}, type {} {}
        typed (const symbol &x, const Type &t) : var {x}, type {t} {}

        bool operator == (const abstract &) const override {
            throw method::unimplemented {"typed =="};
        }
        
    };
    
}

namespace Diophant::make {
    Expression inline typed (const Type &t, Symbol &x) {
        return expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::typed> (x, t))};
    }
}
/*
namespace Diophant::expressions {
        
    maybe<replacements> inline pattern::match (Expression &e, const Machine &m) const {
        return m.cast (type, e) ? maybe<replacements> {replacements {replacement {var, e}}} : maybe<replacements> {};
    }
}*/

#endif

