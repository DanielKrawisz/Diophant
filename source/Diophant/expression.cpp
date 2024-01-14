#include <Diophant/machine.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/apply.hpp>

namespace Diophant {

    bool operator == (Expression &a, Expression &b) {
        auto ax = a.get ();
        auto bx = b.get ();
        if (ax == bx) return true;
        return a.get () == nullptr ? false : b.get () == nullptr ? false : *a == *b;
    }
}

