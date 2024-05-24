#include <Diophant/expressions/pattern.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant::expressions {
    
    bool typed::operator == (const abstract &) const {
        throw method::unimplemented {"typed =="};
    }
}
