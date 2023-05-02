#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/match.hpp>
#include <Diophant/type.hpp>

namespace Diophant {

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

