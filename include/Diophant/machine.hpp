#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/pattern.hpp>
#include <Diophant/symbol.hpp>
#include <Diophant/expression.hpp>

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
        list<data::entry<Pattern, Cast>> operator [] (Symbol &) const;

        Machine ();

    private:
        void define (Pattern &, Expression &);
    };

    Expression inline evaluate (Expression &x, Machine &m) {
        return m.evaluate (x);
    }

    maybe<list<entry<Symbol, Expression>>> match (Expression &, Pattern &);

}

#endif

