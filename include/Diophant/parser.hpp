#ifndef DIOPHANT_PARSER
#define DIOPHANT_PARSER

#include <Diophant/machine.hpp>

namespace Diophant {

    struct Parser {

        void read_line (const std::string &);
        void read_expression (const std::string &);
        void read_value (const std::string &);

        Parser (handler<Expression &> w) : write {w} {
            initialize ();
        }

        void initialize ();

        // set of registered symbols.
        symbols registered {};

        // new symbols that are found within a command as it is being executed.
        // added to registered symbols if the command is read successfully.
        symbols new_symbols {};

        Machine machine {};

        data::stack<Expression> stack {};

        data::stack<data::stack<Expression>> back {};

        handler<Expression &> write;

        void read_symbol (const data::string &in);
        void read_string (const data::string &in);
        void read_number (const data::string &in);

        void open_list ();
        void open_object ();
        void close_list ();
        void close_object ();

        void call ();

        void negate ();
        void mul ();
        void pow ();
        void div ();
        void plus ();
        void minus ();

        void equal ();
        void unequal ();
        void greater_equal ();
        void less_equal ();
        void greater ();
        void less ();

        void boolean_not ();
        void boolean_and ();
        void boolean_or ();

        void lambda ();

        void intuitionistic_and ();
        void intuitionistic_or ();
        void intuitionistic_implies ();
        
        void set ();
    };
    
    void inline Parser::read_symbol (const data::string &in) {
        stack <<= make::symbol (in, new_symbols);
    }

    void inline Parser::read_string (const data::string &in) {
        stack <<= make::string (in);
    }

    void inline Parser::read_number (const data::string &in) {
        stack <<= make::natural (N {in});
    }

    void inline Parser::negate () {
        stack = prepend (rest (stack), make::negate (first (stack)));
    }

    void inline Parser::boolean_not () {
        stack = prepend (rest (stack), make::boolean_not (first (stack)));
    }

    void inline Parser::mul () {
        stack = prepend (rest (rest (stack)), make::times (first (rest (stack)), first (stack)));
    }

    void inline Parser::pow () {
        stack = prepend (rest (rest (stack)), make::power (first (rest (stack)), first (stack)));
    }

    void inline Parser::div () {
        stack = prepend (rest (rest (stack)), make::divide (first (rest (stack)), first (stack)));
    }

    void inline Parser::plus () {
        stack = prepend (rest (rest (stack)), make::plus (first (rest (stack)), first (stack)));
    }

    void inline Parser::minus () {
        stack = prepend (rest (rest (stack)), make::minus (first (rest (stack)), first (stack)));
    }

    void inline Parser::equal () {
        stack = prepend (rest (rest (stack)), make::equal (first (rest (stack)), first (stack)));
    }

    void inline Parser::unequal () {
        stack = prepend (rest (rest (stack)), make::unequal (first (rest (stack)), first (stack)));
    }

    void inline Parser::greater_equal () {
        stack = prepend (rest (rest (stack)), make::greater_equal (first (rest (stack)), first (stack)));
    }

    void inline Parser::less_equal () {
        stack = prepend (rest (rest (stack)), make::less_equal (first (rest (stack)), first (stack)));
    }

    void inline Parser::greater () {
        stack = prepend (rest (rest (stack)), make::greater (first (rest (stack)), first (stack)));
    }

    void inline Parser::less () {
        stack = prepend (rest (rest (stack)), make::less (first (rest (stack)), first (stack)));
    }

    void inline Parser::boolean_and () {
        stack = prepend (rest (rest (stack)), make::boolean_and (first (rest (stack)), first (stack)));
    }

    void inline Parser::boolean_or () {
        stack = prepend (rest (rest (stack)), make::boolean_or (first (rest (stack)), first (stack)));
    }

    void inline Parser::intuitionistic_and () {
        stack = prepend (rest (rest (stack)), make::intuitionistic_and (first (rest (stack)), first (stack)));
    }

    void inline Parser::intuitionistic_or () {
        stack = prepend (rest (rest (stack)), make::intuitionistic_or (first (rest (stack)), first (stack)));
    }

    void inline Parser::intuitionistic_implies () {
        stack = prepend (rest (rest (stack)), make::intuitionistic_implies (first (rest (stack)), first (stack)));
    }

    void inline Parser::call () {
        stack = prepend (rest (rest (stack)), make::call (first (rest (stack)), first (stack)));
    }
}

#endif

