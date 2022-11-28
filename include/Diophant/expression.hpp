#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/symbol.hpp>

namespace Diophant::expressions {
    struct abstract;
}

namespace Diophant {
    
    struct expression {
        
        static Expression read(std::string &);
        
        Expression operator()(Expression) const;
        
        static Expression null();
        static Expression boolean(bool);
        static Expression natural(const data::N &);
        static Expression string(const std::string &);
        static Expression symbol(const std::string &);
        static Expression list(data::stack<Expression>);
        static Expression lambda(Symbol, Expression);
        
        struct builder;
        
        explicit operator Symbol() const;
        
        ptr<const expressions::abstract> _;
    };
    
    Expression inline call(Expression a, Expression b) {
        return a(b);
    }
    
    Expression inline expression::null() {
        return expression{};
    }
    
}

#endif
