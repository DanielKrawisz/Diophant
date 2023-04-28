#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/pattern.hpp>

namespace Diophant {

    // throws an exception if the cast is not valid. For now every cast is valid.
    Expression inline cast (const Type &, Expression &x) {
        return x;
    }

    struct Cast {
        Type type;
        Expression expression;
    };

    struct Machine {
        Expression evaluate (Expression &);
        list<entry<Pattern, Cast>> operator [] (Symbol &) const;
        
        void declare (Pattern, Type);
        void define (Pattern, Expression);
        
        Machine ();

    };

    Expression inline evaluate (Expression &x, Machine &m) {
        return m.evaluate (x);
    }

    maybe<list<entry<Symbol, Expression>>> match (Expression &, Pattern &);

}

#endif

