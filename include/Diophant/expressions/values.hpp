#ifndef DIOPHANT_EXPRESSIONS_VALUES
#define DIOPHANT_EXPRESSIONS_VALUES

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    template <typename X>
    struct value final : abstract {
        X val;
        value (const X &x) : val {x} {}
        
        static Expression make (const X &);
    };
    
    using boolean = value<bool>;
    using rational = value<data::Q>;
    using list = value<data::stack<Expression>>;
    using object = value<data::stack<entry<std::string, Expression>>>;
    using string = value<std::string>;
}

namespace Diophant::make {
    
    Expression inline boolean (bool b) {
        return expressions::boolean::make (b);
    }
    
    Expression inline rational (const data::Q &n) {
        return expressions::rational::make (n);
    }
    
    Expression inline string (const std::string &x) {
        return expressions::string::make (x);
    }
    
    Expression inline list (data::stack<Expression> z) {
        return expressions::list::make (z);
    };
    
    Expression inline object (data::stack<entry<std::string, Expression>> z) {
        return expressions::object::make (z);
    };
    
}

namespace Diophant::expressions {
    template <typename X>
    Expression inline value<X>::make (const X &x) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<value<X>> (x))};
    }
}


#endif
