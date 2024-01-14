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
    
    constexpr const char *unary_operator (unary_operand X) {
        switch (X) {
            case unary_operand::NOT : return "!";
            case unary_operand::NEGATE : return "-";
            case unary_operand::PLUS : return "+";
            default: throw exception {} << "can't get here";
        }
    }
    
    struct unary_expression final : abstract {
        
        uint32 precedence () const override {
            return 200;
        }
        
        unary_operand op;
        Expression expression;
        unary_expression (unary_operand o, Expression &x) : op {o}, expression {x} {}
        
        static Expression make (unary_operand o, Expression &);
        
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
    };
}

namespace Diophant::make {
    
    Expression inline boolean_not (Expression &x) {
        return expressions::unary_expression::make (expressions::unary_operand::NOT, x);
    }
    
    Expression inline negate (Expression &x) {
        return expressions::unary_expression::make (expressions::unary_operand::NEGATE, x);
    }

    Expression inline plus (Expression &x) {
        return expressions::unary_expression::make (expressions::unary_operand::PLUS, x);
    }
    
}

namespace Diophant::expressions {
    
    Expression inline unary_expression::make (unary_operand op, Expression &x) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<unary_expression> (op, x))};
    }
    
    std::ostream inline &unary_expression::write (std::ostream &o) const {
        return expression->write (o << unary_operator (op) << " ");
    }

    bool inline unary_expression::operator == (const abstract &a) const {
        try {
            return expression == dynamic_cast<const unary_expression &> (a).expression;
        } catch (std::bad_cast) {
            return false;
        }
    }
}


#endif

