#include <Diophant/expressions/symbol.hpp>
#include <Diophant/pattern.hpp>

namespace Diophant::expressions {
    
    const ptr<const symbol> &symbol::make (const std::string &x) {
        static std::map<std::string, ptr<const symbol>> Symbols;
        
        auto v = Symbols.find (x);
        
        if (v == Symbols.end ()) {
        
            Symbols[x] = nullptr;
            
            v = Symbols.find (x);
            
            v->second = ptr<const symbol> (new symbol {v->first});
        }
        
        return v->second;
    }
    
}

