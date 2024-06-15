#ifndef DIOPHANT_EXPRESSIONS_VALUES
#define DIOPHANT_EXPRESSIONS_VALUES

#include <Diophant/replace.hpp>
#include <Diophant/expression.hpp>

namespace Diophant::expressions {
    
    template <typename X>
    struct value final : abstract {
        X val;
        value (const X &x) : val {x} {}
        
        static Expression make (const X &);
        std::ostream &write (std::ostream &) const override;

        bool operator == (const abstract &) const override;
        
    };

    using boolean = value<bool>;
    using natural = value<data::N>;
    using integer = value<data::Z>;
    using rational = value<data::Q>;
    using dfloat = value<double>;
    using string = value<data::string>;
    using list = value<data::cross<expression>>;
    using map = value<data::map<Expression, Expression>>;
}

namespace Diophant::make {
    
    Expression inline boolean (bool b) {
        return expressions::boolean::make (b);
    }

    Expression inline natural (const data::N &n) {
        return expressions::natural::make (n);
    }

    Expression inline integer (const data::Z &n) {
        return expressions::integer::make (n);
    }
    
    Expression inline rational (const data::Q &n) {
        return expressions::rational::make (n);
    }
    
    Expression inline string (const data::string &x) {
        return expressions::string::make (x);
    }
    
    Expression inline list (data::stack<Expression> z) {
        return expressions::list::make (cross<expression> (z));
    };
    
    Expression inline map (data::stack<entry<Expression, Expression>> z) {
        data::map<Expression, Expression> m;
        for (const auto &e : z) m = m.insert (e);
        return expressions::map::make (m);
    };
    
}

namespace Diophant::expressions {
    template <typename X>
    Expression inline value<X>::make (const X &x) {
        return Diophant::expression {std::static_pointer_cast<const abstract> (std::make_shared<value<X>> (x))};
    }
    
    std::ostream inline &write (std::ostream &o, bool b) {
        return o << '<' << std::boolalpha << b << '>';
    }
    
    std::ostream inline &write (std::ostream &o, const data::Q &q) {
        return o << q;
    }
    
    std::ostream inline &write (std::ostream &o, const data::string &x) {
        return o << x;
    }
    
    std::ostream inline &write (std::ostream &o, const data::N &x) {
        return o << x;
    }

    template <typename X>
    std::ostream inline &write_iterable (std::ostream &o, const X &x) {
        auto i = x.begin ();
        if (i == x.end ()) return o;
        o << *i;
        while (true) {
            i++;
            if (i == x.end ()) return o;
            o << ", " << *i;
        }
    }
    
    std::ostream inline &write (std::ostream &o, const data::cross<expression> &x) {
        return write_iterable (o << "[", x) << "]";
    }

    std::ostream inline &write (std::ostream &o, data::map<Expression, Expression> x) {
        return write_iterable (o << "{", x) << "}";
    }

    template <typename X>
    std::ostream inline &value<X>::write (std::ostream &o) const {
        return expressions::write (o, val);
    }

    template <typename X>
    bool inline value<X>::operator == (const abstract &a) const {
        try {
            return val == dynamic_cast<const value<X> &> (a).val;
        } catch (std::bad_cast) {
            return false;
        }
    }
}

namespace Diophant {

    data::N inline plus_N (const data::N &a, const data::N &b) {
        return a + b;
    }

    data::N inline times_N (const data::N &a, const data::N &b) {
        return a * b;
    }

    data::Z inline plus_Z (const data::Z &a, const data::Z &b) {
        return a + b;
    }

    data::Z inline minus_Z (const data::Z &a, const data::Z &b) {
        return a - b;
    }

    data::Z inline times_Z (const data::Z &a, const data::Z &b) {
        return a * b;
    }

    bool inline eq_N (const data::N &a, const data::N &b) {
        return a == b;
    }

    bool inline neq_N (const data::N &a, const data::N &b) {
        return a != b;
    }

    bool inline leq_N (const data::N &a, const data::N &b) {
        return a <= b;
    }

    bool inline geq_N (const data::N &a, const data::N &b) {
        return a >= b;
    }

    bool inline less_N (const data::N &a, const data::N &b) {
        return a < b;
    }

    bool inline greater_N (const data::N &a, const data::N &b) {
        return a > b;
    }

    bool inline eq_Z (const data::Z &a, const data::Z &b) {
        return a == b;
    }

    bool inline neq_Z (const data::Z &a, const data::Z &b) {
        return a != b;
    }

    bool inline leq_Z (const data::Z &a, const data::Z &b) {
        return a <= b;
    }

    bool inline geq_Z (const data::Z &a, const data::Z &b) {
        return a >= b;
    }

    bool inline less_Z (const data::Z &a, const data::Z &b) {
        return a < b;
    }

    bool inline greater_Z (const data::Z &a, const data::Z &b) {
        return a > b;
    }

}

#endif
