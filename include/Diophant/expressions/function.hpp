#ifndef DIOPHANT_EXPRESSIONS_FUNCTION
#define DIOPHANT_EXPRESSIONS_FUNCTION

#include <Diophant/expressions/expressions.hpp>
#include <Diophant/expression.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/replace.hpp>
#include <Diophant/type.hpp>

namespace Diophant::expressions {
    
    template <typename fun> signature {
        type operator * ();
    };
    
    template <> signature<void> {
        type operator * ();
    };
    
    template <> signature<bool> {
        type operator * ();
    };
    
    template <> signature<double> {
        type operator * ();
    };
    
    template <> signature<data::N> {
        type operator * ();
    };
    
    template <> signature<data::string> {
        type operator * ();
    };
    
    template <uint32 u> signature<data::array<N, u>> {
        type operator * ();
    };
    
    template <typename t> signature<data::stack<t>> {
        type operator * ();
    };
    
    template <typename... t> signature<std::tuple<t...>> {
        type operator * ();
    };
    
    template <typename r, typename v, typename b, typename... t> signature<r (v, b, t...)> {
        type operator * () {
            return make::intuitionistic_implies (*signature<v> {}, *signature<r, (b, t...)> {});
        }
    };
    
    template <typename r, typename v> signature<r (v)> {
        type operator * () {
            return make::intuitionistic_implies (*signature<v> {}, *signature<r> {} {});
        }
    };
    
    struct function final : abstract {
        Type Signature;
        data::function<void *(void *[])> Fun;
        
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
