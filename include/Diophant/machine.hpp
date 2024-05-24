#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/evaluate.hpp>
#include <Diophant/type.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/apply.hpp>

namespace Diophant {

    struct parameters : cross<pattern> {};
    std::ostream &operator << (std::ostream &, const parameters &);
    std::partial_ordering operator <=> (const parameters &, const parameters &);

    maybe<replacements> match (const parameters &, stack<Expression>);

    struct subject {
        Symbol root;
        Diophant::parameters parameters;

        subject (Symbol &x) : root {x}, parameters {} {}
        subject (Symbol &x, const cross<pattern> &p) : root {x}, parameters {p} {}
        subject (Symbol &x, stack<Expression>);

        static subject read (Expression &);
    };

    struct predicate {
        Diophant::type type;
        ptr<Diophant::function> function;

        predicate (Type);
        predicate (ptr<Diophant::function>);
        predicate (Type, ptr<Diophant::function>);

        // dummy function so that we can use maps.
        // at the moment, we don't need this. 
        bool operator == (const predicate &) const {
            return true;
        }
    };

    std::ostream &operator << (std::ostream &, const subject &);
    std::ostream &operator << (std::ostream &, const predicate &);

    struct transformation {
        Diophant::parameters Key;
        Diophant::predicate Value;
        
        bool operator == (const transformation &t) const {
            return Key == t.Key && Value == t.Value;
        }
    };

    //std::partial_ordering operator <=> (const transformation &a, const transformation &b);

    struct Machine {
        Expression evaluate (Expression &, data::set<expressions::symbol> fixed);

        void declare (const subject &, Type);
        void define (const subject &, const predicate &);

        maybe<replacements> match (parameters &, Expression &) const;
        
        // every cast is valid for now. 
        bool cast (const Type &, const Expression &) const {
            return true;
        }
        
        Machine ();
        using overloads = stack<transformation>;

        map<Symbol, overloads> definitions;
    };

    std::ostream &operator << (std::ostream &, const Machine &);
    
    inline predicate::predicate (Type t) : type {t}, function {} {}
    inline predicate::predicate (ptr<Diophant::function> e) : type {}, function {e} {}
    inline predicate::predicate (Type t, ptr<Diophant::function> e) : type {t}, function {e} {}
/*
    std::partial_ordering inline operator <=> (const transformation &a, const transformation &b) {
        return a.Key <=> b.Key;
    }*/

    std::ostream inline &operator << (std::ostream &o, const parameters &p) {
        for (const auto &e : p) e.write (o << " ", call_precedence);
        return o;
    }

    std::ostream inline &operator << (std::ostream &o, const subject &z) {
        return o << z.root << z.parameters;
    }

}

#endif

