#ifndef DIOPHANT_EXPRESSIONS_BUILDER
#define DIOPHANT_EXPRESSIONS_BUILDER

#include <Diophant/expression.hpp>

namespace Diophant {
    
    struct expression::builder {
        builder &string(const std::string &);
        builder &symbol(const std::string &);
        builder &natural(const data::N &);
        
        builder &call();
        
        builder &open_parenthesis();
        builder &close_parenthesis();
        
        builder &open_bracket();
        builder &close_bracket();
        builder &comma();
        
        builder &lambda();
        
        Expression complete();
        
        builder();
        
    private:
        data::stack<data::stack<Expression>> expressions;
    };
    
    inline expression::builder::builder() {
        expressions = expressions << data::stack<Expression>{};
    }
    
}

#endif

