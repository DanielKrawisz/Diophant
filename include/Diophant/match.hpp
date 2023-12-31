#ifndef DIOPHANT_MATCH
#define DIOPHANT_MATCH

#include <Diophant/pattern.hpp>
#include <Diophant/replace.hpp>

namespace Diophant {

    maybe<replacements> match (Pattern, Expression);

}

#endif

