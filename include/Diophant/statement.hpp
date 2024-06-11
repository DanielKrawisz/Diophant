#ifndef DIOPHANT_STATEMENT
#define DIOPHANT_STATEMENT

#include <Diophant/data.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/pattern.hpp>
#include <Diophant/symbol.hpp>

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
        parameters (list<Expression> z, Type st = {});
        
        bool operator == (const parameters &) const;

    };

    intuitionistic_partial_ordering operator <=> (const parameters &, const parameters &);

    std::ostream &operator << (std::ostream &, const parameters &);

    struct subject {
        Symbol root;
        Diophant::parameters parameters;

        subject (Symbol &r, const Diophant::parameters &p = {}): root {r}, parameters {p} {}
    };
    
    std::ostream &operator << (std::ostream &, const subject &x);
    
    struct statement {
        Diophant::subject subject;
        Expression predicate;
    };
    
    std::ostream &operator << (std::ostream &o, const statement &x);
    
    std::ostream inline &operator << (std::ostream &o, const subject &x) {
        return o << x.root << " " << x.parameters;
    }
    
    std::ostream inline &operator << (std::ostream &o, const statement &x) {
        return o << x.subject << " := " << x.predicate;
    }
}


#endif

