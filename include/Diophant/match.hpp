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

    replacements match (Pattern &, Expression &, replacements prior = {{}}, data::set<expressions::symbol> fixed = {});

    replacements combine (replacements, replacements);

    intuit constructable (Type &);

}

#endif

