#ifndef DIOPHANT_EXPRESSIONS_LAMBDA
#define DIOPHANT_EXPRESSIONS_LAMBDA

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/replace.hpp>

namespace Diophant {
    bool pattern_equal (Expression &a, Expression &b, list<std::pair<Symbol, Symbol>>);
}

namespace Diophant::expressions {
    
    struct lambda final : abstract {
        Symbol argument;
        Expression body;
        bool operator == (const abstract &) const final override;
        lambda (Symbol a, Expression b): argument {a}, body {b} {}
        Expression operator () (Expression) const;
        std::ostream &write (std::ostream &) const override;
        
        uint32 precedence () const override {
            return 150;
        }
    };

    std::istream &operator >> (std::istream &i, lambda &l);
    
    Expression inline lambda::operator () (Expression x) const {
        return replace (body, {{{argument, x}}});
    }

    bool inline lambda::operator == (const abstract &x) const {
        try {
            auto z = dynamic_cast<const lambda &> (x);
            return pattern_equal (this->body, z.body, {{argument, z.argument}});
        } catch (std::bad_cast) {
            return false;
        }
    }

    std::ostream inline &lambda::write (std::ostream &o) const {
        return o << "@ " << argument << " -> " << body;
    }
    
}

namespace Diophant::make {

    Expression inline lambda (Symbol &arg, Expression &body) {
        return Diophant::expression
            {std::static_pointer_cast<const expressions::abstract> (std::make_shared<expressions::lambda> (arg, body))};
    }

}

#endif

