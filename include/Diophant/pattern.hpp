#ifndef DIOPHANT_PATTERN
#define DIOPHANT_PATTERN

#include <Diophant/symbol.hpp>
#include <Diophant/type.hpp>
#include <Diophant/replace.hpp>

namespace Diophant {

    struct pattern;
    using Pattern = const pattern;
    
    std::ostream &operator << (std::ostream &, Pattern &);

    struct pattern : expression {
        pattern (Expression &ex) : expression {ex} {}
        pattern () : expression {} {}
    };
    
    struct bidirectional_replacements {
        replacements left;
        replacements right;
        
        bidirectional_replacements (): left {}, right {} {}
        bidirectional_replacements (replacements l, replacements r): left {l}, right {r} {}
        
        operator bool () const {
            return bool (left) && bool (right);
        }
        
        operator std::partial_ordering () const;
    };
    
    bidirectional_replacements compare (Pattern &, Pattern &, bidirectional_replacements = {{{}}, {{}}}, 
        data::set<expressions::symbol> fixed_left = {}, data::set<expressions::symbol> fixed_right = {});

}

namespace Diophant::make {
    Pattern any ();
    Pattern var (Symbol &x);
    Pattern such_that (Pattern &, Type &);
}

namespace Diophant {

    std::ostream inline &operator << (std::ostream &o, Pattern &p) {
        return o << static_cast<Expression &> (p);
    }

}

#endif

