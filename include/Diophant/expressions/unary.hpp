#ifndef DIOPHANT_EXPRESSIONS_UNARY
#define DIOPHANT_EXPRESSIONS_UNARY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    enum class unary_operand {
        NOT, 
        NEGATE, 
        PLUS
    };
    
    template <unary_operand X>
    struct unary_expression final : abstract {
        
        uint32 precedence () const override {
            return 200;
        }
        
        Expression expression;
        unary_expression (Expression &x) : expression {x} {}
        
        static Expression make (Expression &);
    };
    
    using boolean_not = unary_expression<unary_operand::NOT>;
    using negate = unary_expression<unary_operand::NEGATE>;
}

namespace Diophant::make {
    
    Expression inline boolean_not (Expression &x) {
        return expressions::boolean_not::make (x);
    }
    
    Expression inline negate (Expression &x) {
        return expressions::negate::make (x);
    }
    
}

namespace Diophant::expressions {
    
    template <unary_operand X>
    Expression inline unary_expression<X>::make (Expression &x) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<unary_expression<X>> (x))};
    }
}


#endif

