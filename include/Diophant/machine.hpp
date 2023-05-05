#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/evaluate.hpp>
#include <Diophant/type.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/symbol.hpp>

namespace Diophant {

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

    struct Machine {
        Expression evaluate (Expression &);
        
        void declare (Pattern, Type);
        void define (Pattern, Expression);
        void define (Pattern, Type, Expression);
        
        maybe<replacements> match (Pattern &, Expression &) const;
        
        // every cast is valid for now. 
        bool cast (const Type &, const Expression &) const {
            return true;
        }
        
        Machine ();
        
    private:
        struct Transformation {
            Type type;
            expression expr;
            
            Transformation (Type);
            Transformation (Expression);
            Transformation (Type, Expression);
            
            bool operator == (const Transformation &) const;
        };
        
        map<Symbol, map<Pattern, Transformation>> definitions;
        
        map<Pattern, Transformation> operator [] (Symbol &x) const;
    };

    Expression inline evaluate (Expression &x, Machine &m) {
        return m.evaluate (x);
    }
    
    inline Machine::Transformation::Transformation (Type t) : type {t}, expr {} {}
    inline Machine::Transformation::Transformation (Expression e) : type {}, expr {e} {}
    inline Machine::Transformation::Transformation (Type t, Expression e) : type {t}, expr {e} {}
            
    bool inline Machine::Transformation::operator == (const Transformation &t) const {
        return type == t.type && expr == t.expr;
    }

}

#endif

