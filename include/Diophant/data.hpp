#ifndef DIOPHANT_DATA
#define DIOPHANT_DATA

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include <data/math.hpp>
#include <data/either.hpp>

namespace Diophant {
    using namespace data;

    struct expression;
    using Expression = const expression;

    bool operator == (Expression &, Expression &);

    std::ostream &operator << (std::ostream &, Expression &);
    std::istream &operator >> (std::istream &, expression &);

    struct type;
    using Type = const type;

    std::ostream &operator << (std::ostream &, Type &);

    enum intuit : char {
        unknown = 0,
        yes = 1,
        no = -1
    };

    intuit operator ! (intuit);
    intuit operator && (intuit, intuit);
    intuit operator || (intuit, intuit);
    
    struct intuitionistic_partial_ordering {
        intuit disjoint;
        intuit left_castable;
        intuit right_castable;
        
        intuit equal () const {
            return right_castable && left_castable;
        }
        
        intuit sub () const {
            return left_castable && !right_castable;
        }
        
        intuit super () const {
            return right_castable && !left_castable;
        }
        
        intuitionistic_partial_ordering (intuit d, intuit l, intuit r): disjoint {d}, left_castable {l}, right_castable {r} {}
        
        intuitionistic_partial_ordering (std::partial_ordering p) {
            if (p == std::partial_ordering::unordered) {
                *this = {yes, no, no};
                return;
            } else if (p == std::partial_ordering::greater) {
                *this = {no, yes, no};
                return;
            } else if (p == std::partial_ordering::less) {
                *this = {no, no, yes};
                return;
            } else if (p == std::partial_ordering::equivalent) 
                *this = {no, yes, yes};
        }
    };

    // thrown in the case of a cast that we don't know how to make.
    struct unknown_operation : exception {
        using exception::exception;
    };

    intuit inline operator ! (intuit x) {
        return intuit (-char (x));
    }
    
    intuit inline operator && (intuit a, intuit b) {
        return std::max (a, b);
    }
    
    intuit inline operator || (intuit a, intuit b) {
        return std::min (a, b);
    }

}

#endif
