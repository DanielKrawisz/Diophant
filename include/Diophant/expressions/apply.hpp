#ifndef DIOPHANT_EXPRESSIONS_APPLY
#define DIOPHANT_EXPRESSIONS_APPLY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {

    struct call final : abstract {

        uint32 precedence () const override;
        Expression function;
        data::list<Expression> arguments;

        call (Expression &f, data::list<Expression> &a) : function {f}, arguments {a} {}

        std::ostream &write (std::ostream &o) const override;

        bool operator == (const abstract &) const override;

    };

}

namespace Diophant {
    constexpr precedence call_precedence = 700;
}

namespace Diophant::make {

    Expression inline call (Expression &l, data::list<Expression> r) {
        return expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::call> (l, r))};
    }
    
}

namespace Diophant::expressions {

    std::ostream inline &call::write (std::ostream &o) const {
        write_parens (o, *function, function->precedence () > this->precedence ());
        for (const auto &arg : arguments) write_parens (o << " ", *arg, arg->precedence () > this->precedence ());
        return o;
    }

    bool inline call::operator == (const abstract &a) const {
        try {
            const call &b = dynamic_cast<const call &> (a);
            return function == b.function && arguments == b.arguments;
        } catch (std::bad_cast) {
            return false;
        }
    }

    Diophant::precedence inline call::precedence () const {
        return call_precedence;
    }
    
}

#endif

