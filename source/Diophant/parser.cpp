#include <Diophant/parser.hpp>

namespace Diophant {
    
    void Parser::declare () {
        machine.declare (Pattern {first (rest (stack))}, Type {first (stack)});
        stack = rest (stack);
    }
    
    void Parser::set () {
        machine.define (Pattern {first (rest (stack))}, first (stack));
        stack = prepend (rest (rest (stack)), first (stack));
    }
    
    void Parser::infer () {
        Pattern p {first (rest (stack))};
        machine.declare (p, Type {});
        machine.define (p, first (stack));
        stack = prepend (rest (rest (stack)), first (stack));
    }
}
