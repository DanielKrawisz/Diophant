#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/evaluate.hpp>
#include <Diophant/type.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/apply.hpp>

namespace Diophant {

    struct unknown_construction : unknown_operation {
        unknown_construction (Type &x) : unknown_operation {} {
            *this << "do not know if we can construct type " << x;
        }
    };

    struct parameters {
        cross<pattern> params;
        type such_that;

        parameters (): params {}, such_that {} {}
        parameters (stack<Expression> z, Type st = {});

    };

    intuit equal (const parameters &, const parameters &);
    intuit disjoint (const parameters &, const parameters &);
    intuit sub (const parameters &, const parameters &);

    std::ostream &operator << (std::ostream &, const parameters &);

    struct subject {
        Symbol root;
        Diophant::parameters parameters;

        subject (Symbol &r, const Diophant::parameters &p = {}): root {r}, parameters {p} {}

        static subject read (Expression &);
    };

    std::ostream &operator << (std::ostream &, const subject &);

    struct Machine {
        Expression evaluate (Expression &, data::set<expressions::symbol> fixed);

        void declare (const subject &);
        void define (const subject &, Expression &);

        maybe<replacements> match (parameters &, Expression &) const;
        
        // every cast is valid for now. 
        bool cast (const Type &, const Expression &) const {
            return true;
        }
        
        Machine ();
        struct transformation;

        using overloads = stack<transformation>;

        struct transformation {
            Diophant::parameters key;
            maybe<expression> value;

            overloads more_specific;
        };

        std::map<Symbol, overloads> definitions;
    };

    std::ostream &operator << (std::ostream &, const Machine &);

    std::ostream inline &operator << (std::ostream &o, const parameters &p) {
        for (const auto &e : p.params) e.write (o << " ", call_precedence);
        if (p.such_that.get () != nullptr) o << " /; " << p.such_that;
        return o;
    }

    std::ostream inline &operator << (std::ostream &o, const subject &z) {
        return o << z.root << z.parameters;
    }

}

#endif

