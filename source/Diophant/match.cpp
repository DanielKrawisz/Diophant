
#include <Diophant/match.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/apply.hpp>

namespace Diophant {

    maybe<replacements> match (Pattern &p, Expression &e) {
        if (data::size (p.parameters) == 0) {
            auto x = static_cast<Symbol *> (e.get ());
            if (x == nullptr) return {};
            return *x == p.root ? maybe<replacements> {replacements {}} : maybe<replacements> {};
        } 
        
        auto fx = static_cast<const expressions::apply *> (e.get ()); 
        if (fx == nullptr) return {};
        
        auto ra = data::first (p.parameters).match (fx->argument);
        if (!bool (ra)) return {};
        
        auto rf = match (pattern {p.root, data::rest (p.parameters)}, fx->function);
        if (!bool (rf)) return {};
        
        replacements r = *rf;
        
        for (const auto &e : *ra) {
            if (auto v = r.contains (e.Key); bool (v)) if (*v != e.Value) return {};
            
            r = r.insert (e.Key, e.Value);
        }
        
        return r;
        
    }
    
}
