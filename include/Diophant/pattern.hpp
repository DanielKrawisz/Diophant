#ifndef DIOPHANT_PATTERN
#define DIOPHANT_PATTERN

#include <Diophant/symbol.hpp>

namespace Diophant {

    struct Type {
        
        Type () {}
        
        explicit Type (Expression) {}
        
    };

    std::partial_ordering operator <=> (const Type &, const Type &);
    bool operator == (const Type &, const Type &);
    
    std::ostream &operator << (std::ostream &, Type &);

    Type type (Expression &);
    
    struct pattern;
    using Pattern = const pattern;

    bool operator == (Pattern &, Pattern &);
    std::partial_ordering operator <=> (const Pattern &, const Pattern &);
    
    std::ostream &operator << (std::ostream &, Pattern &);
    
    Symbol root (Pattern &); 
    
    using replacement = entry<Symbol, Expression>;
    using replacements = list<replacement>;

    maybe<replacements> match (Expression &e, Pattern &p);
    
    struct pattern {
        Symbol symbol;
        list<Expression> parameters;
        
        pattern (Symbol &);
        pattern (Expression &);

        // attempt to match against the expression. Return a list of replacements if match is successful.
        maybe<replacements> match (Expression &) const;
    };

    maybe<replacements> inline match (Expression &e, Pattern &p) {
        return p.match (e);
    }

    std::partial_ordering inline operator <=> (const Type &, const Type &) {
        return std::partial_ordering::equivalent;
    }
    
    bool inline operator == (const Type &, const Type &) {
        return true;
    }
    
    std::ostream inline &operator << (std::ostream &o, Type &) {
        return o;
    }
    
}

#endif

