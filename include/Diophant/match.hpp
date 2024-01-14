#ifndef DIOPHANT_MATCH
#define DIOPHANT_MATCH

#include <Diophant/pattern.hpp>
#include <Diophant/replace.hpp>

namespace Diophant {

    maybe<replacements> match (Pattern &, Expression &, data::set<expressions::symbol> fixed = {});

    maybe<replacements> combine (maybe<replacements>, maybe<replacements>);

}

#endif

