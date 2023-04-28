#include<Diophant/expressions/values.hpp>

namespace Diophant::expressions {
    
    std::ostream &write (std::ostream &o, data::stack<Expression> val) {
        o << "[";

        if (!data::empty (val)) {
            first (val)->write (o);
            for (const auto v : rest (val)) v->write (o << ", ");
        }

        return o << "]";
    }
    
    std::ostream &write (std::ostream &o, data::stack<entry<std::string, Expression>> val) {
        o << "{";

        if (!data::empty (val)) {
            auto e = first (val);
            e.Value->write (o << e.Key << ": ");
            for (const auto e : rest (val)) e.Value->write (o << ", " << e.Key << ": ");
        }

        return o << "}";
    }
    
}
