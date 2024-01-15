#ifndef DIOPHANT_EXPRESSIONS_BINARY
#define DIOPHANT_EXPRESSIONS_BINARY

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    enum class binary_operand : byte {
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

    constexpr uint32 binary_precedence (binary_operand X) {
        return 100 * uint32 (X);
    }
    
    constexpr const char *binary_operator (binary_operand X) {
        switch (X) {
            case binary_operand::PLUS : return "+";
            case binary_operand::MINUS : return "-";
            case binary_operand::TIMES : return "*";
            case binary_operand::POWER : return "^";
            case binary_operand::DIVIDE : return "/";
            case binary_operand::EQUAL : return "==";
            case binary_operand::UNEQUAL : return "!=";
            case binary_operand::GREATER_EQUAL : return ">";
            case binary_operand::LESS_EQUAL : return "<=";
            case binary_operand::GREATER : return ">";
            case binary_operand::LESS : return "<";
            case binary_operand::BOOLEAN_AND : return "&&";
            case binary_operand::BOOLEAN_OR : return "||";
            case binary_operand::ARROW : return "->";
            case binary_operand::INTUITIONISTIC_AND : return "&";
            case binary_operand::INTUITIONISTIC_OR : return "|";
            case binary_operand::INTUITIONISTIC_IMPLIES : return "=>";
            default : return "";
        }
    }
    
    struct binary_expression final : abstract {
        
        uint32 precedence () const override {
            return binary_precedence (op);
        }

        binary_operand op;
        Expression left;
        Expression right;
        binary_expression (binary_operand x, Expression &l, Expression &r) : op {x}, left {l}, right {r} {}
        
        static Expression make (binary_operand op, Expression &, Expression &);
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
    };
    
}

namespace Diophant::make {
    
    Expression inline plus (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::PLUS, l, r);
    }
    
    Expression inline minus (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::MINUS, l, r);
    }
    
    Expression inline times (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::TIMES, l, r);
    }
    
    Expression inline power (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::POWER, l, r);
    }
    
    Expression inline divide (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::DIVIDE, l, r);
    }
    
    Expression inline equal (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::EQUAL, l, r);
    }
    
    Expression inline unequal (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::UNEQUAL, l, r);
    }
    
    Expression inline greater_equal (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::GREATER_EQUAL, l, r);
    }
    
    Expression inline less_equal (Expression &l, Expression &r) {
        using namespace expressions;
        return expressions::binary_expression::make (binary_operand::LESS_EQUAL, l, r);
    }
    
    Expression inline greater (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::GREATER, l, r);
    }
    
    Expression inline less (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::LESS, l, r);
    }
    
    Expression inline boolean_and (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::BOOLEAN_AND, l, r);
    }
    
    Expression inline boolean_or (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::BOOLEAN_OR, l, r);
    }
    
    Expression inline arrow (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::ARROW, l, r);
    }
    
    Expression inline intuitionistic_and (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::INTUITIONISTIC_AND, l, r);
    }
    
    Expression inline intuitionistic_or (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::INTUITIONISTIC_OR, l, r);
    }
    
    Expression inline intuitionistic_implies (Expression &l, Expression &r) {
        using namespace expressions;
        return binary_expression::make (binary_operand::INTUITIONISTIC_IMPLIES, l, r);
    }
    
}

namespace Diophant::expressions {
    
    Expression inline binary_expression::make (binary_operand op, Expression &l, Expression &r) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<binary_expression> (op, l, r))};
    }
    
    std::ostream inline &binary_expression::write (std::ostream &o) const {
        return write_parens (
            write_parens (o, 
                *left, 
                left->precedence () > this->precedence ()) << " " << binary_operator (op) << " ",
            *right, 
            right->precedence () > this->precedence ());
    }

    bool inline binary_expression::operator == (const abstract &a) const {
        try {
            const binary_expression &b = dynamic_cast<const binary_expression &> (a);
            return op == b.op && left == b.left && right == b.right;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
}

#endif
