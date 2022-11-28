#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/call.hpp>

namespace Diophant {
    // a lambda is valid if every sub expression is 
    // either a lambda, a call, or bound symbol. 
    bool valid_lambda(Expression x, data::stack<Symbol> bound) {
        if (x == expression::null()) return false;
        
        auto to_symbol = Symbol(x);
        if (to_symbol != expression::null()) return data::contains(bound, to_symbol);
        
        auto to_lambda = std::dynamic_pointer_cast<const expressions::lambda>(x._);
        if (to_lambda != nullptr) 
            return valid_lambda(to_lambda->Body, bound << to_lambda->Argument);
        
        auto to_call = std::dynamic_pointer_cast<const expressions::call>(x._);
        if (to_call != nullptr) 
            return valid_lambda(to_call->Function, bound) && 
                valid_lambda(to_call->Argument, bound);
        
        return false;
    }
    
    bool lambda::valid() const {
        return valid_lambda(*this, data::stack<Symbol>{});
    }
    
    std::istream &operator>>(std::istream &i, lambda &l) {
        expression e; 
        i >> e;
        l = lambda(e);
        return i;
    }
    
}

