#ifndef DIOPHANT_MACHINE
#define DIOPHANT_MACHINE

#include <Diophant/evaluate.hpp>
#include <Diophant/type.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/pattern.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/statement.hpp>

namespace Diophant {

    std::ostream &operator << (std::ostream &, const subject &);

    struct Machine {
        Expression evaluate (Expression &, data::set<symbol> fixed);

        void declare (const subject &);
        void define (const subject &, Expression &);

        maybe<replacements> match (parameters &, Expression &) const;
        
        // every cast is valid for now. 
        bool cast (const Type &, const Expression &) const {
            return true;
        }
        
        Machine ();
        
        struct definition;

        using overloads = stack<definition>;
        
        struct definition {
            Diophant::parameters key;
            maybe<expression> value;
            bool locked;
            overloads more_specific;
            
            friend std::ostream &operator << (std::ostream &, const definition &);
            definition (const Diophant::parameters &k, const maybe<expression> &v = {}): 
                key {k}, value {v}, locked {false}, more_specific {} {}
        };

        std::map<symbol, overloads> definitions;
    };
    
    void initialize (Machine&);

    std::ostream &operator << (std::ostream &, const Machine &);

    std::ostream inline &operator << (std::ostream &o, const parameters &p) {
        for (const auto &e : p.params) e.write (o << " ", call_precedence);
        if (p.such_that.get () != nullptr) o << " ? " << p.such_that;
        return o;
    }

}

#endif

