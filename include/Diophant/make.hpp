#ifndef DIOPHANT_MAKE
#define DIOPHANT_MAKE

#include <Diophant/data.hpp>
#include <Diophant/symbol.hpp>
#include <Diophant/pattern.hpp>
#include <Diophant/statement.hpp>

namespace Diophant::make {

    // basic values
    Expression null ();
    Expression boolean (bool b);
    Expression natural (const data::N &q);
    Expression integer (const data::Z &q);
    Expression rational (const data::Q &q);
    Expression symbol (const std::string &x);
    Expression string (const data::string &str);
    Expression list (data::stack<Expression> ls);
    Expression map (data::stack<data::entry<Expression, Expression>> x);
    Expression lambda (stack<Symbol>, Expression &);

    Expression call (Expression &, data::list<Expression>);
    Expression let (data::stack<statement>, Expression);
    Expression dif (Expression, Expression, Expression);

    Expression negate (Expression &);
    Expression plus (Expression &);
    Expression plus (Expression &, Expression &);
    Expression minus (Expression &, Expression &);
    Expression times (Expression &, Expression &);
    Expression power (Expression &, Expression &);
    Expression divide (Expression &, Expression &);
    Expression mod (Expression &, Expression &);
    Expression div_mod (Expression &, Expression &);

    Expression equal (Expression &, Expression &);
    Expression unequal (Expression &, Expression &);
    Expression greater_equal (Expression &, Expression &);
    Expression less_equal (Expression &, Expression &);
    Expression greater (Expression &, Expression &);
    Expression less (Expression &, Expression &);

    Expression bool_equal (Expression &, Expression &);
    Expression bool_unequal (Expression &, Expression &);

    Expression boolean_not (Expression &);
    Expression boolean_and (Expression &, Expression &);
    Expression boolean_or (Expression &, Expression &);

    Expression equal (Expression &, Expression &);
    Expression unequal (Expression &, Expression &);

    Expression intuitionistic_and (Expression &, Expression &);
    Expression intuitionistic_or (Expression &, Expression &);
    Expression intuitionistic_implies (Expression &, Expression &);
    
    template <typename Return, typename... Args>
    Expression built_in_function (Return (*f) (Args...));

}

#endif

