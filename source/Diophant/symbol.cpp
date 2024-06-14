#include <Diophant/expressions/symbol.hpp>

namespace Diophant {/*
    Symbol symbols::operator [] (const std::string &x) {
        std::cout << "    retrieving symbol " << x << std::endl;
        auto v = registered.find (x);
        if (v != registered.end ()) {
        std::cout << "    symbol found. " << std::endl;
            return v->second;
        }
        
        registered[x] = nullptr;
            
        v = registered.find (x);
            
        v->second = ptr<const expressions::symbol> (new expressions::symbol {v->first});
        
        new_symbols <<= v->second;
        
        return v->second;
    }
        
    symbol symbols::find (const std::string &x) {
        auto v = registered.find (x);
        if (v != registered.end ()) return v->second;
        return {nullptr};
    }
    
    // delete new symbols. 
    void symbols::revert () {
        for (const auto &x : new_symbols) registered.erase (registered.find (x->name)); 
        new_symbols = {};
    }*/
}
