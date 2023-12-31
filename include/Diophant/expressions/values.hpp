#ifndef DIOPHANT_EXPRESSIONS_VALUES
#define DIOPHANT_EXPRESSIONS_VALUES

#include <Diophant/replace.hpp>
#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    template <typename X>
    struct value final : abstract {
        X val;
        value (const X &x) : val {x} {}
        
        static Expression make (const X &);
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
        
    };
    
    using boolean = value<bool>;
    using rational = value<data::Q>;
    using list = value<data::stack<Expression>>;
    using object = value<data::stack<entry<data::string, Expression>>>;
    using string = value<data::string>;
}

namespace Diophant::make {
    
    Expression inline boolean (bool b) {
        return expressions::boolean::make (b);
    }
    
    Expression inline rational (const data::Q &n) {
        return expressions::rational::make (n);
    }
    
    Expression inline string (const data::string &x) {
        return expressions::string::make (x);
    }
    
    Expression inline list (data::stack<Expression> z) {
        return expressions::list::make (z);
    };
    
    Expression inline object (data::stack<entry<data::string, Expression>> z) {
        return expressions::object::make (z);
    };
    
}

namespace Diophant::expressions {
    template <typename X>
    Expression inline value<X>::make (const X &x) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<value<X>> (x))};
    }
    
    std::ostream inline &write (std::ostream &o, bool b) {
        return o << std::boolalpha << b;
    }
    
    std::ostream inline &write (std::ostream &o, const data::Q &q) {
        return o << q;
    }
    
    std::ostream inline &write (std::ostream &o, const data::string &x) {
        return o << x;
    }
    
    std::ostream &write (std::ostream &o, data::stack<Expression>);
    std::ostream &write (std::ostream &o, data::stack<entry<data::string, Expression>>);

    template <typename X>
    std::ostream inline &value<X>::write (std::ostream &o) const {
        return expressions::write (o, val);
    }

    template <typename X>
    bool inline value<X>::operator == (const abstract &a) const {
        try {
            return val == dynamic_cast<const value<X> &> (a).val;
        } catch (std::bad_cast) {
            return false;
        }
    }
}

#endif
