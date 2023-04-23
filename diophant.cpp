
#include <lambda.hpp>
#include <Diophant/expression.hpp>

int main(int args, char **arg) {

    std::cout << "\nDiophant engaged! " << std::endl;
    using namespace Diophant;
    
    machine m;
    std::string input_str;
    
    while (true) {
        std::cout << "\n input: ";
        
        if (!std::getline (std::cin, input_str)) break;
        if (input_str.empty ()) continue;
        
        try {
            tao::pegtl::memory_input<> input (input_str, "expression");
            Diophant::evaluation eval {vars};
            tao::pegtl::parse<calc::grammar, eval_action> (input, eval);
        } catch (...) {
            return 1;
        }
    }
    
    return 0;
}
