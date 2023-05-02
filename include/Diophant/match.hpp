#ifndef DIOPHANT_MATCH
#define DIOPHANT_MATCH

#include <Diophant/root.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/expression.hpp>

namespace Diophant {

    // attempt to match against the expression. Return a list of replacements if match is successful.
    maybe<replacements> match (Pattern &p, Expression &e);
    
    struct pattern {
        Symbol root;
        stack<Expression> parameters;
        
        pattern () : root {}, parameters {} {}
        pattern (Symbol &x) : root {x}, parameters {} {}
        pattern (Symbol &x, stack<Expression> p) : root {x}, parameters {p} {}
        
        bool valid () const {
            return root.valid () && parameters.valid ();
        }
    };
    
    Symbol inline &root (Pattern &p) {
        return p.root;
    }
    
    inline expression::operator Pattern () const {
        return (Pattern) (*this);
    }
    
}

namespace Diophant::expressions {
        
    inline abstract::operator Pattern () const {
        return {};
    }
    
    inline symbol::operator Pattern () const {
        return {*this};
    }
    
}

#endif

