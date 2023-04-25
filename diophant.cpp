
#include <Diophant/parser.hpp>

int main(int args, char **arg) {

    std::cout << "\nDiophant engaged! " << std::endl;
    using namespace Diophant;

    Diophant::Parser eval {};
    std::string input_str;
    
    while (true) {
        std::cout << "\n input: ";
        
        if (!std::getline (std::cin, input_str)) break;
        if (input_str.empty ()) continue;
        
        try {
            tao::pegtl::memory_input<> input (input_str, "expression");
            tao::pegtl::parse<parse::grammar, Diophant::eval_action> (input, eval);
        } catch (...) {
            return 1;
        }
    }
    
    return 0;
}
