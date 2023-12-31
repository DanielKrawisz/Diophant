#ifndef DIOPHANT_EXPRESSIONS_APPLY
#define DIOPHANT_EXPRESSIONS_APPLY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    struct apply final : abstract {
        
        uint32 precedence () const override {
            return 100;
        }
        
        Expression function;
        Expression argument;
        
        apply (Expression &f, Expression &a) : function {f}, argument {a} {}
        
        const abstract *root () const override;
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
        
    };
    
}

namespace Diophant::make {
    
    Expression inline apply (Expression &l, Expression &r) {
        return expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::apply> (l, r))};
    }
    
}

namespace Diophant::expressions {
    
    std::ostream inline &apply::write (std::ostream &o) const {
        return write_parens (
            write_parens (o, 
                *function, 
                function->precedence () > this->precedence ()) << " ", 
            *argument, 
            argument->precedence () > this->precedence ());
    }
    
    const abstract inline *apply::root () const {
        auto a = dynamic_cast<const apply *> (function.get ()); 
        return a == nullptr ? &*this->function : a->root ();
    }

    bool inline apply::operator == (const abstract &a) const {
        try {
            const apply &b = dynamic_cast<const apply &> (a);
            return function == b.function && argument == b.argument;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
}

#endif

