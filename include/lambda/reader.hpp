#ifndef LAMBDA_READER
#define LAMBDA_READER

#include <Diophant/expression.hpp>

namespace Diophant {
    
    
    struct lambda_reader {
        
        lambda_reader &string(const std::string &);
        lambda_reader &natural(const data::N &);
        lambda_reader &symbol(const std::string &);
        lambda_reader &open_parenthesis();
        lambda_reader &close_parenthesis();
        lambda_reader &open_bracket();
        lambda_reader &close_bracket();
        lambda_reader &comma();
        lambda_reader &lambda();
        
        void complete(Expression) = 0;
        
    };
}

#endif
