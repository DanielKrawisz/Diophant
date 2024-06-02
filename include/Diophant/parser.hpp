#ifndef DIOPHANT_PARSER
#define DIOPHANT_PARSER

#include <Diophant/machine.hpp>
#include <Diophant/expressions/pattern.hpp>

namespace Diophant {

    struct Parser {

        void read_line (const std::string &);
        void read_expression (const std::string &);
        void read_value (const std::string &);

        Parser (handler<Expression &> w) : write {w} {
            initialize ();
        }

        void initialize ();

        Machine machine {};

        data::stack<Expression> stack {};

        data::stack<data::stack<Expression>> back {};

        handler<Expression &> write;

        void read_symbol (const data::string &in);
        void read_string (const data::string &in);
        void read_number (const data::string &in);
        
        void any ();
        void var ();

        void open_list ();
        void open_object ();
        void close_list ();
        void close_object ();

        void lambda ();

        void call ();

        void negate ();
        void boolean_not ();

        void mul ();
        void pow ();
        void div ();
        void div_mod ();

        void plus ();
        void minus ();

        void greater_equal ();
        void less_equal ();
        void greater ();
        void less ();

        void bool_equal ();
        void bool_unequal ();

        void boolean_and ();
        void boolean_or ();
        
        void element ();

        void equal ();
        void unequal ();
        void double_implication_expr ();

        void intuitionistic_and ();
        void intuitionistic_or ();
        void intuitionistic_implies ();

        void such_that ();
        
        void set ();
    };
    
    void inline Parser::read_symbol (const data::string &in) {
        stack <<= make::symbol (in, machine.registered);
    }
    
    void inline Parser::any () {
        stack <<= make::any ();
    }
    
    void inline Parser::var () {
        stack = prepend (rest (stack), make::var (dynamic_cast<const expressions::symbol &> (*first (stack).get ())));
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

    void inline Parser::div_mod () {
        stack = prepend (rest (rest (stack)), make::div_mod (first (rest (stack)), first (stack)));
    }

    void inline Parser::plus () {
        stack = prepend (rest (rest (stack)), make::plus (first (rest (stack)), first (stack)));
    }

    void inline Parser::minus () {
        stack = prepend (rest (rest (stack)), make::minus (first (rest (stack)), first (stack)));
    }

    void inline Parser::bool_equal () {
        stack = prepend (rest (rest (stack)), make::bool_equal (first (rest (stack)), first (stack)));
    }

    void inline Parser::bool_unequal () {
        stack = prepend (rest (rest (stack)), make::bool_unequal (first (rest (stack)), first (stack)));
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

    void inline Parser::such_that () {
        stack = prepend (rest (rest (stack)), make::such_that (first (rest (stack)), first (stack)));
    }

    void inline Parser::element () {
        stack = prepend (rest (rest (stack)), make::element (first (rest (stack)), first (stack)));
    }

    void inline Parser::call () {
        stack = prepend (rest (rest (stack)), make::call (first (rest (stack)), first (stack)));
    }
}

#endif

