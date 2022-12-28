#ifndef DIOPHANT_TYPES
#define DIOPHANT_TYPES

#include <data/expression.hpp>
#include <data/math.hpp>
#include <data/numbers.hpp>

namespace Diophant {
    template <typename X> using ptr = data::ptr<X>;
    template <typename X> using optional = std::optional<X>;
    template <typename... X> using variant = std::variant<X...>;
    template <typename... X> using proposition = std::function<bool(X...)>;
    using data::uint32;
}

#endif
