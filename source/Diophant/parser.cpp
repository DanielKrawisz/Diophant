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
    
    struct comment : seq<string<'/','<'>, star<seq<not_at<string<'>','/'>>, ascii::print>>, string<'>','/'>> {};
    struct white : sor<space, comment> {};
    struct ws : star<white> {};

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
    
    // symbols are alpha characters followed by _ and alphanumeric.
    struct normal_symbol : seq<alpha, star<sor<alnum, one<'_'>>>> {};
    
    // you can also have a symbol that's `any string inside these`. 
    struct abnormal_symbol : seq<one<'`'>, star<seq<not_at<one<'`'>>, sor<seq<string<'\\', '`'>>, ascii::print>>>, one<'`'>> {};

    struct symbol : sor<normal_symbol, abnormal_symbol> {};

    struct var : seq<one<'_'>, opt<symbol>> {};

    struct expression;
    struct statement;
    
    struct let : seq<string<'l', 'e', 't'>, not_at<ascii::print>, ws,
        statement, star<seq<ws, one<','>, statement>>, 
        string<'i','n'>, not_at<ascii::print>, ws, 
        expression> {};

    struct dif : seq<string<'i', 'f'>, not_at<ascii::print>, ws, expression, ws,
        string<'t', 'h', 'e', 'n'>, not_at<ascii::print>, ws, expression, ws,
        string<'e', 'l', 's', 'e'>, not_at<ascii::print>, ws, expression> {};

    struct lambda : seq<one<'@'>, ws, symbol, ws, string<'-','>'>, ws, expression> {};

    // parentheses are only used to group expressions
    struct open_paren : one<'('> {};
    struct close_paren : one<')'> {};

    struct parenthetical : seq<open_paren, ws, expression, ws, close_paren> {};
    
    struct open_list : one<'['> {};
    struct close_list : one<']'> {};
    struct list : seq<open_list, ws,
            opt<seq<expression, ws, star<seq<one<','>, ws, expression, ws>>>>,
        close_list> {};
    
    struct entry : seq<symbol, ws, one<':'>, ws, expression> {};
    struct dstruct : seq<one<'{'>, ws, opt<seq<entry, ws, star<seq<one<','>, ws, entry, ws>>>>, one<'}'>> {};
    
    struct structure : sor<number_lit, string_lit, symbol, var, let, parenthetical, list, dstruct, lambda> {};

    struct call : seq<plus<white>, structure> {};
    struct call_expr : seq<structure, star<call>> {};

    struct unary_operator : sor<one<'-'>, one<'!'>, one<'~'>, one<'+'>, one<'*'>> {};
    struct unary_expr : seq<star<unary_operator>, call_expr> {};

    struct pow_expr;
    struct mul_expr;
    struct mod_expr;
    struct div_mod_expr;
    struct div_expr;
    struct sub_expr;
    struct add_expr;

    struct pow_op : seq<ws, one<'^'>, ws, pow_expr> {};
    struct mul_op : seq<ws, sor<one<'*'>, one<'%'>, one<'~'>>, ws, mul_expr> {};
    struct mod_op : seq<ws, string<'%'>, ws, mod_expr> {};
    struct div_mod_op : seq<ws, string<'/', '%'>, ws, div_mod_expr> {};
    struct div_op : seq<ws, one<'/'>, ws, div_expr> {};
    struct sub_op : seq<ws, one<'-'>, ws, sub_expr> {};
    struct add_op : seq<ws, one<'+'>, ws, add_expr> {};

    struct pow_expr : seq<unary_expr, opt<pow_op>> {};
    struct mul_expr : seq<pow_expr, opt<mul_op>> {};
    struct mod_expr : seq<mul_expr, opt<mod_op>> {};
    struct div_mod_expr : seq<mod_expr, opt<div_mod_op>> {};
    struct div_expr : seq<div_mod_expr, opt<div_op>> {};
    struct sub_expr : seq<div_expr, opt<sub_op>> {};
    struct add_expr : seq<sub_expr, opt<add_op>> {};

    struct comp_expr;
    struct greater_equal_op : seq<ws, string<'>','='>, ws, comp_expr> {};
    struct less_equal_op : seq<ws, string<'<','='>, ws, comp_expr> {};
    struct greater_op : seq<ws, one<'>'>, ws, comp_expr> {};
    struct less_op : seq<ws, one<'<'>, ws, comp_expr> {};
    
    struct comp_expr : seq<add_expr,
        opt<sor<greater_equal_op, less_equal_op, greater_op, less_op>>> {};

    struct bool_equal_expr;

    struct bool_equal_op : seq<ws, string<'=','='>, ws, bool_equal_expr> {};
    struct bool_unequal_op : seq<ws, string<'!','='>, ws, bool_equal_expr> {};

    struct bool_equal_expr : seq<comp_expr, opt<sor<bool_equal_op, bool_equal_op>>> {};
    struct bool_unequal_expr : seq<bool_equal_expr, opt<sor<bool_equal_op, bool_unequal_op>>> {};

    struct bool_and_expr;
    struct bool_or_expr;

    struct bool_and_op : seq<ws, string<'&','&'>, ws, bool_and_expr> {};
    struct bool_or_op : seq<ws, string<'|','|'>, ws, bool_or_expr> {};

    struct bool_and_expr : seq<bool_unequal_expr, opt<bool_and_op>> {};
    struct bool_or_expr : seq<bool_and_expr, opt<bool_or_op>> {};

    struct element_op : seq<ws, one<':'>, ws, bool_or_expr> {};
    struct element_expr : seq<bool_or_expr, opt<element_op>> {};
    
    struct equal_expr;
    struct unequal_expr;
    struct double_implication_expr;
    
    struct equal_op : seq<ws, one<'='>, ws, equal_expr> {};
    struct unequal_op : seq<ws, string<'/', '='>, ws, unequal_expr> {};
    struct double_implication_op : seq<ws, string<'<','=','=','>'>, ws, double_implication_expr> {};
    
    struct equal_expr : seq<element_expr, opt<equal_op>> {};
    struct unequal_expr : seq<equal_expr, opt<unequal_op>> {};
    struct double_implication_expr : seq<unequal_expr, opt<double_implication_op>> {};
    
    struct intuitionistic_and_expr;
    struct intuitionistic_or_expr;

    struct intuitionistic_and_op : seq<ws, one<'&'>, ws, intuitionistic_and_expr> {};
    struct intuitionistic_or_op : seq<ws, one<'|'>, ws, intuitionistic_or_expr> {};
    struct intuitionistic_implies_op : seq<ws, string<'=','>'>, ws, expression> {};

    struct intuitionistic_and_expr : seq<double_implication_expr, opt<intuitionistic_and_op>> {};
    struct intuitionistic_or_expr : seq<intuitionistic_and_expr, opt<intuitionistic_or_op>> {};
    struct intuitionistic_implies_expr : seq<intuitionistic_or_expr, opt<intuitionistic_implies_op>> {};
    struct cast_expr : seq<intuitionistic_implies_expr, opt<seq<ws, string<'#'>, ws, intuitionistic_implies_expr>>> {};
    struct expression : cast_expr {};
    
    struct parse_expression : seq<expression, eof> {};

    struct such_that_op : seq<ws, string<'?'>, ws, intuitionistic_implies_expr> {};
    struct set : seq<string<'-', '>'>, ws, expression> {};

    struct statement : seq<expression, opt<ws, such_that_op>, opt<ws, set>> {};

    struct program : seq<statement, opt<seq<ws, one<';'>, statement>>, eof> {};
}

namespace Diophant {
    namespace pegtl = tao::pegtl;

    template <typename Rule> struct eval_action : pegtl::nothing<Rule> {};

    template <> struct eval_action<parse::number_lit> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " read number lit " << string_view {in.begin (), in.end () - in.begin ()} << std::endl; 
            eval.read_number (in.string ());
        }
    };

    template <> struct eval_action<parse::string_body> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.read_string (in.string ());
        }
    };

    template <> struct eval_action<parse::normal_symbol> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " read symbol " << string_view {in.begin (), in.end () - in.begin ()} << std::endl; 
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::abnormal_symbol> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " read symbol " << string_view {in.begin (), in.end () - in.begin ()} << std::endl; 
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::var> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " read var " << string_view {in.begin (), in.end () - in.begin ()} << std::endl; 
            if (in.size () == 1) eval.any ();
            eval.var ();
        }
    };

    template <> struct eval_action<parse::call> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.call ();
        }
    };

    template <> struct eval_action<parse::open_list> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.open_list ();
        }
    };

    template <> struct eval_action<parse::close_list> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.close_list ();
        }
    };

    template <> struct eval_action<parse::lambda> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.lambda ();
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

    template <> struct eval_action<parse::div_mod_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.div_mod ();
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

    template <> struct eval_action<parse::bool_equal_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.bool_equal ();
        }
    };

    template <> struct eval_action<parse::bool_unequal_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.bool_unequal ();
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

    template <> struct eval_action<parse::element_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " reading element op \"" << string_view {in.begin (), in.end () - in.begin ()} << "\"" << std::endl;
            eval.element ();
        }
    };

    template <> struct eval_action<parse::such_that_op> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " read such that op \"" << string_view {in.begin (), in.end () - in.begin ()} << "\"" << std::endl;
            eval.such_that ();
        }
    };

    template <> struct eval_action<parse::set> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " read set \"" << string_view {in.begin (), in.end () - in.begin ()} << "\"" << std::endl;
            eval.set ();
        }
    };

    template <> struct eval_action<parse::program> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            std::cout << " about to evaluate program " << string_view {in.begin (), in.end () - in.begin ()} << std::endl; 
            if (data::size (eval.stack) >= 1) {
                auto v = evaluate (eval.stack.first (), eval.machine);
                eval.write (v);
                eval.stack = data::stack<Expression> {};
            }
        }
    };

    template <> struct eval_action<parse::parse_expression> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            if (data::size (eval.stack) >= 1) {
                eval.write (eval.stack.first ());
                eval.stack = data::stack<Expression> {};
            }
        }
    };

    void Parser::read_line (const std::string &in) {
        std::cout << "read line \"" << in << "\"" << std::endl;
        tao::pegtl::parse<parse::program, Diophant::eval_action> (tao::pegtl::memory_input<> {in, "expression"}, *this);
    }

    void Parser::read_expression (const std::string &in) {
        tao::pegtl::parse<parse::parse_expression, Diophant::eval_action> (tao::pegtl::memory_input<> {in, "expression"}, *this);
    }

    void Parser::initialize () {
        make::symbol ("Impossible", machine.registered);
        make::symbol ("Null", machine.registered);
        make::symbol ("Bool", machine.registered);
        make::symbol ("Intuit", machine.registered);
        make::symbol ("Sign", machine.registered);
        make::symbol ("N", machine.registered);
        make::symbol ("Z", machine.registered);
        make::symbol ("Q", machine.registered);
        make::symbol ("Float", machine.registered);
        make::symbol ("String", machine.registered);
        make::symbol ("Int", machine.registered);
        make::symbol ("Uint", machine.registered);
        make::symbol ("List", machine.registered);
        make::symbol ("Tupple", machine.registered);
        make::symbol ("Struct", machine.registered);
        make::symbol ("Array", machine.registered);
        make::symbol ("!l", machine.registered);
        make::symbol ("!r", machine.registered);
        make::symbol ("++l", machine.registered);
        make::symbol ("--l", machine.registered);
        make::symbol ("^", machine.registered);
        make::symbol ("*", machine.registered);
        make::symbol ("%", machine.registered);
        make::symbol ("/", machine.registered);
        make::symbol ("/%", machine.registered);
        make::symbol ("<", machine.registered);
        make::symbol (">", machine.registered);
        make::symbol ("<=", machine.registered);
        make::symbol (">=", machine.registered);
        make::symbol ("==", machine.registered);
        make::symbol ("!=", machine.registered);
        make::symbol ("&&", machine.registered);
        make::symbol ("||", machine.registered);
        make::symbol ("=", machine.registered);
        make::symbol ("/=", machine.registered);
        make::symbol ("<==>", machine.registered);
        make::symbol (":", machine.registered);
        make::symbol ("&", machine.registered);
        make::symbol ("|", machine.registered);
        
        read_line (R"(_x == _x ? x:Value -> true; )");
        read_line (R"(_x == _y ? x:Value & y:Value -> false; )");
        read_line (R"(not true -> false; )");
        read_line (R"(not false -> true; )");
        read_line (R"(!_x ? x:Bool -> not; )");
        read_line (R"(_x != _y ? x:Value & y:Value -> !(x == y); )");
        read_line (R"(and true true -> true; )");
        read_line (R"(and _x _y ? x:Bool & y:Bool -> false; )");
        read_line (R"(or false false -> false; )");
        read_line (R"(or _x _y ? x:Bool & y:Bool -> false; )");
        read_line (R"(xor _x _y ? x:Bool & y:Bool -> `!=`; )");
        read_line (R"(implies _x _y ? x:Bool & y:Bool -> or y !x; )");
        read_line (R"(nand true true -> false; )");
        read_line (R"(nand _x _y ? x:Bool y:Bool -> true; )");
        read_line (R"(nor false false -> true; )");
        read_line (R"(nor _x _y ? x:Bool & y:Bool -> false; )");
        read_line (R"(`&&` -> Bool => Bool => Bool # and; )");
        read_line (R"(`||` -> Bool => Bool => Bool # or; )");
        read_line (R"(_x : Nonzero _n -> x : N & x != 0 )");
        read_line (R"(+N -> Nonzero N; )");
        read_line (R"(`++` ? `++` : N => +N; )");
        read_line (R"(`--` ? `--` : +N => N; )");
        read_line (R"(`+` ? `+` : N => N => N; )");
        read_line (R"(`-` ? `-` : N => N => N | Null; )");
        read_line (R"(`*` ? `*` : N => N => N; )");
        read_line (R"(`/%` ? `/%` : N => +N => {quotient: N, remainder: N}; )");
        read_line (R"(`/` ? `/` : N => +N => Q; )");
        read_line (R"(`++` ? `++` : Z => Z; )");
        read_line (R"(`--` ? `--` : Z => Z; )");
        read_line (R"(`+` ? `+` : Z => Z => Z; )");
        read_line (R"(`-` ? `-` : Z => Z => Z; )");
        read_line (R"(`*` ? `*` : Z => Z => Z; )");
        read_line (R"(`/%` ? `/%` : Z => Nonzero Z => {quotient: Z, remainder: N}; )");
        read_line (R"(`/` ? `/` : Z => Nonzero Z => Q; )");
        read_line (R"(`+` ? `+` : Q => Q => Q; )");
        read_line (R"(`-` ? `-` : Q => Q => Q; )");
        read_line (R"(`*` ? `*` : Q => Q => Q; )");
        read_line (R"(`/` ? `/` : Q => Nonzero Q => Q; )");
        read_line (R"(`>` ? `>` : Q => Q => Bool; )");
        read_line (R"(`<` ? `<` : Q => Q => Bool; )");
        read_line (R"(`>=` ? `>=` : Q => Q => Bool; )");
        read_line (R"(`<=` ? `<=` : Q => Q => Bool; )");
        read_line (R"(prepend ? prepend : Stack x => x => Stack x; )");
        read_line (R"(first ? first : Stack x => x | Null; )");
        read_line (R"(rest ? rest : Stack x => Stack x; )");
        
        // TODO fill in the missing functions

    }

    void Parser::lambda () {
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
    
    void Parser::set () {
        machine.define (machine.make_subject (first (rest (stack))), first (stack));
        stack = prepend (rest (rest (stack)), first (stack));
    }
}
