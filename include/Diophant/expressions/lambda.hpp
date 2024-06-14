#ifndef DIOPHANT_EXPRESSIONS_LAMBDA
#define DIOPHANT_EXPRESSIONS_LAMBDA

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/make.hpp>

namespace Diophant::expressions {
    
    struct lambda final : abstract {
        stack<Symbol> arguments;
        Expression body;
        bool operator == (const abstract &) const final override;
        lambda (stack<Symbol> a, Expression b): arguments {a}, body {b} {}
        Expression operator () (Expression) const;
        std::ostream &write (std::ostream &) const override;
        
        uint32 precedence () const override {
            return 300;
        }
    };

    std::istream &operator >> (std::istream &i, lambda &l);
    
    Expression inline lambda::operator () (Expression x) const {
        if (data::size (arguments) == 1) return replace (body, {{{data::first (arguments), x}}});
        return make::lambda (data::rest (arguments), replace (body, {{{data::first (arguments), x}}}));
    }

    std::ostream inline &lambda::write (std::ostream &o) const {
        o << "@ ";
        for (auto x : arguments) o << x << " ";
        return o << "-> " << body;
    }
    
}

namespace Diophant::make {

    Expression inline lambda (stack<Symbol> arg, Expression &body) {
        return Diophant::expression
            {std::static_pointer_cast<const expressions::abstract> (std::make_shared<expressions::lambda> (arg, body))};
    }

}

#endif

