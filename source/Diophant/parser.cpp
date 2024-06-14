#include <Diophant/parser.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/if.hpp>
#include <Diophant/expressions/let.hpp>
#include <Diophant/make.hpp>

#include <tao/pegtl.hpp>

namespace parse {
    using namespace tao::pegtl;
    
    struct comment : seq<string<'/','<'>, star<seq<not_at<string<'>','/'>>, ascii::print>>, string<'>','/'>> {};
    struct white : sor<one<' '>, one<'\t'>, one<'\n'>, comment> {};
    struct ws : star<white> {};
    
    struct statement;
    struct expression;
    
    // the overall structure of a Diophant program is a series of statements separated by ; followed by an expression. 
    struct statement_separator : one<';'> {};
    struct program : seq<star<seq<ws, statement, ws, statement_separator>>, ws, expression, ws, eof> {};
    
    struct pattern;

    // statements are of the form pattern := expression. 
    struct set : seq<string<':', '='>, ws, expression> {};
    struct statement : seq<pattern, ws, set> {};

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
    struct symbol_char : sor<alnum, one<'_'>> {};
    struct normal_symbol : seq<alpha, star<symbol_char>> {};
    
    // you can also have a symbol that's `any string inside these`. 
    struct abnormal_symbol : star<seq<not_at<one<'`'>>, sor<seq<string<'\\', '`'>>, ascii::print>>> {};

    struct symbol : minus<sor<normal_symbol, seq<one<'`'>, abnormal_symbol, one<'`'>>>,
        sor<string<'i', 'f'>, string<'i', 'n'>, string<'l', 'e', 't'>, string<'t', 'h', 'e', 'n'>, string<'e', 'l', 's', 'e'>>> {};

    struct var : seq<one<'_'>, opt<symbol>> {};
    
    struct let_open : seq<string<'l', 'e', 't'>, not_at<symbol_char>> {};
    struct let_in : seq<string<'i','n'>, not_at<symbol_char>> {};
    struct rule : seq<pattern, ws, string<'-', '>'>, ws, expression> {};
    struct let : seq<let_open, ws, rule, star<seq<ws, one<','>, ws, rule>>, ws, let_in, ws, expression> {};

    struct dif : seq<string<'i', 'f'>, not_at<symbol_char>, ws, expression, ws,
        string<'t', 'h', 'e', 'n'>, not_at<symbol_char>, ws, expression, ws,
        string<'e', 'l', 's', 'e'>, not_at<symbol_char>, ws, expression> {};
    
    struct lambda_start : one<'@'> {};
    struct lambda_arrow : string<'-','>'> {};
    struct lambda : seq<lambda_start, ws, plus<seq<symbol, ws>>, lambda_arrow, ws, expression> {};

    // parentheses are only used to group expressions
    struct open_paren : one<'('> {};
    struct close_paren : one<')'> {};

    struct parenthetical : seq<open_paren, ws, expression, ws, close_paren> {};
    
    struct open_list : one<'['> {};
    struct close_list : one<']'> {};
    struct list : seq<open_list,
            opt<seq<ws, expression, ws, star<seq<one<','>, ws, expression, ws>>>>, 
        close_list> {};
    
    struct entry : seq<symbol, ws, string<'-', '>'>, ws, expression> {};
    struct dstruct : seq<one<'{'>, ws, opt<seq<entry, ws, star<seq<one<','>, ws, entry, ws>>>>, one<'}'>> {};
    
    struct expression_atom : sor<number_lit, string_lit, dif, let, symbol, parenthetical, list, lambda, dstruct> {};
    struct pattern_atom : sor<number_lit, string_lit, symbol, var, parenthetical, list> {};

    template <typename atom> struct call : seq<plus<white>, atom> {};
    template <typename atom> struct call_expr : seq<atom, star<call<atom>>> {};
    
    struct unary_operator : sor<one<'-'>, one<'!'>, one<'~'>, one<'+'>, one<'*'>> {};

    template <typename atom> struct unary_expr;
    template <typename atom> struct unary_operation : seq<unary_operator, unary_expr<atom>> {};
    template <typename atom> struct unary_expr : sor<unary_operation<atom>, call_expr<atom>> {};

    template <typename atom> struct pow_expr;
    template <typename atom> struct mul_expr;
    template <typename atom> struct mod_expr;
    template <typename atom> struct div_mod_expr;
    template <typename atom> struct div_expr;
    template <typename atom> struct sub_expr;
    template <typename atom> struct add_expr;

    template <typename atom> struct pow_op : seq<ws, one<'^'>, ws, pow_expr<atom>> {};
    template <typename atom> struct mul_op : seq<ws, sor<one<'*'>, one<'%'>, one<'~'>>, ws, mul_expr<atom>> {};
    template <typename atom> struct mod_op : seq<ws, string<'%'>, ws, mod_expr<atom>> {};
    template <typename atom> struct div_mod_op : seq<ws, string<'/', '%'>, ws, div_mod_expr<atom>> {};
    template <typename atom> struct div_op : seq<ws, one<'/'>, ws, div_expr<atom>> {};
    template <typename atom> struct sub_op : seq<ws, one<'-'>, ws, sub_expr<atom>> {};
    template <typename atom> struct add_op : seq<ws, one<'+'>, ws, add_expr<atom>> {};

    template <typename atom> struct pow_expr : seq<unary_expr<atom>, opt<pow_op<atom>>> {};
    template <typename atom> struct mul_expr : seq<pow_expr<atom>, opt<mul_op<atom>>> {};
    template <typename atom> struct mod_expr : seq<mul_expr<atom>, opt<mod_op<atom>>> {};
    template <typename atom> struct div_mod_expr : seq<mod_expr<atom>, opt<div_mod_op<atom>>> {};
    template <typename atom> struct div_expr : seq<div_mod_expr<atom>, opt<div_op<atom>>> {};
    template <typename atom> struct sub_expr : seq<div_expr<atom>, opt<sub_op<atom>>> {};
    template <typename atom> struct add_expr : seq<sub_expr<atom>, opt<add_op<atom>>> {};

    template <typename atom> struct comp_expr;
    template <typename atom> struct greater_equal_op : seq<ws, string<'>','='>, ws, comp_expr<atom>> {};
    template <typename atom> struct less_equal_op : seq<ws, string<'<','='>, ws, comp_expr<atom>> {};
    template <typename atom> struct greater_op : seq<ws, one<'>'>, ws, comp_expr<atom>> {};
    template <typename atom> struct less_op : seq<ws, one<'<'>, ws, comp_expr<atom>> {};
    
    template <typename atom> struct comp_expr : seq<add_expr<atom>,
        opt<sor<greater_equal_op<atom>, less_equal_op<atom>, greater_op<atom>, less_op<atom>>>> {};

    template <typename atom> struct bool_equal_expr;

    template <typename atom> struct bool_equal_op : seq<ws, string<'=','='>, ws, bool_equal_expr<atom>> {};
    template <typename atom> struct bool_unequal_op : seq<ws, string<'!','='>, ws, bool_equal_expr<atom>> {};

    template <typename atom> struct bool_equal_expr : seq<comp_expr<atom>, opt<sor<bool_equal_op<atom>, bool_equal_op<atom>>>> {};
    template <typename atom> struct bool_unequal_expr : 
        seq<bool_equal_expr<atom>, opt<sor<bool_equal_op<atom>, bool_unequal_op<atom>>>> {};

    template <typename atom> struct bool_and_expr;
    template <typename atom> struct bool_or_expr;

    template <typename atom> struct bool_and_op : seq<ws, string<'&','&'>, ws, bool_and_expr<atom>> {};
    template <typename atom> struct bool_or_op : seq<ws, string<'|','|'>, ws, bool_or_expr<atom>> {};

    template <typename atom> struct bool_and_expr : seq<bool_unequal_expr<atom>, opt<bool_and_op<atom>>> {};
    template <typename atom> struct bool_or_expr : seq<bool_and_expr<atom>, opt<bool_or_op<atom>>> {};

    template <typename atom> struct element_op : seq<ws, one<':'>, ws, bool_or_expr<atom>> {};
    template <typename atom> struct element_expr : seq<bool_or_expr<atom>, opt<element_op<atom>>> {};
    
    template <typename atom> struct equal_expr;
    template <typename atom> struct unequal_expr;
    template <typename atom> struct double_implication_expr;
    
    template <typename atom> struct equal_op : seq<ws, one<'='>, ws, equal_expr<atom>> {};
    template <typename atom> struct unequal_op : seq<ws, string<'/', '='>, ws, unequal_expr<atom>> {};
    template <typename atom> struct double_implication_op : seq<ws, string<'<','=','=','>'>, ws, double_implication_expr<atom>> {};
    
    template <typename atom> struct equal_expr : seq<element_expr<atom>, opt<equal_op<atom>>> {};
    template <typename atom> struct unequal_expr : seq<equal_expr<atom>, opt<unequal_op<atom>>> {};
    template <typename atom> struct double_implication_expr : seq<unequal_expr<atom>, opt<double_implication_op<atom>>> {};
    
    template <typename atom> struct intuitionistic_and_expr;
    template <typename atom> struct intuitionistic_or_expr;
    template <typename atom> struct intuitionistic_implies_expr;

    template <typename atom> struct intuitionistic_and_op : seq<ws, one<'&'>, ws, intuitionistic_and_expr<atom>> {};
    template <typename atom> struct intuitionistic_or_op : seq<ws, one<'|'>, ws, intuitionistic_or_expr<atom>> {};
    template <typename atom> struct intuitionistic_implies_op : 
        seq<ws, string<'=','>'>, ws, intuitionistic_implies_expr<atom>> {};

    template <typename atom> struct intuitionistic_and_expr : seq<double_implication_expr<atom>, opt<intuitionistic_and_op<atom>>> {};
    template <typename atom> struct intuitionistic_or_expr : seq<intuitionistic_and_expr<atom>, opt<intuitionistic_or_op<atom>>> {};
    template <typename atom> struct intuitionistic_implies_expr : 
        seq<intuitionistic_or_expr<atom>, opt<intuitionistic_implies_op<atom>>> {};
    
    template <typename atom> struct such_that_expr;
    template <typename atom> struct such_that_op : seq<ws, string<'?'>, ws, intuitionistic_implies_expr<atom>> {};
    template <typename atom> struct such_that_expr : seq<intuitionistic_implies_expr<atom>, opt<such_that_op<atom>>> {};
    
    template <typename atom> struct replace_op : seq<ws, string<'/', '.'>, ws, dstruct> {};
    template <typename atom> struct replace_expr : seq<such_that_expr<atom>, opt<replace_op<atom>>> {};
    
    template <typename atom> struct cast_expr : 
        seq<replace_expr<atom>, opt<seq<ws, string<'#'>, ws, intuitionistic_implies_expr<atom>>>> {};
    
    struct expression : cast_expr<expression_atom> {};
    struct pattern : replace_expr<pattern_atom> {};
    
    struct parse_expression : seq<ws, expression, ws, eof> {};

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

    template <> struct eval_action<parse::normal_symbol> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::abnormal_symbol> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.read_symbol (in.string ());
        }
    };

    template <> struct eval_action<parse::var> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            if (in.size () == 1) eval.any ();
            eval.var ();
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

    template <> struct eval_action<parse::lambda_start> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.start_vars ();
        }
    };

    template <> struct eval_action<parse::lambda_arrow> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.end_vars ();
        }
    };

    template <> struct eval_action<parse::lambda> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.lambda ();
        }
    };

    template <> struct eval_action<parse::dif> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.dif ();
        }
    };

    template <typename atom> struct eval_action<parse::call<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.call ();
        }
    };

    template <typename atom> struct eval_action<parse::unary_operation<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            // NOTE this won't work if we ever have any unary operators that are bigger than one char. 
            eval.unary (*in.begin ());
        }
    };

    template <typename atom> struct eval_action<parse::mul_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.mul ();
        }
    };

    template <typename atom> struct eval_action<parse::pow_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.pow ();
        }
    };

    template <typename atom> struct eval_action<parse::div_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.div ();
        }
    };

    template <typename atom> struct eval_action<parse::div_mod_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.div_mod ();
        }
    };

    template <typename atom> struct eval_action<parse::add_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.plus ();
        }
    };

    template <typename atom> struct eval_action<parse::sub_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.minus ();
        }
    };

    template <typename atom> struct eval_action<parse::greater_equal_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.greater_equal ();
        }
    };

    template <typename atom> struct eval_action<parse::less_equal_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.less_equal ();
        }
    };

    template <typename atom> struct eval_action<parse::less_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.less ();
        }
    };

    template <typename atom> struct eval_action<parse::greater_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.greater ();
        }
    };

    template <typename atom> struct eval_action<parse::bool_equal_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.bool_equal ();
        }
    };

    template <typename atom> struct eval_action<parse::bool_unequal_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.bool_unequal ();
        }
    };

    template <typename atom> struct eval_action<parse::bool_and_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.boolean_and ();
        }
    };

    template <typename atom> struct eval_action<parse::bool_or_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.boolean_or ();
        }
    };

    template <typename atom> struct eval_action<parse::equal_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.equal ();
        }
    };

    template <typename atom> struct eval_action<parse::unequal_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.unequal ();
        }
    };

    template <typename atom> struct eval_action<parse::intuitionistic_and_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.intuitionistic_and ();
        }
    };

    template <typename atom> struct eval_action<parse::intuitionistic_or_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.intuitionistic_or ();
        }
    };

    template <typename atom> struct eval_action<parse::intuitionistic_implies_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.intuitionistic_implies ();
        }
    };

    template <typename atom> struct eval_action<parse::element_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.element ();
        }
    };

    template <typename atom> struct eval_action<parse::such_that_op<atom>> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.such_that ();
        }
    };

    template <> struct eval_action<parse::let_open> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.back_statements <<= eval.statements;
            eval.statements = {};
        }
    };

    template <> struct eval_action<parse::rule> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            // NOTE for some reason, "in" gets on the stack, which is why we are taking the 
            // third and second elements from the stack instead of the first and second. 
            // This is a kludge and should be repaired eventually. 
            eval.statements <<= statement {subject (eval.expr[2]), eval.expr[1]};
            eval.expr = data::drop (eval.expr, 3);
        }
    };

    template <> struct eval_action<parse::let> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.expr <<= make::let (data::first (eval.back_statements), data::first (eval.expr));
            eval.back_statements = data::rest (eval.back_statements);
        }
    };

    template <> struct eval_action<parse::set> {
        template <typename Input>
        static void apply (const Input &in, Parser &eval) {
            eval.statements <<= statement {subject (eval.expr[1]), eval.expr[0]};
            eval.expr = data::rest (data::rest (eval.expr));
        }
    };

    void Parser::read_line (const std::string &in) {
        if (!tao::pegtl::parse<parse::program, Diophant::eval_action> (tao::pegtl::memory_input<> {in, "expression"}, *this))
            throw exception {} << "could not parse line \"" << in << "\"";
    }

    Expression Parser::read_expression (const std::string &in) {
        if (!tao::pegtl::parse<parse::parse_expression, Diophant::eval_action> (tao::pegtl::memory_input<> {in, "expression"}, *this))
            throw exception {} << "could not parse expression \"" << in << "\"";

        if (!runnable () || statements.size () != 0) throw exception {} << "not a valid program";

        auto result = first (expr);
        expr = {};
        return result;
    }

    void Parser::initialize () {
        /*
        read_line (R"(`=` ? (@ a -> a = a) & (@ a b -> a = b => b = a) & (@ a b c -> a = b & b = c => a = c); ");
        read_line (R"(Natural _N _S -> 
            /< zero is a natural number >/
            0 : N & 
            /< the successor of a natural number is a natural number. >/
            (@ x -> x : N <==> (S n) : N) & 
            /< in peano's original version, there were 3 axioms about equality
               that are automatically handled by the = sign already. >/
            /< natural numbers are closed under equality. >/
            (@ x y -> x : N => x == y => y : N)
            /< successors of equal natural numbers are equal. >/
            (@ x y -> x : N => y : N => x == y <==> S x == S y) & 
            /< 0 is the successor of no natural number. >/
            (@ x -> S x == 0 => Impossible) & 
            /< induction >/
            (@ P n -> replace P n 0 => (P => replace P n (S n)) => @ x -> replace P n x); )");

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
        read_line (R"(rest ? rest : Stack x => Stack x; )");*/
        
        // TODO fill in the missing functions

    }
    
    void inline Parser::read_symbol (const data::string &in) {
        if (reading_lambda_vars) vars <<= expressions::symbol {in};
        else expr <<= make::symbol (in);
    }
    
    void inline Parser::any () {
        expr <<= make::any ();
    }
    
    void inline Parser::var () {
        expr = prepend (rest (expr), make::var (dynamic_cast<const expressions::symbol &> (*first (expr).get ())));
    }

    void inline Parser::read_string (const data::string &in) {
        expr <<= make::string (in);
    }

    void inline Parser::read_number (const data::string &in) {
        expr <<= make::natural (N {in});
    }

    void inline Parser::negate () {
        expr = prepend (rest (expr), make::negate (first (expr)));
    }

    void inline Parser::boolean_not () {
        expr = prepend (rest (expr), make::boolean_not (first (expr)));
    }

    void inline Parser::mul () {
        expr = prepend (rest (rest (expr)), make::times (first (rest (expr)), first (expr)));
    }

    void inline Parser::pow () {
        expr = prepend (rest (rest (expr)), make::power (first (rest (expr)), first (expr)));
    }

    void inline Parser::div () {
        expr = prepend (rest (rest (expr)), make::divide (first (rest (expr)), first (expr)));
    }

    void inline Parser::div_mod () {
        expr = prepend (rest (rest (expr)), make::div_mod (first (rest (expr)), first (expr)));
    }

    void inline Parser::plus () {
        expr = prepend (rest (rest (expr)), make::plus (first (rest (expr)), first (expr)));
    }

    void inline Parser::minus () {
        expr = prepend (rest (rest (expr)), make::minus (first (rest (expr)), first (expr)));
    }

    void inline Parser::bool_equal () {
        expr = prepend (rest (rest (expr)), make::bool_equal (first (rest (expr)), first (expr)));
    }

    void inline Parser::bool_unequal () {
        expr = prepend (rest (rest (expr)), make::bool_unequal (first (rest (expr)), first (expr)));
    }

    void inline Parser::equal () {
        expr = prepend (rest (rest (expr)), make::equal (first (rest (expr)), first (expr)));
    }

    void inline Parser::unequal () {
        expr = prepend (rest (rest (expr)), make::unequal (first (rest (expr)), first (expr)));
    }

    void inline Parser::greater_equal () {
        expr = prepend (rest (rest (expr)), make::greater_equal (first (rest (expr)), first (expr)));
    }

    void inline Parser::less_equal () {
        expr = prepend (rest (rest (expr)), make::less_equal (first (rest (expr)), first (expr)));
    }

    void inline Parser::greater () {
        expr = prepend (rest (rest (expr)), make::greater (first (rest (expr)), first (expr)));
    }

    void inline Parser::less () {
        expr = prepend (rest (rest (expr)), make::less (first (rest (expr)), first (expr)));
    }

    void inline Parser::boolean_and () {
        expr = prepend (rest (rest (expr)), make::boolean_and (first (rest (expr)), first (expr)));
    }

    void inline Parser::boolean_or () {
        expr = prepend (rest (rest (expr)), make::boolean_or (first (rest (expr)), first (expr)));
    }

    void inline Parser::intuitionistic_and () {
        expr = prepend (rest (rest (expr)), make::intuitionistic_and (first (rest (expr)), first (expr)));
    }

    void inline Parser::intuitionistic_or () {
        expr = prepend (rest (rest (expr)), make::intuitionistic_or (first (rest (expr)), first (expr)));
    }

    void inline Parser::intuitionistic_implies () {
        expr = prepend (rest (rest (expr)), make::intuitionistic_implies (first (rest (expr)), first (expr)));
    }

    void inline Parser::such_that () {
        expr = prepend (rest (rest (expr)), make::such_that (first (rest (expr)), first (expr)));
    }

    void inline Parser::element () {
        expr = prepend (rest (rest (expr)), make::element (first (rest (expr)), first (expr)));
    }

    void inline Parser::call () {
        expr = prepend (rest (rest (expr)), make::call (first (rest (expr)), first (expr)));
    }

    void inline Parser::start_vars () {
        reading_lambda_vars = true;
    }

    void inline Parser::end_vars () {
        reading_lambda_vars = false;
    }

    void Parser::lambda () {
        expr = prepend (rest (expr), make::lambda (vars, first (expr)));
        vars = {};
    }

    void Parser::open_list () {
        back_expr <<= expr;
        expr = data::stack<Expression> {};
    }

    void Parser::open_object () {
        back_expr <<= expr;
        expr = data::stack<Expression> {};
    }

    void Parser::close_list () {
        auto lz = make::list (reverse (expr));
        expr = data::prepend (first (back_expr), lz);
        back_expr = rest (back_expr);
    }

    void Parser::close_object () {
        data::stack<entry<Expression, Expression>> m;

        while (data::size (expr) > 0) {
            m <<= entry<Expression, Expression> {expr[1], expr[0]};
            expr = rest (rest (expr));
        }

        expr = data::prepend (first (back_expr), make::map (m));
        back_expr = rest (back_expr);
    }
    
    void Parser::unary (char op) {
        expr = prepend (rest (expr), 
            expressions::left_unary_expression::make (expressions::left_unary_operand {op}, first (expr)));
    }
    
    void Parser::set () {
        auto &st = first (statements);
        machine.define (st.subject, st.predicate);
        statements = rest (statements);
    }
    
    void Parser::dif () {
        // NOTE another kludge here.
        expr = prepend (rest (rest (rest (expr))), make::dif (expr[4], expr[2], expr[0]));
    }

    bool Parser::runnable () const {
        // NOTE the commented clause ought to work but it doesn't due to extra items being pushed on the stack.
        return back_expr.size () == 0 && back_statements.size () == 0 /*&& expr.size () == 1*/ && !reading_lambda_vars;
    }

    Expression Parser::run () {
        if (!runnable ()) throw exception {} << "not in a valid state to run program";

        statements = data::reverse (statements);
        while (statements.size () > 0) set ();

        Expression result = evaluate (first (expr), machine);
        expr = {};

        return result;
    }
}
