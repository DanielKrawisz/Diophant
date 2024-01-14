#include <Diophant/parser.hpp>

namespace Diophant {

    void Parser::arrow () {
        Expression &x = first (rest (stack));
        auto z = dynamic_cast<Symbol *> (x.get ());
        if (z == nullptr) throw exception {} << "expected symbol, found " << x;
        stack = prepend (rest (rest (stack)), make::lambda (*z, first (stack)));
    }

    void Parser::open_list () {
        back = stack;
        stack = data::stack<Expression> {};
    }

    void Parser::open_object () {
        back = stack;
        stack = data::stack<Expression> {};
    }

    void Parser::close_list () {
        stack = prepend (first (back), make::list (stack));
        back = rest (back);
    }

    void Parser::close_object () {
        data::stack<entry<Expression, Expression>> m;
        while (data::size (stack) > 0) m <<= entry<Expression, Expression> {first (rest (stack)), first (stack)};
        stack = prepend (first (back), make::map (m));
        back = rest (back);
    }

    void Parser::declare () {
        machine.declare (subject::read (first (rest (stack))), Type {first (stack)});
        stack = rest (stack);
    }
    
    void Parser::set () {
        machine.define (subject::read (first (rest (stack))), first (stack));
        stack = prepend (rest (rest (stack)), first (stack));
    }
    
    void Parser::infer () {
        auto x = first (stack);
        machine.define (subject::read (first (rest (stack))), predicate {type_of (x), x});
        stack = prepend (rest (rest (stack)), first (stack));
    }
}
