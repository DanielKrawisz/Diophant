#ifndef DIOPHANT_VALUE
#define DIOPHANT_VALUE

#include <Diophant/symbol.hpp>

namespace Diophant {
    
    struct function;
    
    struct value : ptr<either<bool, std::string, data::N, stack<value>, map<value, value>, function> {};
    using Value = const value;
    
    bool operator == (Value, Value);

    std::ostream &operator << (std::ostream &, Value);
    std::istream &operator >>(std::istream &, value &);
    
    struct value {
        
        static Value read (std::string &);
        bool valid () const;
        
        static Value null ();
        static Value boolean (bool);
        static Value natural (const data::N &);
        static Value string (const std::string &);
    };
}

#endif
