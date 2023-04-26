
#include <Diophant/pattern.hpp>
#include <Diophant/machine.hpp>
#include <Diophant/symbol.hpp>
#include <Diophant/replace.hpp>

namespace Diophant {

    Machine::Machine () {
/*
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
*/
    }

    Expression Machine::evaluate (Expression &x) {
        Symbol h = head (x);

        auto ps = (*this)[h];

        // note: we do nothing with types yet.
        for (auto p : ps) if (auto matches = match (x, p.Key); bool (matches))
            return evaluate (cast (p.Value.type, replace (p.Value.expression, *matches)));

        return x;
    }

}
