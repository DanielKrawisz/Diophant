
#include <Diophant/pattern.hpp>

namespace Diophant {
    
    machine::machine () {
        
        define (Pattern {"null : null"}, Expression::null ());
        define (Pattern {"true : bool"}, Expression::boolean (true));
        define (Pattern {"false : bool"}, Expression::boolean (false));
        
        define (Pattern {"-x.Q : Q"});
        define (Pattern {"x.Q + y.Q : Q"});
        define (Pattern {"x.Q * y.Q : Q"});
        define (Pattern {"x.Q - y.Q : Q"});
        define (Pattern {"x.Q / y.Q : Q"});
        
        define (Pattern {"x.bool || y.bool : bool"});
        define (Pattern {"x.bool && y.bool : bool"});
        define (Pattern {"!x.bool : bool"});
        
        define (Pattern {"x.Q == y.Q : bool"});
        define (Pattern {"x.Q != y.Q : bool"});
        define (Pattern {"x.Q <= y.Q : bool"});
        define (Pattern {"x.Q >= y.Q : bool"});
        define (Pattern {"x.Q < y.Q : bool"});
        define (Pattern {"x.Q > y.Q : bool"});
        
    }
    
    Expression machine::evaluate (Expression x) {
        Symbol h = head (x);
        auto q = Definitions.find (h);
        
        if (q != Definitions.end ()) {
            
        }
        
        
    }
    
}
