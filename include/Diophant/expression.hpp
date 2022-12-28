#ifndef DIOPHANT_EXPRESSION
#define DIOPHANT_EXPRESSION

#include <Diophant/symbol.hpp>

namespace Diophant {
    
    struct lambda;
    
    struct type_expression : data::expression<
        ptr<variant<bool, data::N, std::string, symbol, list<expression>, map<expression, expression>, lambda>>, 
        variant<symbol, lambda>> {
        
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
    
}

#endif
