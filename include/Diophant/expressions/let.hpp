#ifndef DIOPHANT_EXPRESSIONS_LET
#define DIOPHANT_EXPRESSIONS_LET

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/pattern.hpp>
#include <Diophant/statement.hpp>

namespace Diophant::expressions {
    
    struct let final : abstract {
        data::stack<statement> Let;
        Expression In;
        
        let (data::stack<statement> xx, Expression x): Let {xx}, In {x} {}
        
        bool operator == (const abstract &) const final override {
            throw exception {} << " let == not yet implemented ";
        }
        
        std::ostream &write (std::ostream &o) const final override {
            auto b = Let.begin ();
            o << *b;
            while (true) {
                b++; 
                if (b == Let.end ()) break;
                o << ", " << *b;
            }
            return o << " in " << In;
        }
    };
    
}

namespace Diophant::make {
    Expression inline let (data::stack<statement> xx, Expression x) {
        return expression {std::static_pointer_cast<const expressions::abstract> (std::make_shared<const expressions::let> (xx, x))};
    }
}

#endif
