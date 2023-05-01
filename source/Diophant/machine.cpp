
#include <Diophant/replace.hpp>
#include <Diophant/machine.hpp>

namespace Diophant {

    Machine::Machine () {
/*
        define (Pattern {"null"}, Type {"null"}, Expression::null ());
        define (Pattern {"true"}, Type {"boolean"}, Expression::boolean (true));
        define (Pattern {"false"}, Type {"boolean"}, Expression::boolean (false));

        define (Pattern {"-x.Q"}, Type {"Q"});
        define (Pattern {"x.Q + y.Q"}, Type {"Q"});
        define (Pattern {"x.Q * y.Q"}, Type {"Q"});
        define (Pattern {"x.Q - y.Q"}, Type {"Q"});
        define (Pattern {"x.Q / y.Q"}, Type {"Q"});

        define (Pattern {"x.bool || y.bool"}, Type {"boolean"});
        define (Pattern {"x.bool && y.bool"}, Type {"boolean"});
        define (Pattern {"!x.bool"}, Type {"boolean"});

        define (Pattern {"x.Q == y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q != y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q <= y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q >= y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q < y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q > y.Q"}, Type {"boolean"});
*/
    }
        
    map<Pattern, Machine::Transformation> Machine::operator [] (Symbol &x) const {
        auto v = definitions.contains (x);
        if (v) return *v;
        return {};
    }

    Expression Machine::evaluate (Expression &x) {
        Symbol *h = dynamic_cast<Symbol *> (root (x));
        
        if (h == nullptr) return x;

        auto ps = (*this)[*h];

        // note: we do nothing with types yet.
        for (auto p : ps) if (auto matches = match (x, p.Key); bool (matches))
            return evaluate (cast (p.Value.type, replace (p.Value.expr, *matches)));

        return x;
    }
    
    void Machine::declare (Pattern p, Type t) {
        Symbol &h = dynamic_cast<Symbol &> (root (p));
        
        auto v = definitions.contains (h);
        
        if (!v) {
            definitions = definitions.insert (h, map<Pattern, Transformation> {}.insert (p, Transformation {t}));
            return; 
        }
        
        auto w = v->contains (p);
        
        if (!w) {
            *v = v->insert (p, Transformation {t});
            return;
        }
        
        if (w->type == t) return;
        
        throw exception {} << "conflicting declaration " << p << " : " << t;
        
    }
    
    void Machine::define (Pattern p, Expression e) {
        Symbol &h = dynamic_cast<Symbol &> (root (p));
        
        auto v = definitions.contains (h);
        
        if (!v) {
            definitions = definitions.insert (h, map<Pattern, Transformation> {}.insert (p, Transformation {e}));
            return; 
        }
        
        auto w = v->contains (p);
        
        if (!w) {
            *v = v->insert (p, Transformation {e});
            return;
        }
        
        if (w->expr.get () == nullptr) {
            w->expr = e;
            return;
        }
        
        throw exception {} << "conflicting declaration " << p << " = " << e;
    }
    
    void Machine::define (Pattern p, Type t, Expression e) {
        Symbol &h = dynamic_cast<Symbol &> (root (p));
        
        auto v = definitions.contains (h);
        
        if (!v) {
            definitions = definitions.insert (h, map<Pattern, Transformation> {}.insert (p, Transformation {t, e}));
            return; 
        }
        
        auto w = v->contains (p);
        
        if (!w) {
            *v = v->insert (p, Transformation {t, e});
            return;
        }
        
        if (w->type == t && w->expr.get () == nullptr) {
            w->expr = e;
            return;
        }
        
        throw exception {} << "conflicting declaration " << p << " : " << t << " = " << e;
    }

}
