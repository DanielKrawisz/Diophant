#include <Diophant/parser.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/lambda.hpp>

#include <tao/pegtl.hpp>

namespace parse {
    using namespace tao::pegtl;

    struct ws : star<space> {};

    // literals can be natural numbers or strings,

    // natural numbers are written in hex or decimal.
    struct hex_digit : seq<xdigit, xdigit> {};
    struct hex_lit : seq<string<'0', 'x'>, star<hex_digit>> {};
    struct dec_lit : sor<one<'0'>, seq<range<'1', '9'>, star<digit>>> {};

    struct number_lit : sor<dec_lit, hex_lit> {};

    // strings can have escaped characters with \ .
    struct string_body : star<sor<
        seq<not_at<sor<one<'\\'>, one<'"'>>>, ascii::print>,  // any ascii character other than \ and "
        seq<one<'\\'>, ascii::print>                          // \ followed by any printable character.
    >> {};

    // strings are written with ""
    struct string_lit : seq<
        one<'"'>,                                             // opening "
            string_body,
        one<'"'>> {};                                         // closing "

    // symbols are _ or alpha characters followed by _ and alphanumeric.
    struct symbol : seq<sor<alpha, one<'_'>>, star<sor<alnum, one<'_'>>>> {};

    // parentheses are only used to group expressions
    struct open_paren : one<'('> {};
    struct close_paren : one<')'> {};

    struct expression;
    struct type_expression;

    struct input : seq<type_expression, ws, one<';'>, ws, symbol> {};

    struct parenthetical : seq<open_paren, ws,
            expression,
            //star<seq<ws, one<','>, ws, expression>>,
        ws, close_paren> {};

    struct list : seq<one<'['>, ws,
            opt<seq<expression, ws, opt<star<seq<one<','>, ws, expression, ws>>>>>,
        one<']'>> {};

    struct map : seq<one<'{'>, ws, opt<
        seq<symbol, ws, string<'-', '>'>, ws, expression, ws, opt<star<
            seq<one<','>, ws, symbol, ws, string<'-', '>'>, expression, ws>>>>>, one<'}'>> {};

    struct structure;
    struct call : seq<plus<space>, structure> {};
    struct part : seq<one<'.'>, sor<number_lit, symbol>> {};
    struct structure : seq<sor<number_lit, string_lit, symbol,
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
    struct div_op : seq<ws, sor<one<'/'>, string<'/', '%'>>, ws, div_expr> {};
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

    struct arrow_expr;
    struct arrow_op : seq<ws, string<'-','>'>, ws, arrow_expr> {};

    struct arrow_expr : seq<bool_or_expr, opt<arrow_op>> {};

    struct intuitionistic_and_expr;
    struct intuitionistic_or_expr;
    struct type_expression;

    struct intuitionistic_and_op : seq<ws, one<'&'>, ws, intuitionistic_and_expr> {};
    struct intuitionistic_or_op : seq<ws, one<'|'>, ws, intuitionistic_or_expr> {};
    struct intuitionistic_implies_op : seq<ws, string<'=','>'>, ws, expression> {};

    struct intuitionistic_and_expr : seq<arrow_expr, opt<intuitionistic_and_op>> {};
    struct intuitionistic_or_expr : seq<intuitionistic_and_expr, opt<intuitionistic_or_op>> {};
    struct type_expression : seq<intuitionistic_or_expr, opt<intuitionistic_implies_op>> {};

    struct expression : seq<type_expression, opt<ws, one<':'>, symbol>> {};

    struct set : seq<one<'='>, ws, expression> {};
    struct infer : seq<string<':', '='>, ws, expression> {};
    struct declare : seq<one<':'>, ws, type_expression, opt<set>> {};

    struct statement : seq<arrow_expr, opt<ws, sor<infer, set, declare>>> {};

    struct program : seq<statement, opt<seq<ws, one<';'>, statement>>, eof> {};
}

namespace Diophant {
    namespace pegtl = tao::pegtl;

    template <typename Rule> struct eval_action : pegtl::nothing<Rule> {};

    template <> struct eval_action<parse::number_lit> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.read_number (in.string ());
        }
    };

    template <> struct eval_action<parse::string_body> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.read_string (in.string ());
        }
    };

    template <> struct eval_action<parse::symbol> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::call> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.call ();
        }
    };

    template <> struct eval_action<parse::negate_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.negate ();
        }
    };

    template <> struct eval_action<parse::bool_not_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.boolean_not ();
        }
    };

    template <> struct eval_action<parse::mul_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.mul ();
        }
    };

    template <> struct eval_action<parse::pow_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.pow ();
        }
    };

    template <> struct eval_action<parse::div_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.div ();
        }
    };

    template <> struct eval_action<parse::add_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.plus ();
        }
    };

    template <> struct eval_action<parse::sub_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.minus ();
        }
    };

    template <> struct eval_action<parse::equal_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.equal ();
        }
    };

    template <> struct eval_action<parse::unequal_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.unequal ();
        }
    };

    template <> struct eval_action<parse::greater_equal_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.greater_equal ();
        }
    };

    template <> struct eval_action<parse::less_equal_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.less_equal ();
        }
    };

    template <> struct eval_action<parse::less_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.less ();
        }
    };

    template <> struct eval_action<parse::greater_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.greater ();
        }
    };

    template <> struct eval_action<parse::bool_and_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.boolean_and ();
        }
    };

    template <> struct eval_action<parse::bool_or_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.boolean_or ();
        }
    };

    template <> struct eval_action<parse::arrow_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.arrow ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_and_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.intuitionistic_and ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_or_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.intuitionistic_or ();
        }
    };

    template <> struct eval_action<parse::intuitionistic_implies_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.intuitionistic_implies ();
        }
    };

    template <> struct eval_action<parse::declare> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.declare ();
        }
    };

    template <> struct eval_action<parse::set> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.set ();
        }
    };

    template <> struct eval_action<parse::infer> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.infer ();
        }
    };

    template <> struct eval_action<parse::statement> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            if (data::size (eval.stack) == 1) {
                auto v = evaluate (eval.stack.first (), eval.machine);
                eval.user.write (v);
                eval.stack = data::stack<Expression> {};
                eval.registered = eval.new_symbols;
            }
        }
    };

    void Parser::read_line (const std::string &in) {
        tao::pegtl::parse<parse::program, Diophant::eval_action> (tao::pegtl::memory_input<> {in, "expression"}, *this);
    }

    void Parser::initialize () {
        auto symbol_null = *expressions::symbol::make ("null", registered);
        auto symbol_true = *expressions::symbol::make ("true", registered);
        auto symbol_false = *expressions::symbol::make ("false", registered);

        auto symbol_Null = *expressions::symbol::make ("Null", registered);
        auto symbol_Bool = *expressions::symbol::make ("Bool", registered);
        auto symbol_Value = *expressions::symbol::make ("Value", registered);

        auto symbol_N = *expressions::symbol::make ("N", registered);
        auto symbol_Z = *expressions::symbol::make ("Z", registered);
        auto symbol_Q = *expressions::symbol::make ("Q", registered);

        machine.define (subject {symbol_null},
            predicate {ptr<function> {new user_defined {make::null ()}}});

        machine.define (subject {symbol_true},
            predicate {ptr<function> {new user_defined {make::boolean (true)}}});

        machine.define (subject {symbol_false},
            predicate {ptr<function> {new user_defined {make::boolean (false)}}});


    }

    void Parser::arrow () {
        Expression &x = first (rest (stack));
        auto z = dynamic_cast<Symbol *> (x.get ());
        if (z == nullptr) throw exception {} << "expected symbol, found " << x;
        stack = prepend (rest (rest (stack)), make::lambda (*z, first (stack)));
    }

    void Parser::open_list () {
        back = stack;
        stack = data::stack<Expression> {};
    }

    void Parser::open_object () {
        back = stack;
        stack = data::stack<Expression> {};
    }

    void Parser::close_list () {
        stack = prepend (first (back), make::list (stack));
        back = rest (back);
    }

    void Parser::close_object () {
        data::stack<entry<Expression, Expression>> m;
        while (data::size (stack) > 0) m <<= entry<Expression, Expression> {first (rest (stack)), first (stack)};
        stack = prepend (first (back), make::map (m));
        back = rest (back);
    }

    void Parser::declare () {
        machine.declare (subject::read (first (rest (stack))), Type {first (stack)});
        stack = rest (stack);
    }
    
    void Parser::set () {
        machine.define (subject::read (first (rest (stack))), predicate {ptr<function> {new user_defined {first (stack)}}});
        stack = prepend (rest (rest (stack)), first (stack));
    }
    
    void Parser::infer () {
        auto x = first (stack);
        machine.define (subject::read (first (rest (stack))), predicate {type_of (x), ptr<function> {new user_defined {x}}});
        stack = prepend (rest (rest (stack)), first (stack));
    }
}
