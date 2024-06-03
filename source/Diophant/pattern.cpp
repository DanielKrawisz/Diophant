
#include <Diophant/match.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant {
    
    bidirectional_replacements compare (Pattern &a, Pattern &b, bidirectional_replacements br, 
        data::set<expressions::symbol> fixed_left, data::set<expressions::symbol> fixed_right) {
        
        return bidirectional_replacements {
            match (a, b, br.left, fixed_left), 
            match (b, a, br.right, fixed_right)
        };
    }

}
