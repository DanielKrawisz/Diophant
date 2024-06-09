
#include <Diophant/type.hpp>
#include <Diophant/machine.hpp>
#include <Diophant/value.hpp>
#include <Diophant/make.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/pattern.hpp>

namespace Diophant {
    
    Type type::Z (const data::N &n) {
        return make::call ((*symbols ())["Z"], make::natural (n));
    }
    
    Type type::List (Type &t) {
        return make::call ((*symbols ())["List"], t);
    }
    
    Type type::Tuple (data::stack<Type> x) {
        return make::list (x);
    }
    
    Type type::Array (Type &t, uint32 size) {
        return make::call (make::call ((*symbols ())["Array"], t), make::natural (size));
    }
    
    intuit cast (Type t, Expression expr, const Machine &m) {
        const auto tp = t.get ();
        const auto ep = t.get ();
        
        if (const auto xtp = dynamic_cast<const expressions::symbol *> (tp); xtp != nullptr) {
            if (xtp->name == "Value") return value {expr}.valid () ? yes : no;
            
            if (ep == nullptr) return xtp->name == "Null" ? yes : no;
            
            if (const auto bep = dynamic_cast<const expressions::boolean *> (ep); bep != nullptr) 
                return xtp->name == "Bool" ? yes : no;
            
            if (const auto sep = dynamic_cast<const expressions::string *> (ep); sep != nullptr) 
                return xtp->name == "String" ? yes : no;
            
            if (const auto nep = dynamic_cast<const expressions::natural *> (ep); nep != nullptr) 
                return xtp->name == "N" ? yes : no;
            
            if (const auto fep = dynamic_cast<const expressions::dfloat *> (ep); fep != nullptr) 
                return xtp->name == "Float" ? yes : no;
            
            return no;
        }
        
        if (const auto ctp = dynamic_cast<const expressions::call *> (tp); ctp != nullptr) {
            
            if (const auto xxx = dynamic_cast<const expressions::symbol *> (ctp->function.get ())) {
                if (xxx->name == "List") {
                    if (const auto lep = dynamic_cast<const expressions::list *> (ep); lep != nullptr) {
                        intuit return_value = yes;
            
                        for (Expression &ex : lep->val) {
                            auto castable = cast (type {ctp->argument}, ex, m);
                            if (castable == no) return no;
                            if (castable == unknown) return_value = unknown;
                        }
                                    
                        return return_value;
                    }
            
                    return no;
                }
            }
            
            return no;
        }
         
        if (const auto ltp = dynamic_cast<const expressions::list *> (tp); ltp != nullptr) {   
            const auto lep = dynamic_cast<const expressions::list *> (ep); 
            if (lep == nullptr) return no;
            if (lep->val.size () != ltp->val.size ()) return no;
            
            intuit return_value = yes;
            
            auto b = ltp->val.begin ();
            for (Expression &ex : lep->val) {
                auto castable = cast (type {*b}, ex, m);
                if (castable == no) return no;
                if (castable == unknown) return_value = unknown;
                b++;
            }
                        
            return return_value;
            
        }
        
        if (const auto btp = dynamic_cast<const expressions::binary_expression *> (tp); btp != nullptr) { 
            if (btp->op == expressions::binary_operand::INTUITIONISTIC_OR) {
                intuit left_result = cast (btp->left, expr, m);
                if (left_result == yes) return yes;
                return left_result || cast (btp->right, expr, m);
            }
            
            if (btp->op == expressions::binary_operand::INTUITIONISTIC_AND) {
                intuit left_result = cast (btp->left, expr, m);
                if (left_result == no) return no;
                return left_result && cast (btp->right, expr, m);
            }
            
            if (btp->op == expressions::binary_operand::INTUITIONISTIC_IMPLIES) {
                
            }
        }
        
        if (const auto sttp = dynamic_cast<const expressions::such_that *> (tp); sttp != nullptr) { 
            // TODO
        }
        
        return no;
    }
    
    intuitionistic_partial_ordering compare (Type &a, Type &b) {
        // if they are the same expression we can provide an answer. 
        if (static_cast<Expression> (a) == static_cast<Expression> (b)) return {no, yes, yes};
        return {unknown, unknown, unknown};
        
        // A | B is bigger than A 
        
        // A & B is smaller than A
        
        // A | B => C is bigger than A => C 
    }

}
