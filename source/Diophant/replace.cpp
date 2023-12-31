#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/call.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/symbol.hpp>

namespace Diophant {
    
    Expression replace (Expression expr, Symbol x, Expression replacement) {
        auto p = expr._.get ();
        
        auto to_symbol = dynamic_cast<const expressions::symbol *> (p);
        if (to_symbol != nullptr) return to_symbol->Name == x._->Name ? replacement : expr;
        
        auto to_call = dynamic_cast<const expressions::call *> (p);
        if (to_call != nullptr) return call(replace(to_call->Function, x, replacement), replace(to_call->Argument, x, replacement));
        
        auto to_list = dynamic_cast<const expressions::list *> (p);
        if (to_list != nullptr) {
            auto list = new expressions::list ({});
            for (Expression e : to_list->val) list->val = list->val << replace(e, x, replacement);
            expression r;
            r._.reset(list);
            return r;
        }

        auto to_lambda = dynamic_cast<const expressions::lambda *> (p);
        if (to_lambda != nullptr && to_lambda->Argument != x) 
            return expression::lambda (to_lambda->Argument, replace (to_lambda->Body, x, replacement));
        
        return expr;
        
    }
}
