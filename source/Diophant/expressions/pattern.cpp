#include <Diophant/expressions/pattern.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant::expressions {
    
    bool var::operator == (const abstract &) const {
        throw method::unimplemented {"typed =="};
    }

    bool such_that::operator == (const abstract &) const {
        throw method::unimplemented {"typed =="};
    }
}
