
#include <Diophant/machine.hpp>
#include <Diophant/symbol.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/if.hpp>
#include <Diophant/expressions/let.hpp>

namespace Diophant {

    Expression flatten_calls (Expression &x, symbols registered) {
        if (x.get () == nullptr) return x;

        if (auto pc = std::dynamic_pointer_cast<const expressions::call> (x); pc != nullptr) {
            // first evaluate function and argument.
            expression fun = pc->function;
            list<Expression> args;

            while (true) {
                args = args + data::for_each ([&registered] (Expression &x) -> Expression {
                    return flatten_calls (x, registered);
                }, pc->arguments);

                // check for call
                if (pc = std::dynamic_pointer_cast<const expressions::call> (fun); pc != nullptr) {
                    fun = pc->function;
                } else return make::call (fun, args);
            }
        }

        auto p = x.get ();

        if (auto pb = dynamic_cast<const expressions::binary_expression *> (p); pb != nullptr)
            return make::call (registered.find (binary_operator (pb->op)),
                {flatten_calls (pb->left, registered), flatten_calls (pb->right, registered)});

        /*
        if (auto pu = dynamic_cast<const expressions::left_unary_expression *> (p); pu != nullptr) {
            return make::call (, {apply_to_call (pu->expression, m, fixed)});
        }*/

        if (auto pz = dynamic_cast<const expressions::list *> (p); pz != nullptr) {
            stack<Expression> evaluated;
            for (Expression &e : pz->val) {
                Expression ex = flatten_calls (e, registered);
                evaluated <<= ex;
            }

            return make::list (reverse (evaluated));
        }

        if (auto pm = dynamic_cast<const expressions::map *> (p); pm != nullptr) {
            stack<entry<Expression, Expression>> evaluated;
            for (const entry<Expression, Expression> &e : pm->val) {
                Expression ex = flatten_calls (e.Value, registered);
                evaluated <<= entry<Expression, Expression> {e.Key, ex};
            }

            return make::map (evaluated);
        }

        if (auto pif = dynamic_cast<const expressions::dif *> (p); pif != nullptr)
            return make::dif (
                flatten_calls (pif->Condition, registered),
                flatten_calls (pif->Then, registered),
                flatten_calls (pif->Else, registered));

        if (auto plet = dynamic_cast<const expressions::let *> (p); plet != nullptr)
            throw exception {} << " we don't evaluate let expressions yet";

        return x;
    }

    Expression evaluate (Expression &x, Machine &m, data::set<expressions::symbol> fixed) {
        // evaluate again and again until the result doesn't change
        expression last = flatten_calls (x, m.registered);
        int i = 0;
        while (true) {
            expression next = m.evaluate (last, fixed);
            if (next == last) return last;
            last = next;
        }
    }
}
