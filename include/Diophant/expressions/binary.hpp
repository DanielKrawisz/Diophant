#ifndef DIOPHANT_EXPRESSIONS_BINARY
#define DIOPHANT_EXPRESSIONS_BINARY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    enum class binary_operand : byte {
        APPLY = 10,
        PLUS = 30, 
        MINUS = 40, 
        TIMES = 50, 
        POWER = 60, 
        DIVIDE = 70,
        EQUAL = 80,
        UNEQUAL = 81, 
        GREATER_EQUAL = 82, 
        LESS_EQUAL = 83, 
        GREATER = 84, 
        LESS = 85, 
        BOOLEAN_AND = 90, 
        BOOLEAN_OR = 100, 
        ARROW = 110, 
        INTUITIONISTIC_AND = 120, 
        INTUITIONISTIC_OR = 130, 
        INTUITIONISTIC_IMPLIES = 140
    };
    
    template <binary_operand X>
    constexpr uint32 binary_precedence () {
        return 100 * uint32 (X);
    }
    
    template <binary_operand X>
    struct binary_expression final : abstract {
        
        uint32 precedence () const override {
            return binary_precedence<X> ();
        }
        
        Expression left;
        Expression right;
        binary_expression (Expression &l, Expression &r) : left {l}, right {r} {}
        
        static Expression make (Expression &, Expression &);
    };
    
    using apply = binary_expression<binary_operand::APPLY>;
    
    using plus = binary_expression<binary_operand::PLUS>;
    using minus = binary_expression<binary_operand::MINUS>;
    using times = binary_expression<binary_operand::TIMES>;
    using power = binary_expression<binary_operand::POWER>;
    using divide = binary_expression<binary_operand::DIVIDE>;
    
    using equal = binary_expression<binary_operand::EQUAL>;
    using unequal = binary_expression<binary_operand::UNEQUAL>;
    using greater_equal = binary_expression<binary_operand::GREATER_EQUAL>;
    using less_equal = binary_expression<binary_operand::LESS_EQUAL>;
    using greater = binary_expression<binary_operand::GREATER>;
    using less = binary_expression<binary_operand::LESS>;
    
    using boolean_and = binary_expression<binary_operand::BOOLEAN_AND>;
    using boolean_or = binary_expression<binary_operand::BOOLEAN_OR>;
    using arrow = binary_expression<binary_operand::ARROW>;
    
    using intuitionistic_and = binary_expression<binary_operand::INTUITIONISTIC_AND>;
    using intuitionistic_or = binary_expression<binary_operand::INTUITIONISTIC_OR>;
    using intuitionistic_implies = binary_expression<binary_operand::INTUITIONISTIC_IMPLIES>;
    
}

namespace Diophant::make {
    
    Expression inline apply (Expression &l, Expression &r) {
        return expressions::apply::make (l, r);
    }
    
    Expression inline plus (Expression &l, Expression &r) {
        return expressions::plus::make (l, r);
    }
    
    Expression inline minus (Expression &l, Expression &r) {
        return expressions::minus::make (l, r);
    }
    
    Expression inline times (Expression &l, Expression &r) {
        return expressions::times::make (l, r);
    }
    
    Expression inline power (Expression &l, Expression &r) {
        return expressions::power::make (l, r);
    }
    
    Expression inline divide (Expression &l, Expression &r) {
        return expressions::divide::make (l, r);
    }
    
    Expression inline equal (Expression &l, Expression &r) {
        return expressions::equal::make (l, r);
    }
    
    Expression inline unequal (Expression &l, Expression &r) {
        return expressions::unequal::make (l, r);
    }
    
    Expression inline greater_equal (Expression &l, Expression &r) {
        return expressions::greater_equal::make (l, r);
    }
    
    Expression inline less_equal (Expression &l, Expression &r) {
        return expressions::less_equal::make (l, r);
    }
    
    Expression inline greater (Expression &l, Expression &r) {
        return expressions::greater::make (l, r);
    }
    
    Expression inline less (Expression &l, Expression &r) {
        return expressions::less::make (l, r);
    }
    
    Expression inline boolean_and (Expression &l, Expression &r) {
        return expressions::boolean_and::make (l, r);
    }
    
    Expression inline boolean_or (Expression &l, Expression &r) {
        return expressions::boolean_or::make (l, r);
    }
    
    Expression inline arrow (Expression &l, Expression &r) {
        return expressions::arrow::make (l, r);
    }
    
    Expression inline intuitionistic_and (Expression &l, Expression &r) {
        return expressions::intuitionistic_and::make (l, r);
    }
    
    Expression inline intuitionistic_or (Expression &l, Expression &r) {
        return expressions::intuitionistic_or::make (l, r);
    }
    
    Expression inline intuitionistic_implies (Expression &l, Expression &r) {
        return expressions::intuitionistic_implies::make (l, r);
    }
    
}

namespace Diophant::expressions {
    
    template <binary_operand X>
    Expression inline binary_expression<X>::make (Expression &l, Expression &r) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<binary_expression<X>> (l, r))};
    }
}


#endif

