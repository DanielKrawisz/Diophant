
#include <Diophant/parser.hpp>

using namespace Diophant;

struct Console : User {
    maybe<std::string> read () override {
        std::string input;
        std::cout << "\n input: ";
        if (!std::getline (std::cin, input)) return {};
        return {input};
    }

    void write (Expression &x) override {
        std::cout << "\n result: " << std::string (x) << std::endl;
    }
};

int main (int args, char **arg) {

    std::cout << "\nDiophant engaged! " << std::endl;

    // talk to the user via the console.
    Console u {};

    Parser eval {u};
    
    try {
        while (true) {
            maybe<string> input = u.read ();
            if (!bool (input)) break;
            if (input->empty ()) continue;

            // for each input line, attempt to construct an expression
            // out of it and then evaluate it.
            try {
                eval.read_line (*input);
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
