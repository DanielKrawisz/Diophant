#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/evaluate.hpp>
#include <Diophant/expressions/expressions.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/pattern.hpp>

namespace Diophant {
    
    struct expression : ptr<const expressions::abstract> {
        
        using ptr<const expressions::abstract>::ptr;
        expression () : ptr<const expressions::abstract> {} {}
        expression (ptr<const expressions::abstract> p) : ptr<const expressions::abstract> {p} {}
        
        // parse some code and read in an expression.
        expression (const string &);

        uint32 precedence () const {
            return (*this)->precedence ();
        }
        
        const expressions::abstract *root () const {
            return (*this)->root ();
        }
        
        operator Pattern () const;
        
        maybe<replacements> match (Expression &, const Machine &) const;

        explicit operator std::string () const;

        // an expression is valid if it is not a pattern.
        bool valid () const {
            throw method::unimplemented {"expression::valid"};
        }
        
    };
    
    std::ostream inline &operator << (std::ostream &o, Expression &e);
    
    const expressions::abstract inline *root (Expression &e);
    
    struct Type;

}

namespace Diophant::make {

    Expression null ();
    Expression boolean (bool b);
    Expression rational (const data::Q &q);
    Expression symbol (const std::string &x);
    Expression string (const data::string &str);
    Expression list (const data::list<Expression> &ls);
    Expression object (const data::list<data::entry<data::string, Expression>> &x);

    Expression apply (Expression &, Expression &);

    Expression negate (Expression &);
    Expression plus (Expression &, Expression &);
    Expression minus (Expression &, Expression &);
    Expression times (Expression &, Expression &);
    Expression power (Expression &, Expression &);
    Expression divide (Expression &, Expression &);

    Expression equal (Expression &, Expression &);
    Expression unequal (Expression &, Expression &);
    Expression greater_equal (Expression &, Expression &);
    Expression less_equal (Expression &, Expression &);
    Expression greater (Expression &, Expression &);
    Expression less (Expression &, Expression &);

    Expression boolean_not (Expression &);
    Expression boolean_and (Expression &, Expression &);
    Expression boolean_or (Expression &, Expression &);

    Expression arrow (Expression &, Expression &);

    Expression intuitionistic_and (Expression &, Expression &);
    Expression intuitionistic_or (Expression &, Expression &);
    Expression intuitionistic_implies (Expression &, Expression &);
    
    Expression pattern (Symbol &, const Type &);

}

namespace Diophant {

    std::ostream inline &operator << (std::ostream &o, Expression &e) {
        return e.get () == nullptr ? o << "null" : e->write (o);
    }

    const expressions::abstract inline *root (Expression &e) {
        return e->root ();
    }

    inline expression::operator std::string () const {
        std::stringstream ss;
        ss << *this;
        return ss.str ();
    }

    bool inline operator == (Expression &a, Expression &b) {
        return a.get () == nullptr ? b.get () == nullptr : b.get () == nullptr ? false : *a == *b;
    }

}

namespace Diophant::make {
    Expression inline null () {
        return expression {};
    }
}

#endif
