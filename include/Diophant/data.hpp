#ifndef DIOPHANT_DATA
#define DIOPHANT_DATA

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include <data/math.hpp>

namespace Diophant {
    using namespace data;

    struct expression;
    using Expression = const expression;

    bool operator == (Expression &, Expression &);

    std::ostream &operator << (std::ostream &, Expression &);
    std::istream &operator >> (std::istream &, expression &);

    struct type;
    using Type = const type;

    bool operator == (Type &, Type &);

    std::ostream &operator << (std::ostream &, Type &);

    enum intuit : char {
        no = 0,
        yes = 1,
        unknown = -1
    };

    intuit operator && (intuit, intuit);
    intuit operator || (intuit, intuit);

    // thrown in the case of a cast that we don't know how to make.
    struct unknown_operation : exception {};

}

#endif
