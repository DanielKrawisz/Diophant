#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/evaluate.hpp>

namespace Diophant {
    struct symbol : std::string {
        using std::string::string;
        operator Expression() const;
    };
    
}

#endif

