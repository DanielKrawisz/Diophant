#include <Diophant/expressions/pattern.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant::expressions {
    
    bool var::operator == (const abstract &a) const {
        try {
            auto x = dynamic_cast<const var &> (a);
            return x.name == name ? true : x.name == nullptr || name == nullptr ? false : *x.name == *name;
        } catch (std::bad_cast) {
            return false;
        }
    }

    bool such_that::operator == (const abstract &a) const {
        try {
            auto x = dynamic_cast<const such_that &> (a);
            return pattern == x.pattern && type == x.type;
        } catch (std::bad_cast) {
            return false;
        }
    }
}
