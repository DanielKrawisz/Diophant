#ifndef DIOPHANT_EXPRESSIONS_PATTERN
#define DIOPHANT_EXPRESSIONS_PATTERN

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/pattern.hpp>
#include <Diophant/replace.hpp>

namespace Diophant::expressions {
    
    struct var final: abstract {
        maybe<symbol> name;
        
        static const ptr<const symbol> &make (const std::string &);
        
        std::ostream &write (std::ostream &o) const override {
            return bool (name) ? o << "_" << *name : o << "_";
        }
        
        var () : name {} {}
        var (const symbol &x) : name {x} {}

        bool operator == (const abstract &) const override;
        
    };

    struct such_that final : abstract {
        Pattern pattern;
        Type type;

        such_that (Pattern p, Type t) : pattern {p}, type {t} {}

        std::ostream &write (std::ostream &o) const override {
            return o << pattern << " ? " << type;
        }

        bool operator == (const abstract &) const override;

        Diophant::precedence precedence () const override {
            return 5;
        }
    };
    
}

namespace Diophant::make {
    Pattern inline any () {
        return pattern {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::var> ())};
    }

    Pattern inline var (Symbol &x) {
        return pattern {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::var> (x))};
    }

    Pattern inline such_that (Pattern &p, Type &x) {
        return pattern {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::such_that> (p, x))};
    }
}

#endif

