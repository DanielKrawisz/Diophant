#ifndef DIOPHANT_PATTERN
#define DIOPHANT_PATTERN

#include <Diophant/symbol.hpp>
#include <Diophant/type.hpp>

namespace Diophant {

    struct pattern;
    using Pattern = const pattern;

    bool operator == (Pattern &, Pattern &);
    std::partial_ordering operator <=> (const Pattern &, const Pattern &);
    
    std::ostream &operator << (std::ostream &, Pattern &);

    intuit equal (Pattern, Pattern);
    intuit disjoint (Pattern, Pattern);
    intuit sub (Pattern, Pattern);

}

namespace Diophant::make {
    Pattern any ();
    Pattern var (Symbol &x);
    Pattern such_that (Pattern &, Type &);
}

#endif

