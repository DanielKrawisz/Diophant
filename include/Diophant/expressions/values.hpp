#ifndef DIOPHANT_EXPRESSIONS_VALUES
#define DIOPHANT_EXPRESSIONS_VALUES

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/value.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    template <typename X>
    struct value final : abstract {
        X val;
        value(const X &x) : val{x} {}
        bool operator==(const abstract &x) const final override;
        
        static Value make(const X &);
    };
    
    using boolean = value<bool>;
    using natural = value<data::N>;
    using list = value<data::stack<Expression>>;
    using string = value<std::string>;
    
    template <typename X>
    bool inline value<X>::operator==(const abstract &x) const {
        auto p = dynamic_cast<const value<X> *>(&x);
        return p == nullptr ? false : val == p->val;
    }
    
    template <typename X>
    Value inline value<X>::make(const X &x) {
        return Diophant::value{std::make_shared<value<X>>(x)};
    }
}

namespace Diophant {
    
    Expression inline expression::boolean(bool b) {
        return expressions::boolean::make(b);
    }
    
    Expression inline expression::natural(const data::N &n) {
        return expressions::natural::make(n);
    }
    
    Expression inline expression::string(const std::string &x) {
        return expressions::string::make(x);
    }
    
    Expression inline expression::list(data::stack<Expression> z) {
        return expressions::list::make(z);
    };
}

#endif
