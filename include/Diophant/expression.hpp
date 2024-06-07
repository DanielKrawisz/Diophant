#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/expressions/expressions.hpp>

namespace Diophant {
    
    // an expression produces a value.
    struct expression : ptr<const expressions::abstract> {
        
        using ptr<const expressions::abstract>::ptr;
        expression () : ptr<const expressions::abstract> {} {}
        expression (const ptr<const expressions::abstract> &p) : ptr<const expressions::abstract> {p} {}
        expression (ptr<const expressions::abstract> &&p) : ptr<const expressions::abstract> {p} {}
        
        // parse some code and read in an expression.
        expression (const string &);

        Diophant::precedence precedence () const {
            return (*this)->precedence ();
        }

        explicit operator std::string () const;

        // an expression is valid if it is not a pattern.
        bool valid () const;

        std::ostream &write (std::ostream &, Diophant::precedence precedence) const;
        
    };
    
    std::ostream inline &operator << (std::ostream &o, Expression &e);
    
    const expressions::abstract inline *root (Expression &e);

    std::ostream inline &operator << (std::ostream &o, Expression &e) {
        return e.get () == nullptr ? o << "null" : e.write (o, max_precedence);
    }

    inline expression::operator std::string () const {
        std::stringstream ss;
        ss << *this;
        return ss.str ();
    }

    std::ostream inline &expression::write (std::ostream &o, Diophant::precedence precedence) const {
        if (precedence < (*this)->precedence ()) return (*this)->write (o << "(") << ")";
        return (*this)->write (o);
    }

}

namespace Diophant::make {
    Expression inline null () {
        return expression {};
    }
}

#endif
