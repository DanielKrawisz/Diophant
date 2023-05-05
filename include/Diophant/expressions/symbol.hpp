#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/symbol.hpp>
#include <Diophant/expressions/expressions.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        const std::string *Name;
        
        static const ptr<const symbol> &make (const std::string &);
        
        std::ostream &write (std::ostream &) const override;
        
        const abstract *root () const override {
            return this;
        }
        
        symbol () : Name {nullptr} {}
        
        bool valid () const {
            return Name != nullptr;
        }
        
    private:
        symbol (const std::string &x) : Name {&x} {}
    };
    
    std::ostream inline &symbol::write (std::ostream &o) const {
        return o << Name;
    }
    
    std::strong_ordering inline operator <=> (const Symbol &a, const Symbol &b) {
        return a.Name <=> b.Name;
    }
    
    bool inline operator == (const Symbol &a, const Symbol &b) {
        return a.Name == b.Name;
    }
    
}

#endif

