#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/expression.hpp>

namespace Diophant {

    struct Machine {
        Expression evaluate (Expression &);
    };

    Expression inline evaluate (Expression &x, Machine &m) {
        return m.evaluate (x);
    }


}

#endif

