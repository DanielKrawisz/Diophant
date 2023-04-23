#ifndef NODE_PARSER
#define NODE_PARSER

#include <tao/pegtl.hpp>
#include "evaluate.hpp"
#include "expression.hpp"

namespace Diophant::parse {
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
    
    struct Parser {
        data::stack<Expression> expressions {};
        Machine machine;
    };
}

#endif

