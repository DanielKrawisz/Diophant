#include <Diophant/expressions/builder.hpp>

namespace Diophant {
    
    Expression expression::builder::complete() {
        if (data::size(expressions) != 1 || data::size(data::first(expressions)) != 1) 
            throw data::exception{"could not read expression"};
        auto x = data::first(data::first(expressions));
        expressions = data::stack<data::stack<Expression>>{} << data::stack<Expression>{};
        return x;
    }
    
    expression::builder &expression::builder::symbol(const std::string &x) {
        expressions = data::rest(expressions) << (data::first(expressions) << expression::symbol(x));
        return *this;
    }
    
    expression::builder &expression::builder::call() {
        Expression arg = data::first(data::first(expressions));
        Expression func = data::first(data::rest(data::first(expressions)));
        expressions = data::rest(expressions) << 
            (data::rest(data::rest(data::first(expressions))) << Diophant::call(func, arg));
        return *this;
    }
    
    expression::builder &expression::builder::lambda() {
        Expression body = data::first(data::first(expressions));
        Expression arg = data::first(data::rest(data::first(expressions)));
        expressions = data::rest(expressions) << 
            (data::rest(data::rest(data::first(expressions))) << expression::lambda(Symbol(arg), body));
        return *this;
    }
    
    expression::builder &expression::builder::open_parenthesis() {
        expressions = expressions << data::stack<Expression>{};
        return *this;
    }
    
    expression::builder &expression::builder::close_parenthesis() {
        expressions = data::rest(data::rest(expressions)) << 
            (data::first(data::rest(expressions)) << data::first(data::first(expressions)));
        return *this;
    }
    
    expression::builder &expression::builder::open_bracket() {
        expressions = expressions << data::stack<Expression>{} << data::stack<Expression>{};
        return *this;
    }
    
    expression::builder &expression::builder::comma() {
        expressions = data::rest(data::rest(expressions)) << 
            (data::first(data::rest(expressions)) << data::first(data::first(expressions))) << 
            data::stack<Expression>{};
        return *this;
    }
    
    expression::builder &expression::builder::close_bracket() {
        
        if (!data::empty(data::first(expressions))) 
            expressions = data::rest(data::rest(expressions)) << 
                (data::first(data::rest(expressions)) << data::first(data::first(expressions)));
        else expressions = data::rest(expressions);
        
        expressions = data::rest(data::rest(expressions)) << 
            (data::first(data::rest(expressions)) << expression::list(data::first(expressions)));
        
        return *this;
        
    }
    
}

