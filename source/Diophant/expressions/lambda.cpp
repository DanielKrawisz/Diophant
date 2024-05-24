
#include <Diophant/expressions/lambda.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant {
    bool inline pattern_equal (Expression &a, Expression &b, list<std::pair<Symbol, Symbol>>) {
        throw method::unimplemented {"pattern_equal"};
    }
}
