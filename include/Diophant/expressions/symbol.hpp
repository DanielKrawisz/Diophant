#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/symbol.hpp>
#include <Diophant/expressions/expressions.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        const std::string name;
        
        static Symbol make (const std::string &x, symbols &z) {
            return z[x];
        }
        
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
        
        symbol (const std::string &x) : name {x} {}
        
        static bool normal (const std::string &name);
    };
    
    std::ostream inline &symbol::write (std::ostream &o) const {
        if (normal (name)) return o << name;
        return o << "`" << name << "`";
    }
    
    std::strong_ordering inline operator <=> (const symbol &a, const symbol &b) {
        return a.name <=> b.name;
    }
    
    bool inline operator == (const symbol &a, const symbol &b) {
        return a.name == b.name;
    }
    
    bool inline symbol::operator == (const abstract &a) const {
        try {
            return name == dynamic_cast<const symbol &> (a).name;
        } catch (std::bad_cast) {
            return false;
        }
    }

    std::ostream inline &operator << (std::ostream &o, symbol x) {
        return o << x.name;
    }
}

namespace Diophant {
    std::ostream inline &operator << (std::ostream &o, Symbol x) {
        return o << *x;
    }
    
    std::strong_ordering inline operator <=> (Symbol a, Symbol b) {
        return *a <=> *b;
    }
    
    bool inline operator == (Symbol a, Symbol b) {
        return a.get () == b.get () ? true : a.get () == nullptr || b.get () == nullptr ? false : *a == *b;
    }
}

#endif

