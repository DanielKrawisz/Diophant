#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/symbol.hpp>

namespace Diophant {
    
    Expression replace (Expression x, replacements r) {

        auto p = x.get ();
        if (p == nullptr) return x;

        if (auto px = static_cast<const expressions::symbol *> (p); px != nullptr) {
            auto e = r.contains (*px);
            if (!e) return x;
            return *e;
        }
        
        if (auto pc = static_cast<const expressions::call *> (p); pc != nullptr) {
            auto fun = replace (pc->function, r);
            auto arg = replace (pc->argument, r);

            if (fun == pc->function && arg == pc->argument) return x;
            return make::call (fun, arg);
        }
        
        if (auto pb = static_cast<const expressions::binary_expression *> (p); pb != nullptr) {
            auto left = replace (pb->left, r);
            auto right = replace (pb->right, r);

            return left == pb->left && right == pb->right ? x : expressions::binary_expression::make (pb->op, left, right);
        }
        
        if (auto pu = static_cast<const expressions::unary_expression *> (p);pu != nullptr) {
            std::cout << " testing u" << std::endl;
            auto expr = replace (pu->expression, r);

            return expr == pu->expression ? x : expressions::unary_expression::make (pu->op, expr);
        }
        
        if (auto pz = dynamic_cast<const expressions::list *> (p); pz != nullptr) {
            stack<Expression> evaluated;
            bool changed = false;
            for (Expression &e : pz->val) {
                Expression ex = replace (e, r);
                if (ex != e) changed = true;
                evaluated <<= ex;
            }

            return changed ? make::list (evaluated) : x;
        }
        
        if (auto pm = dynamic_cast<const expressions::map *> (p); pm != nullptr) {
            stack<entry<Expression, Expression>> evaluated;
            bool changed = false;
            for (const entry<Expression, Expression> &e : pm->val) {
                Expression ex = replace (e.Value, r);
                if (ex != e.Value) changed = true;
                evaluated <<= entry<Expression, Expression> {e.Key, ex};
            }

            return changed ? make::map (evaluated) : x;
        }

        return x;
    }
}
