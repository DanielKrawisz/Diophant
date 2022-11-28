#include <Diophant/expression.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expressions/call.hpp>
#include <Diophant/expressions/lambda.hpp>

namespace Diophant {
    
    Expression expression::read(std::string &str) {
        expression ex;
        std::stringstream x{str};
        x >> ex;
        return ex;
    }
    
    bool operator==(Expression a, Expression b) {
        if (a._.get() == b._.get()) return true;
        if (a._.get() == nullptr || b._.get() == nullptr) return false;
        
        return *a._ == *b._;
    };
    
    Expression expression::symbol(const std::string &n) {
        return expression{std::make_shared<expressions::symbol>(n)};
    }
    
    Expression expression::operator()(Expression arg) const {
        return expression{std::make_shared<expressions::call>(*this, arg)};
    }
    
    Expression expression::lambda(Symbol x, Expression body) {
        return expression{std::make_shared<expressions::lambda>(x, body)};
    }
}

