#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/evaluate.hpp>
#include <Diophant/expressions/expressions.hpp>

namespace Diophant {
    
    struct expression : ptr<const expressions::abstract> {
        
        // an undefined expression
        expression () : ptr<const expressions::abstract> {} {}
        
        // parse some code and read in an expression.
        expression (const string &);

        uint32 precedence () const;

        Expression operator () (Expression &) const;
        Expression operator - () const;
        Expression operator ! () const;

        Expression operator + (Expression &) const;
        Expression operator - (Expression &) const;
        Expression operator * (Expression &) const;
        Expression operator / (Expression &) const;
        Expression operator ^ (Expression &) const;

        Expression operator == (Expression &) const;
        Expression operator != (Expression &) const;
        Expression operator <= (Expression &) const;
        Expression operator >= (Expression &) const;
        Expression operator < (Expression &) const;
        Expression operator > (Expression &) const;

        Expression operator && (Expression &) const;
        Expression operator || (Expression &) const;
        Expression arrow (Expression &) const;

        Expression operator & (Expression &) const;
        Expression operator | (Expression &) const;
        Expression implies (Expression &) const;
        
    };

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

    
}

#endif
