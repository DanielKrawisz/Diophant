#include <Diophant/expressions/pattern.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant::expressions {
    
    bool var::operator == (const abstract &a) const {
        try {
            return name == dynamic_cast<const var &> (a).name;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
    bool typed::operator == (const abstract &a) const {
        try {
            return type == dynamic_cast<const typed &> (a).type;
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
