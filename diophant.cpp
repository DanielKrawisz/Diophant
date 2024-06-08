
#include <Diophant/parser.hpp>

using namespace Diophant;

maybe<std::string> read () {
    std::string input;
    std::cout << "\n input: ";
    if (!std::getline (std::cin, input)) return {};
    return {input};
}

int main (int args, char **arg) {

    std::cout << "\nDiophant engaged! " << std::endl;

    Parser eval {};
    
    initialize (eval.machine);
    
    try {
        while (true) {
            maybe<string> input = read ();
            if (!bool (input)) break;
            if (input->empty ()) continue;

            // for each input line, attempt to construct an expression
            // out of it and then evaluate it.
            try {
                eval.read_line (*input);
                if (eval.runnable ()) std::cout << "\n result: " << eval.run () << std::endl;
            } catch (data::exception &e) {
                std::cout << "Exception caught: " << e.what () << "!" << std::endl;
            }
        }
    } catch (std::exception &e) {
        std::cout << "Unknown exception caught: " << e.what () << "!" << std::endl;
        return 1;
    } catch (...) {
        std::cout << "Unknown type caught!" << std::endl;
        return 1;
    }
    
    return 0;
}
