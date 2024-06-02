#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/symbol.hpp>
#include <Diophant/expressions/expressions.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        const std::string &name;
        
        static const ptr<const symbol> &make (const std::string &, symbols &);
        
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
        
        symbol (const std::string &x) : name {x} {}
        
        static bool normal (const std::string &name);
    };
    
    std::ostream inline &symbol::write (std::ostream &o) const {
        if (normal (name)) return o << name;
        return o << "`" << name << "`";
    }
    
    std::strong_ordering inline operator <=> (const Symbol &a, const Symbol &b) {
        return a.name <=> b.name;
    }
    
    bool inline operator == (const Symbol &a, const Symbol &b) {
        return a.name == b.name;
    }
    
}

namespace Diophant::expressions {
    bool inline symbol::operator == (const abstract &a) const {
        try {
            return name == dynamic_cast<const symbol &> (a).name;
        } catch (std::bad_cast) {
            return false;
        }
    }

    std::ostream inline &operator << (std::ostream &o, Symbol x) {
        return o << x.name;
    }
}

#endif

