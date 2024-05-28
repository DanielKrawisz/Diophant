#ifndef DIOPHANT_MATCH
#define DIOPHANT_MATCH

#include <Diophant/pattern.hpp>
#include <Diophant/replace.hpp>

namespace Diophant {
    
    // thrown in the case of a cast that we don't know how to make.
    struct unknown_cast : exception {
        unknown_cast (Expression &to, Expression &from) : exception {} {
            *this << "unknown cast from " << from << " to " << to;
        }
    };

    maybe<replacements> match (Pattern &, Expression &, data::set<expressions::symbol> fixed = {});

    maybe<replacements> combine (maybe<replacements>, maybe<replacements>);

}

#endif

