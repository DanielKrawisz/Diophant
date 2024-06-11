#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/symbol.hpp>

namespace Diophant {
    
    Expression zplace (Expression x, map<expressions::symbol, Expression> r) {
        if (x.get () == nullptr) return x;

        if (auto px = std::dynamic_pointer_cast<const expressions::symbol> (x); px != nullptr) {
            auto e = r.contains (*px);
            if (!e) return x;
            return *e;
        }
        
        if (auto pc = std::dynamic_pointer_cast<const expressions::call> (x); pc != nullptr) {
            auto fun = zplace (pc->function, r);
            auto args = data::for_each ([&r] (Expression &z) -> Expression { zplace (z, r); }, pc->arguments);
            if (fun == pc->function && args == pc->arguments) return x;
            return make::call (fun, args);
        }
        
        if (auto pb = std::dynamic_pointer_cast<const expressions::binary_expression> (x); pb != nullptr) {
            auto left = zplace (pb->left, r);
            auto right = zplace (pb->right, r);

            return left == pb->left && right == pb->right ? x : expressions::binary_expression::make (pb->op, left, right);
        }
        
        if (auto pu = std::dynamic_pointer_cast<const expressions::left_unary_expression> (x);pu != nullptr) {
            auto expr = zplace (pu->expression, r);

            return expr == pu->expression ? x : expressions::left_unary_expression::make (pu->op, expr);
        }
        
        if (auto pz = std::dynamic_pointer_cast<const expressions::list> (x); pz != nullptr) {
            stack<Expression> evaluated;
            bool changed = false;
            for (Expression &e : pz->val) {
                Expression ex = zplace (e, r);
                if (ex != e) changed = true;
                evaluated <<= ex;
            }

            return changed ? make::list (evaluated) : x;
        }
        
        if (auto pm = std::dynamic_pointer_cast<const expressions::map> (x); pm != nullptr) {
            stack<entry<Expression, Expression>> evaluated;
            bool changed = false;
            for (const entry<Expression, Expression> &e : pm->val) {
                Expression ex = zplace (e.Value, r);
                if (ex != e.Value) changed = true;
                evaluated <<= entry<Expression, Expression> {e.Key, ex};
            }

            return changed ? make::map (evaluated) : x;
        }

        return x;
    }
    
    Expression replace (Expression x, replacements r) {
        if (!bool (r)) return x;

        return zplace (x, *r);
    }
}
