#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/evaluate.hpp>
#include <Diophant/expressions/expressions.hpp>

namespace Diophant {
    
    struct expression : ptr<const expressions::abstract> {
        
        using ptr<const expressions::abstract>::ptr;
        expression (ptr<const expressions::abstract> &&p) : ptr<const expressions::abstract> {p} {}
        
        // parse some code and read in an expression.
        expression (const string &);

        uint32 precedence () const {
            return (*this)->precedence ();
        }
        
    };

}

namespace Diophant::make {

    Expression null ();
    Expression boolean (bool b);
    Expression rational (const data::Q &q);
    Expression symbol (const data::string &x);
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
    
    Expression inline null () {
        return expression {};
    }

}

#endif
