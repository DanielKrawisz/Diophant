#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/evaluate.hpp>
#include <Diophant/type.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/pattern.hpp>

namespace Diophant {

    struct parameters : cross<pattern> {};
    std::partial_ordering operator <=> (const parameters &, const parameters &);

    maybe<replacements> match (const parameters &, stack<Expression>);

    struct subject {
        Symbol root;
        Diophant::parameters parameters;

        subject (Symbol &x) : root {x}, parameters {} {}
        subject (Symbol &x, const cross<pattern> &p) : root {x}, parameters {p} {}

        static subject read (Expression &);
    };

    struct predicate {
        Diophant::type type;
        maybe<expression> expr;

        predicate (Type);
        predicate (Expression);
        predicate (Type, Expression);
    };

    bool operator == (const predicate &, const predicate &);
    std::ostream &operator << (std::ostream &, const subject &);
    std::ostream &operator << (std::ostream &, const predicate &);

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
        using overloads = stack<entry<parameters, predicate>>;

        map<Symbol, overloads> definitions;
    };
    
    inline predicate::predicate (Type t) : type {t}, expr {} {}
    inline predicate::predicate (Expression e) : type {}, expr {e} {}
    inline predicate::predicate (Type t, Expression e) : type {t}, expr {e} {}

    bool inline operator == (Pattern &, Pattern &) {
        throw method::unimplemented {"pattern == "};
    }

    std::partial_ordering inline operator <=> (const Pattern &, const Pattern &) {
        throw method::unimplemented {"pattern <=> "};
    }

    std::ostream inline &operator << (std::ostream &, Pattern &) {
        throw method::unimplemented {"pattern << "};
    }

    bool inline operator == (const predicate &tr, const predicate & tl) {
        return tl.type == tr.type && tl.expr == tr.expr;
    }

    std::ostream inline &operator << (std::ostream &o, const subject &z) {
        return o << z.root << " " << z.parameters;
    }

}

#endif

