#include <Diophant/expressions/symbol.hpp>
#include <Diophant/pattern.hpp>

namespace Diophant::expressions {
    
    const ptr<const symbol> &symbol::make (const std::string &x, symbols &Symbols) {
        
        auto v = Symbols.find (x);
        
        if (v == Symbols.end ()) {
            Symbols[x] = nullptr;
            
            v = Symbols.find (x);
            
            v->second = ptr<const symbol> (new symbol {v->first});
        }
        
        return v->second;
    }
        
    bool symbol::normal (const std::string &name) {
        if (name.size () == 0) return false;
        if (name[0] < 'A' || name[0] > 'z' || (name[0] > 'Z' && name[0] < 'a')) return false;
        for (int i = 1; i < name.size (); i++) if (name[i] < '0' || 
            (name[i] > '9' && name[i] < 'A') || (name[i] > 'Z' && name[i] < '_') || 
            (name[i] > '_' && name[i] < 'a') || name[i] > 'z') return false;
        return true;
    }
    
}

