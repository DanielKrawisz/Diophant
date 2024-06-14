#ifndef DIOPHANT_SYMBOL
#define DIOPHANT_SYMBOL

#include <Diophant/data.hpp>

namespace Diophant::expressions {
    struct symbol;
}

namespace Diophant {
    using symbol = expressions::symbol;
    using Symbol = const symbol;
    /*
    struct symbols {
        // register a new symbol.
        Symbol operator [] (const std::string &);
        
        // find a symbol if it exists. 
        symbol find (const std::string &);
        
        // all new symbols are remembered in the main map. 
        void update () {
            new_symbols = {};
        }
        
        // delete new symbols. 
        void revert ();
        
    //private:
        std::map<std::string, symbol> registered;
        list<symbol> new_symbols;
    };*/
}

#endif

