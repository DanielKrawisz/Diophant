#ifndef DIOPHANT_EXPRESSIONS_NULL
#define DIOPHANT_EXPRESSIONS_NULL

#include <Diophant/expressions/expressions.hpp>

namespace Diophant::expressions {
    
    struct null final : abstract {
        null () {}
        
        std::ostream &write (std::ostream &) const override;
    };
}

namespace Diophant::make {
    
    Expression inline null (bool b) {
        return expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::null> ())};
    }
    
}

namespace Diophant::expressions {
    
    std::ostream inline &null::write (std::ostream &o) const {
        return o << "null";
    }
    
}

#endif

