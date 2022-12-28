#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/replace.hpp>

namespace Diophant {
    struct symbol : std::string {
        static constexpr auto pattern = ctll::fixed_string{"[a-zA-Z_][a-zA-Z0-9_]"};
        bool valid() const;
        operator Expression() const;
    };
    
}

#endif

