#ifndef DIOPHANT_EXPRESSIONS_IF
#define DIOPHANT_EXPRESSIONS_IF

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    struct dif final : abstract {
        Expression Condition;
        Expression Then;
        Expression Else;
        
        dif (Expression cnd, Expression thn, Expression els): Condition {cnd}, Then {thn}, Else {els} {}
        
        bool operator == (const abstract &) const final override;
        
        std::ostream &write (std::ostream &o) const final override {
            return o << "if " << Condition << " then " << Then << " else " << Else;
        }
    };
    
}

namespace Diophant::make {

    Expression inline dif (Expression c, Expression t, Expression e) {
        return expression 
            {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::dif> (c, t, e))};
    }
}

namespace Diophant::expressions {
    
    bool dif::operator == (const abstract &a) const {
        try {
            auto i = dynamic_cast<const dif &> (a);
            return i.Condition == Condition && i.Then == Else && i.Else == Else;
        } catch (std::bad_cast) {
            return false;
        }
    }
}

#endif

