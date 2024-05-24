
#include <Diophant/expressions/apply.hpp>
#include <Diophant/pattern.hpp>

namespace Diophant::expressions {
    /*
    call::operator Pattern () const {
        auto x = std::dynamic_pointer_cast<const symbol> (function); 
        if (x.get () != nullptr) return pattern {*symbol, argument};
        
        auto a = std::dynamic_pointer_cast<const apply> (function); 
        if (a.get () != nullptr) {
            auto p = Pattern (a);
            return pattern {p.symbol, p.parameters << argument}
        };
        
        return pattern {};
    }*/
    
}

