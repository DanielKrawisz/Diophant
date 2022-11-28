
#include <lambda.hpp>
#include <Diophant/expression.hpp>

int main(int args, char **arg) {
    using namespace Diophant;
    
    lambda x;
    
    try {
        std::cin >> x;
        std::cout << evaluate(x);
    } catch (...) {
        return 1;
    }
    
    return 0;
}
