#ifndef DIOPHANT_PARSER
#define DIOPHANT_PARSER

#include <tao/pegtl.hpp>
#include <Diophant/machine.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/symbol.hpp>

namespace parse {
    using namespace tao::pegtl;

    struct ws : star<space> {};

    struct hex_digit : seq<xdigit, xdigit> {};
    struct hex_lit : seq<string<'0', 'x'>, star<hex_digit>> {};
    struct dec_lit : sor<one<'0'>, seq<range<'1', '9'>, star<digit>>> {};

    struct number_lit : sor<dec_lit, hex_lit> {};

    struct string_body : star<sor<
        seq<not_at<sor<one<'\\'>, one<'"'>>>, ascii::print>,  // any ascii character other than \ and "
        seq<one<'\\'>, ascii::print>                          // \ followed by any printable character.
    >> {};

    struct string_lit : seq<
        one<'"'>,                                             // opening "
            string_body,
        one<'"'>> {};                                         // closing "

    struct symbol : seq<sor<alpha, one<'_'>>, star<sor<alnum, one<'_'>>>> {};

    struct open_paren : one<'('> {};
    struct close_paren : one<')'> {};

    struct expression;
    struct type_expression;

    struct parenthetical : seq<open_paren, ws, 
            expression, 
            star<seq<ws, one<','>, ws, expression>>, ws, 
        close_paren> {};
        
    struct list : seq<one<'['>, ws, 
            opt<seq<expression, ws, opt<star<seq<one<','>, ws, expression, ws>>>>>, 
        one<']'>> {};
    
    struct map : seq<one<'{'>, ws, opt<
        seq<symbol, ws, one<':'>, ws, expression, ws, opt<star<
            seq<one<','>, ws, symbol, ws, one<':'>, expression, ws>>>>>, one<'}'>> {};

    struct typed_input : seq<one<'.'>, ws, type_expression> {};
    struct untyped_input : seq<one<';'>> {};

    struct structure;
    struct call : seq<plus<space>, structure> {};
    struct part : seq<one<'@'>, sor<number_lit, symbol>> {};
    struct structure : seq<sor<number_lit, string_lit, 
        seq<symbol, typed_input, untyped_input>, 
        parenthetical, list, map>, star<part>, star<call>> {};

    struct unary_operator : sor<one<'~'>, one<'+'>, one<'*'>> {};

    struct unary_expr;
    struct negate_op : seq<one<'-'>, ws, unary_expr> {};
    struct bool_not_op : seq<one<'!'>, ws, unary_expr> {};
    struct unary_op : sor<negate_op, bool_not_op, seq<unary_operator, ws, unary_expr>> {};
    struct unary_expr : sor<unary_op, structure> {};

    struct mul_expr;
    struct pow_expr;
    struct div_expr;
    struct sub_expr;
    struct add_expr;

    struct mul_op : seq<ws, sor<one<'*'>, one<'%'>, one<'~'>>, ws, mul_expr> {};
    struct pow_op : seq<ws, one<'^'>, ws, div_expr> {};
    struct div_op : seq<ws, one<'/'>, ws, div_expr> {};
    struct sub_op : seq<ws, one<'-'>, ws, sub_expr> {};
    struct add_op : seq<ws, one<'+'>, ws, add_expr> {};

    struct mul_expr : seq<unary_expr, opt<mul_op>> {};
    struct pow_expr : seq<mul_expr, opt<pow_op>> {};
    struct div_expr : seq<pow_expr, opt<div_op>> {};
    struct sub_expr : seq<div_expr, opt<sub_op>> {};
    struct add_expr : seq<sub_expr, opt<add_op>> {};

    struct comp_expr;

    struct equal_op : seq<ws, string<'=','='>, ws, comp_expr> {};
    struct unequal_op : seq<ws, string<'!','='>, ws, comp_expr> {};
    struct greater_equal_op : seq<ws, string<'>','='>, ws, comp_expr> {};
    struct less_equal_op : seq<ws, string<'<','='>, ws, comp_expr> {};
    struct greater_op : seq<ws, one<'>'>, ws, comp_expr> {};
    struct less_op : seq<ws, one<'<'>, ws, comp_expr> {};

    struct comp_expr : seq<add_expr, 
        opt<sor<equal_op, unequal_op, greater_equal_op, less_equal_op, greater_op, less_op>>> {};

    struct bool_and_expr;
    struct bool_or_expr;

    struct bool_and_op : seq<ws, string<'&','&'>, ws, bool_and_expr> {};
    struct bool_or_op : seq<ws, string<'|','|'>, ws, bool_or_expr> {};

    struct bool_and_expr : seq<comp_expr, opt<bool_and_op>> {};
    struct bool_or_expr : seq<bool_and_expr, opt<bool_or_op>> {};

    struct arrow_op : seq<ws, one<'-','>'>, expression> {};
    struct expression : seq<bool_or_expr, opt<arrow_op>> {};

    struct intuitionistic_and_expr;
    struct intuitionistic_or_expr;
    struct type_expression;

    struct intuitionistic_and_op : seq<ws, one<'&'>, ws, intuitionistic_and_expr> {};
    struct intuitionistic_or_op : seq<ws, one<'|'>, ws, intuitionistic_or_expr> {};
    struct intuitionistic_implies_op : seq<ws, string<'=','>'>, ws, expression> {};

    struct intuitionistic_and_expr : seq<expression, opt<intuitionistic_and_op>> {};
    struct intuitionistic_or_expr : seq<intuitionistic_and_expr, opt<intuitionistic_or_op>> {};
    struct type_expression : seq<intuitionistic_or_expr, opt<intuitionistic_implies_op>> {};
    
    struct set : seq<one<'='>, ws, expression> {};
    struct infer : seq<string<':', '='>, ws, expression> {};
    struct declare : seq<one<':'>, ws, expression, opt<set>> {};

    struct statement : seq<expression, opt<ws, sor<infer, set, declare>>> {};

    struct grammar : seq<statement, ws> {};
}

namespace Diophant {

    struct User {
        // Read a line of user input.
        virtual maybe<std::string> read () = 0;
        // write a result to the user.
        virtual void write (const std::string &) = 0;
    };

    namespace pegtl = tao::pegtl;

    struct Parser {
        Machine machine {};
        data::stack<Expression> stack {};
        User &user;

        Parser (User &u) : user {u} {}

        void read_symbol (const data::string &in);
        void read_string (const data::string &in);
        void read_number (const data::string &in);

        void open_list ();
        void open_object ();
        void close_list ();
        void close_object ();
        void comma ();

        void apply ();

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

        void arrow ();

        void intuitionistic_and ();
        void intuitionistic_or ();
        void intuitionistic_implies ();

        void declare ();
        void infer ();
        void set ();
    };

    template <typename Rule> struct eval_action : pegtl::nothing<Rule> {};

    template <> struct eval_action<parse::number_lit> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.read_number (in.string ());
        }
    };

    template <> struct eval_action<parse::string_body> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.read_string (in.string ());
        }
    };

    template <> struct eval_action<parse::symbol> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::negate_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.negate ();
        }
    };

    template <> struct eval_action<parse::bool_not_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.boolean_not ();
        }
    };

    template <> struct eval_action<parse::mul_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.mul ();
        }
    };

    template <> struct eval_action<parse::pow_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.pow ();
        }
    };

    template <> struct eval_action<parse::div_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.div ();
        }
    };

    template <> struct eval_action<parse::add_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.plus ();
        }
    };

    template <> struct eval_action<parse::sub_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.minus ();
        }
    };

    template <> struct eval_action<parse::equal_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.equal ();
        }
    };

    template <> struct eval_action<parse::unequal_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.unequal ();
        }
    };

    template <> struct eval_action<parse::greater_equal_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.greater_equal ();
        }
    };

    template <> struct eval_action<parse::less_equal_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.less_equal ();
        }
    };

    template <> struct eval_action<parse::less_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.less ();
        }
    };

    template <> struct eval_action<parse::greater_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.greater ();
        }
    };

    template <> struct eval_action<parse::bool_and_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.boolean_and ();
        }
    };

    template <> struct eval_action<parse::bool_or_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.boolean_or ();
        }
    };

    template <> struct eval_action<parse::arrow_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.arrow ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_and_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.intuitionistic_and ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_or_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.intuitionistic_or ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_implies_op> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.intuitionistic_implies ();
        }
    };

    template <> struct eval_action<parse::expression> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {}
    };

    template <> struct eval_action<parse::set> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.set ();
        }
    };

    template <> struct eval_action<parse::declare> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.declare ();
        }
    };

    template <> struct eval_action<parse::infer> {
        template <typename Input>
        static void apply (const Input& in, Parser &eval) {
            eval.infer ();
        }
    };

    template <> struct eval_action<parse::statement> {
        template <typename Input>
        static void apply (const Input&, Parser &eval) {
            if (data::size (eval.stack) == 1) {
                auto v = evaluate (eval.stack.first (), eval.machine);
                eval.user.write (std::string (v));
                eval.stack = data::stack<Expression> {};
            }
        }
    };
    
    void inline Parser::read_symbol (const data::string &in) {
        stack <<= make::symbol (in);
    }

    void inline Parser::read_string (const data::string &in) {
        stack <<= make::string (in);
    }

    void inline Parser::read_number (const data::string &in) {
        stack <<= make::rational (Q {Z {in}});;
    }

    void inline Parser::apply () {
        stack = prepend (rest (rest (stack)), make::apply (first (rest (stack)), first (stack)));
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

    void inline Parser::arrow () {
        stack = prepend (rest (rest (stack)), make::arrow (first (rest (stack)), first (stack)));
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
}

#endif

