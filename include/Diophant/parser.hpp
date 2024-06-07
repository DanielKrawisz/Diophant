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

        // the last several expressions that have been generated. 
        // these could be for a list or some other structure. 
        data::stack<Expression> expr {};

        // there can be lists within lists and so on, so we have
        // a stack of stacks. 
        data::stack<data::stack<Expression>> back_expr {};
        
        // statements that have been generated in processing a
        // let expression. 
        data::stack<statement> statements;
        
        data::stack<data::stack<statement>> back_statements {};
        
        // list of symbols for constructing lambdas. 
        data::stack<Symbol> vars {};
        
        // whether we are reading a sequence of symbols for a
        // lambda now. 
        bool reading_lambda_vars {false};

        handler<Expression &> write;
        
        void define (string &, Expression &);

        void read_symbol (const data::string &in);
        void read_string (const data::string &in);
        void read_number (const data::string &in);
        
        void any ();
        void var ();

        void open_list ();
        void open_object ();
        void close_list ();
        void close_object ();
        
        void dif ();
        void let ();

        void start_vars ();
        void end_vars ();
        void lambda ();

        void call ();
        
        void unary (char op);

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
}

#endif

