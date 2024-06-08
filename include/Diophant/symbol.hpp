#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/data.hpp>

namespace Diophant::expressions {
    struct symbol;
}

namespace Diophant {
    using symbol = ptr<const expressions::symbol>;
    using Symbol = const symbol;
    using symbols = std::map<std::string, symbol>;

    std::ostream &operator << (std::ostream &, Symbol);
    std::strong_ordering operator <=> (Symbol a, Symbol b);
    bool operator == (Symbol a, Symbol b);
    /*
    struct symbols {
        symbol &operator [] (const std::string &);
        
        maybe<std::map<std::string, symbol>::iterator> find (const std::string &);
        
        // all new symbols are remembered in the main map. 
        void update ();
        
        // delete new symbols. 
        void revert () {
            new_symbols = std::map<std::string, symbol> {};
        }
        
    private:
        std::map<std::string, symbol> registered;
        std::map<std::string, symbol> new_symbols;
    };*/
}

#endif

