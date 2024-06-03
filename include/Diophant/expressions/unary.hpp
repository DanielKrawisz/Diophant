#ifndef DIOPHANT_EXPRESSIONS_UNARY
#define DIOPHANT_EXPRESSIONS_UNARY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>

namespace Diophant::expressions {
    
    enum class left_unary_operand : char {
        NOT = '!', 
        NEGATE = '-', 
        PLUS = '+',
        TILDE = '~',
        STAR = '*'
    };
    
    constexpr const char *left_unary_operator (left_unary_operand X) {
        switch (X) {
            case left_unary_operand::NOT : return "!";
            case left_unary_operand::NEGATE : return "-";
            case left_unary_operand::PLUS : return "+";
            case left_unary_operand::TILDE : return "~";
            case left_unary_operand::STAR : return "*";
            default: throw exception {} << "Attempt to create left unary operand " << char (X);
        }
    }
    
    struct left_unary_expression final : abstract {
        
        uint32 precedence () const override {
            return 200;
        }
        
        left_unary_operand op;
        Expression expression;
        left_unary_expression (left_unary_operand o, Expression &x) : op {o}, expression {x} {}
        
        static Expression make (left_unary_operand o, Expression &);
        
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
    };
}

namespace Diophant::make {
    
    Expression inline boolean_not (Expression &x) {
        return expressions::left_unary_expression::make (expressions::left_unary_operand::NOT, x);
    }
    
    Expression inline negate (Expression &x) {
        return expressions::left_unary_expression::make (expressions::left_unary_operand::NEGATE, x);
    }

    Expression inline plus (Expression &x) {
        return expressions::left_unary_expression::make (expressions::left_unary_operand::PLUS, x);
    }
    
}

namespace Diophant::expressions {
    
    Expression inline left_unary_expression::make (left_unary_operand op, Expression &x) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<left_unary_expression> (op, x))};
    }
    
    std::ostream inline &left_unary_expression::write (std::ostream &o) const {
        return expression->write (o << left_unary_operator (op) << " ");
    }

    bool inline left_unary_expression::operator == (const abstract &a) const {
        try {
            return expression == dynamic_cast<const left_unary_expression &> (a).expression;
        } catch (std::bad_cast) {
            return false;
        }
    }
}


#endif

