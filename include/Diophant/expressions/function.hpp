#ifndef DIOPHANT_EXPRESSIONS_FUNCTION
#define DIOPHANT_EXPRESSIONS_FUNCTION

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/type.hpp>

namespace Diophant::expressions {

    struct function : abstract {
        virtual Type signature () const = 0;
        virtual Expression operator () (data::list<Expression>) const = 0;

        std::ostream &write (std::ostream &o) const override {
            return o << '<' << signature () << '>';
        }

        uint32 number_of_args () const;
    };

    template <typename func> struct built_in;

    template <typename T> struct signature;

    template <typename Return, typename Arg> struct signature<Return (Arg)> {
        Type operator * () const;
    };

    template <typename Return, typename Arg1, typename Arg2, typename... Args> struct signature<Return (Arg1, Arg2, Args...)> {
        Type operator * () const;
    };

    template <typename Return, typename... Args>
    struct built_in<Return (Args...)> final : function {
        Type signature () const override {
            return *expressions::signature<Return (Args...)> {};
        }

        Expression operator () (data::list<Expression>) const override;
        bool operator == (const abstract &) const final override;

        built_in (Return (*f) (Args...));
    };
    
}

namespace Diophant::make {
    template <typename Return, typename... Args>
    Expression built_in_function (Return (*f) (Args...)) {
        return Diophant::expression
            {std::static_pointer_cast<const expressions::abstract> (std::make_shared<expressions::built_in<Return (Args...)>> (f))};
    }
}

#endif
