#ifndef DIOPHANT_EXPRESSIONS_FUNCTION
#define DIOPHANT_EXPRESSIONS_FUNCTION

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/replace.hpp>

namespace Diophant::expressions {
    
    struct function final : abstract {
        enum types : char {
            Null = 0, 
            Bool = 1, 
            N = 2, 
            Z = 3, 
            Q = 4,
            Float = 5,
            String = 6,
            List = 8
        };
        
        cross<types> Signature;
        function<void *(void *[])> Function;
        
        bool operator == (const abstract &) const final override;
        
        std::ostream &write (std::ostream &o) const override {
            auto b = Signature.begin ();
            write (o, *b) << "(";
            b++;
            if (b != Signature.end ())
                while (true) {
                    write (o, *b);
                    b++;
                    if (b == Signature.end ()) break;
                    o << ", ";
                }
            return o << ")";
        }
        
        static std::ostream &write (std::ostream &o, types t) {
            switch (t & 7) {
                case (Null): {
                    o << "Null";
                } break;
                case (Bool): {
                    o << "Bool";
                } break;
                case (N): {
                    o << "N";
                } break;
                case (Z): {
                    o << "Z";
                } break;
                case (Q): {
                    o << "Q";
                } break;
                case (Float): {
                    o << "Float";
                } break;
                case (String): {
                    o << "String";
                } break;
                default: throw exception {} << "invalid type";
            }
            
            return t & 8 ? o << "[] " : o << " *";
        }
    };
        
    bool inline function::operator == (const abstract &x) const {
        return static_cast<const abstract *> (this) == &x;
    }
    
}

#endif
