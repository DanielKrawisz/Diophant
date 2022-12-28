#ifndef DIOPHANT_LAMBDA
#define DIOPHANT_LAMBDA

#include <Diophant/symbol.hpp>

namespace Diophant {
    struct lambda : data::entry<symbol, expression> {};
    using Lambda = const lambda;
    
    struct lambda {
        static Lambda read(std::string &);
        bool valid() const;
        Lambda operator()(Lambda) const;
        
        operator Expression() const;
        explicit lambda(Expression); 
        
        ptr<const expressions::lambda> _;
    };
    
    std::ostream inline &operator<<(std::ostream &o, Lambda l) {
        return o << Expression(l);
    }
    
    std::istream &operator>>(std::istream &i, lambda &l);
    
}

#endif
