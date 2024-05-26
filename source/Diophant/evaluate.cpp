
#include <Diophant/machine.hpp>
#include <data/io/wait_for_enter.hpp>

namespace Diophant {
    Expression evaluate (Expression &x, Machine &m, data::set<expressions::symbol> fixed) {
        // evaluate again and again until the result doesn't change
        expression last = x;
        int i = 0;
        while (true) {
            expression next = m.evaluate (last, fixed);
            if (next == last) return last;
            last = next;
            wait_for_enter ();
        }
    }
}
