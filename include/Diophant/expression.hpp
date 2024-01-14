#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/pattern.hpp>
#include <Diophant/symbol.hpp>

namespace Diophant {
    
    // an expression produces a value.
    struct expression : ptr<const expressions::abstract> {
        
        using ptr<const expressions::abstract>::ptr;
        expression () : ptr<const expressions::abstract> {} {}
        expression (ptr<const expressions::abstract> p) : ptr<const expressions::abstract> {p} {}
        
        // parse some code and read in an expression.
        expression (const string &);

        Diophant::precedence precedence () const {
            return (*this)->precedence ();
        }
        
        //maybe<list<entry<Symbol, Expression>>> match (Expression &, const Machine &) const;

        explicit operator std::string () const;

        // an expression is valid if it is not a pattern.
        bool valid () const {
            throw method::unimplemented {"expression::valid"};
        }

        std::ostream &write (std::ostream &, Diophant::precedence precedence) const;
        
    };
    
    std::ostream inline &operator << (std::ostream &o, Expression &e);
    
    const expressions::abstract inline *root (Expression &e);

}

namespace Diophant::make {

    // basic values
    Expression null ();
    Expression boolean (bool b);
    Expression natural (const data::Q &q);
    Expression integer (const data::Q &q);
    Expression rational (const data::Q &q);
    Expression symbol (const std::string &x, symbols &);
    Expression string (const data::string &str);
    Expression list (data::stack<Expression> ls);
    Expression map (data::stack<data::entry<Expression, Expression>> x);
    Expression lambda (Symbol &, Expression &);

    Expression call (Expression &, Expression &);

    Expression negate (Expression &);
    Expression plus (Expression &);
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

    Expression intuitionistic_and (Expression &, Expression &);
    Expression intuitionistic_or (Expression &, Expression &);
    Expression intuitionistic_implies (Expression &, Expression &);
    
    Expression pattern (Symbol &, const Type &);

}

namespace Diophant {

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
