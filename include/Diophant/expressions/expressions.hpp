#ifndef DIOPHANT_EXPRESSIONS_EXPRESSIONS
#define DIOPHANT_EXPRESSIONS_EXPRESSIONS

#include <Diophant/types.hpp>

namespace Diophant::expressions {
    
    struct abstract {
        virtual ~abstract() = 0;
        virtual bool operator==(const abstract &) const = 0;
    };
    
    inline abstract::~abstract() {}
    
}

#endif

