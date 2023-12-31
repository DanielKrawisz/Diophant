#ifndef DIOPHANT_EXPRESSIONS_SYMBOL
#define DIOPHANT_EXPRESSIONS_SYMBOL

#include <Diophant/symbol.hpp>
#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    struct symbol final: abstract {
        const std::string *name;
        
        static const ptr<const symbol> &make (const std::string &);
        
        std::ostream &write (std::ostream &) const override;
        
        const abstract *root () const override {
            return this;
        }
        
        symbol () : name {nullptr} {}
        
        bool valid () const {
            return name != nullptr;
        }

        bool operator == (const abstract &) const override;
        
    private:
        symbol (const std::string &x) : name {&x} {}
    };
    
    std::ostream inline &symbol::write (std::ostream &o) const {
        return o << name;
    }
    
    std::strong_ordering inline operator <=> (const Symbol &a, const Symbol &b) {
        return *a.name <=> *b.name;
    }
    
    bool inline operator == (const Symbol &a, const Symbol &b) {
        return a.name == b.name;
    }
    
}

namespace Diophant::make {

    Expression inline symbol (const std::string &x) {
        return expressions::symbol::make (x);
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
}

#endif

