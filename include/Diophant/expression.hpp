#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/symbol.hpp>

namespace Diophant {
    
    struct Expression : ptr<const expressions::abstract> {
        
        // an undefined expression
        expression () : ptr<expressions::abstract> {} {}
        
        // parse some code and read in an expression.
        expression (const string &);
        
        static Expression null ();
        static Expression boolean (bool b);
        static Expression rational (const data::Q &q);
        static Expression symbol (const data::string &x);
        static Expression string (const data::string &str);
        static Expression list (const data::list<Expression> &ls);
        static Expression object (const data::list<data::entry<data::string, Expression>> &x);

        static Expression apply (const Expression, const Expression);

        static Expression negate (const Expression);
        static Expression plus (const Expression, const Expression);
        static Expression minus (const Expression, const Expression);
        static Expression times (const Expression, const Expression);
        static Expression power (const Expression, const Expression);
        static Expression divide (const Expression, const Expression);

        static Expression equal (const Expression, const Expression);
        static Expression unequal (const Expression, const Expression);
        static Expression greater_equal (const Expression, const Expression);
        static Expression less_equal (const Expression, const Expression);
        static Expression greater (const Expression, const Expression);
        static Expression less (const Expression, const Expression);

        static Expression boolean_not (const Expression);
        static Expression boolean_and (const Expression, const Expression);
        static Expression boolean_or (const Expression, const Expression);

        static Expression arrow (const Expression, const Expression);

        static Expression intuitionistic_and (const Expression, const Expression);
        static Expression intuitionistic_or (const Expression, const Expression);
        static Expression intuitionistic_implies (const Expression, const Expression);

        virtual ~expression () {};

        virtual std::ostream &write (std::ostream &) const = 0;

        data::string write () const {
            std::stringstream ss;
            write (ss);
            return ss.str ();
        }

        virtual uint32 precedence () const {
            return 0;
        }

        virtual Expression evaluate (const std::map<data::string, Expression> &vars) const {
            return this->shared_from_this ();
        };

        virtual Expression operator () (const Expression) const;
        virtual Expression operator - () const;
        virtual Expression operator ! () const;

        virtual Expression operator + (const Expression) const;
        virtual Expression operator - (const Expression) const;
        virtual Expression operator * (const Expression) const;
        virtual Expression operator / (const Expression) const;
        virtual Expression operator ^ (const Expression) const;

        virtual Expression operator == (const Expression) const;
        virtual Expression operator != (const Expression) const;
        virtual Expression operator <= (const Expression) const;
        virtual Expression operator >= (const Expression) const;
        virtual Expression operator < (const Expression) const;
        virtual Expression operator > (const Expression) const;

        virtual Expression operator && (const Expression) const;
        virtual Expression operator || (const Expression) const;
        virtual Expression arrow (const Expression) const;

        virtual Expression operator & (const Expression) const;
        virtual Expression operator | (const Expression) const;
        virtual Expression implies (const Expression) const;
        
    };
    
}

#endif
