#ifndef DIOPHANT_EXPRESSIONS_UNARY
#define DIOPHANT_EXPRESSIONS_UNARY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>

namespace Diophant::expressions {
    
    enum class unary_operand {
        NOT, 
        NEGATE, 
        PLUS
    };
    
    template <unary_operand X>
    constexpr const char *unary_operator () {
        switch (X) {
            case unary_operand::NOT : return "!";
            case unary_operand::NEGATE : return "-";
            case unary_operand::PLUS : return "+";
        }
    }
    
    template <unary_operand X>
    struct unary_expression final : abstract {
        
        uint32 precedence () const override {
            return 200;
        }
        
        Expression expression;
        unary_expression (Expression &x) : expression {x} {}
        
        static Expression make (Expression &);
        
        std::ostream &write (std::ostream &) const override;
        
        const abstract *root () const override {
            return &static_cast<const abstract &> (*symbol::make (unary_operator<X> ()));
        }
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
    
    template <unary_operand X>
    std::ostream inline &unary_expression<X>::write (std::ostream &o) const {
        return expression->write (o << unary_operator<X> () << " ");
    }
}


#endif

