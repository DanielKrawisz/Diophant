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

    template <typename T>
    struct to {
        const T &operator () (Expression &x) const {
            return static_cast<const value<T> *> (x.get ())->val;
        }
    };

    template <typename T>
    struct to<const T &> {
        const T &operator () (Expression &x) const {
            return static_cast<const value<T> *> (x.get ())->val;
        }
    };

    template <typename T>
    struct from {
        Expression &operator () (const T &) const;
    };

    template <typename T> struct function_call;

    template <typename T> T take_first (data::list<T> &l) {
        auto L = l;
        l = data::rest (l);
        return data::first (L);
    }

    template <typename Return, typename Arg1>
    struct function_call<Return (Arg1)> {
        Expression operator () (Return (*f) (Arg1), data::list<Expression> l) {
            return from<Return> {} (f (to<Arg1> {} (take_first (l))));
        }
    };

    template <typename Return, typename Arg1, typename Arg2>
    struct function_call<Return (Arg1, Arg2)> {
        Expression operator () (Return (*f) (Arg1, Arg2), data::list<Expression> l) {
            return from<Return> {} (f (to<Arg1> {} (take_first (l)), to<Arg2> {} (take_first (l))));
        }
    };

    template <typename Return, typename Arg1, typename Arg2, typename Arg3>
    struct function_call<Return (Arg1, Arg2, Arg3)> {
        Expression operator () (Return (*f) (Arg1, Arg2, Arg3), data::list<Expression> l) {
            return from<Return> {} (f (to<Arg1> {} (take_first (l)), to<Arg2> {} (take_first (l)), to<Arg3> {} (take_first (l))));
        }
    };

    template <typename Return, typename... Args>
    struct built_in<Return (Args...)> final : function {
        Return (*Fun) (Args...);

        Type signature () const override {
            return *expressions::signature<Return (Args...)> {};
        }

        Expression operator () (data::list<Expression> l) const override {
            return function_call<Return (Args...)> {} (Fun, l);
        }

        bool operator == (const abstract &a) const final override {
            try {
                return Fun == dynamic_cast<const built_in &> (a).Fun;
            } catch (std::bad_cast) {
                return false;
            }
        }

        built_in (Return (*f) (Args...)) : Fun {f} {}
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
