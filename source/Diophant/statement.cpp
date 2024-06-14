
#include <Diophant/statement.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/pattern.hpp>

namespace Diophant {

    subject::subject (Expression &x) {
        if (x.get () == nullptr) goto fail;

        if (auto px = std::dynamic_pointer_cast<const expressions::symbol> (x); px != nullptr) {
            *this = subject (*px);
            return;
        } else if (auto pa = std::dynamic_pointer_cast<const expressions::call> (x); pa != nullptr) {
            expression fun = pa->function;
            list<Expression> arguments = pa->arguments;

            while (true) {
                if (const auto fx = std::dynamic_pointer_cast<const expressions::symbol> (fun); bool (fx)) {
                    *this = subject {*fx, Diophant::parameters {arguments}};
                    return;
                }

                if (const auto fa = std::dynamic_pointer_cast<const expressions::call> (fun); bool (fa)) {
                    fun = fa->function;
                    arguments = fa->arguments + arguments;
                    continue;
                }

                goto fail;
            }
        }

        if (auto pst = std::dynamic_pointer_cast<const expressions::such_that> (x); pst != nullptr) {
            *this = subject {pst->pattern};
            this->parameters.such_that = pst->type;
            return;
        }

        if (auto pb = std::dynamic_pointer_cast<const expressions::binary_expression> (x); pb != nullptr) {
            auto v = make::symbol (binary_operator (pb->op));
            if (! bool (v)) throw exception {} << " unknown binary operator " << binary_operator (pb->op);
            *this = subject {v};
            this->parameters.params.resize (2);
            this->parameters.params[0] = pb->left;
            this->parameters.params[1] = pb->right;
            return;
        }

        if (auto pu = std::dynamic_pointer_cast<const expressions::left_unary_expression> (x); pu != nullptr)
            throw exception {} << "not yet implemented: define " << x;

        fail:
        throw exception {} << "cannot define " << x;
    }
}
