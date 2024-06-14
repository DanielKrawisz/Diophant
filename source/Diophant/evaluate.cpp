
#include <Diophant/machine.hpp>
#include <Diophant/symbol.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/if.hpp>
#include <Diophant/expressions/let.hpp>

namespace Diophant {

    Expression flatten_calls (Expression &x) {
        if (x.get () == nullptr) return x;

        if (auto pc = std::dynamic_pointer_cast<const expressions::call> (x); pc != nullptr) {
            // first evaluate function and argument.
            expression fun = pc->function;
            list<Expression> args;

            while (true) {
                args = args + data::for_each (flatten_calls, pc->arguments);

                // check for call
                if (pc = std::dynamic_pointer_cast<const expressions::call> (fun); pc != nullptr) {
                    fun = pc->function;
                } else return make::call (fun, args);
            }
        }

        if (auto pb = std::dynamic_pointer_cast<const expressions::binary_expression> (x); pb != nullptr) {
            return make::call (make::symbol (binary_operator (pb->op)),
                {flatten_calls (pb->left), flatten_calls (pb->right)});
        }

        auto p = x.get ();
        /*
        if (auto pu = dynamic_cast<const expressions::left_unary_expression *> (p); pu != nullptr) {
            return make::call (, {apply_to_call (pu->expression, m, fixed)});
        }*/

        if (auto pz = dynamic_cast<const expressions::list *> (p); pz != nullptr) {
            stack<Expression> evaluated;
            for (Expression &e : pz->val) {
                Expression ex = flatten_calls (e);
                evaluated <<= ex;
            }

            return make::list (reverse (evaluated));
        }

        if (auto pm = dynamic_cast<const expressions::map *> (p); pm != nullptr) {
            stack<entry<Expression, Expression>> evaluated;
            for (const entry<Expression, Expression> &e : pm->val) {
                Expression ex = flatten_calls (e.Value);
                evaluated <<= entry<Expression, Expression> {e.Key, ex};
            }

            return make::map (evaluated);
        }

        if (auto pif = dynamic_cast<const expressions::dif *> (p); pif != nullptr)
            return make::dif (
                flatten_calls (pif->Condition),
                flatten_calls (pif->Then),
                flatten_calls (pif->Else));

        if (auto plet = dynamic_cast<const expressions::let *> (p); plet != nullptr)
            throw exception {} << " we don't evaluate let expressions yet";

        return x;
    }

    Expression evaluate (Expression &x, Machine &m, data::set<symbol> fixed) {
        // evaluate again and again until the result doesn't change
        expression last = flatten_calls (x);
        int i = 0;
        while (true) {
            expression next = m.evaluate (last, fixed);
            if (next == last) return last;
            last = next;
        }
    }
}
