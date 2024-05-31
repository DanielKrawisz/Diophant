
#include <Diophant/pattern.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant {

    bool operator == (Pattern &, Pattern &) {
        throw method::unimplemented {"pattern == "};
    }

    std::ostream &operator << (std::ostream &, Pattern &) {
        throw method::unimplemented {"pattern << "};
    }

}
