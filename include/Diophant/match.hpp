#ifndef DIOPHANT_MATCH
#define DIOPHANT_MATCH

#include <Diophant/pattern.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/type.hpp>

namespace Diophant {

    struct unknown_cast : unknown_operation {
        unknown_cast (Expression &to, Type &from) : unknown_operation {} {
            *this << "unknown cast from " << from << " to " << to;
        }
    };

    maybe<replacements> match (Pattern &, Expression &, data::set<expressions::symbol> fixed = {});

    maybe<replacements> combine (maybe<replacements>, maybe<replacements>);

    intuit constructable (Type &);

}

#endif

