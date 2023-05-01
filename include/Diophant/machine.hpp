#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/pattern.hpp>

namespace Diophant {

    // throws an exception if the cast is not valid. For now every cast is valid.
    Expression inline cast (const Type &, Expression &x) {
        return x;
    }

    struct Cast {
        Type type;
        Expression expression;
    };

    struct Machine {
        Expression evaluate (Expression &);
        
        void declare (Pattern, Type);
        void define (Pattern, Expression);
        void define (Pattern, Type, Expression);
        
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

