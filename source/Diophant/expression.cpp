#include <Diophant/machine.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/parser.hpp>

namespace Diophant {

    bool operator == (Expression &a, Expression &b) {
        auto ax = a.get ();
        auto bx = b.get ();
        if (ax == bx) return true;
        return a.get () == nullptr ? false : b.get () == nullptr ? false : *a == *b;
    }

    bool valid_expr (Expression &e) {
        const auto *x = e.get ();
        
        if (x == nullptr) return true;
        if (auto b = dynamic_cast<const expressions::boolean *> (x); b != nullptr) return true;
        if (auto b = dynamic_cast<const expressions::natural *> (x); b != nullptr) return true;
        if (auto b = dynamic_cast<const expressions::integer *> (x); b != nullptr) return true;
        if (auto b = dynamic_cast<const expressions::rational *> (x); b != nullptr) return true;
        if (auto b = dynamic_cast<const expressions::string *> (x); b != nullptr) return true;
        
        if (auto b = dynamic_cast<const expressions::list *> (x); b != nullptr) {
            for (Expression &n : b->val) if (!valid_expr (n)) return false;
            return true;
        }
        
        if (auto b = dynamic_cast<const expressions::map *> (x); b != nullptr) {
            for (const entry<Expression, Expression> &n : b->val) 
                if (!valid_expr (n.Key) && valid_expr (n.Value)) return false;
            return true;
        }
        
        if (auto b = dynamic_cast<const expressions::lambda *> (x); b != nullptr) 
            return valid_expr (b->body);
        
        if (auto b = dynamic_cast<const expressions::call *> (x); b != nullptr) 
            return valid_expr (b->function) && valid_expr (b->argument);
        
        if (auto b = dynamic_cast<const expressions::symbol *> (x); b != nullptr)
            return true;
        
        return false;
        
    }

    // an expression is valid if it is not a pattern.
    bool expression::valid () const {
        return valid_expr (*this);
    }
    
    // parse some code and read in an expression.
    expression::expression (const string &x) {
        Parser p {};
        *this = p.read_expression (x);
    }
}

