
#include <Diophant/type.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/values.hpp>

namespace Diophant {
    
    intuit cast (Type t, Expression expr) {
        const auto tp = t.get ();
        const auto ep = t.get ();
        
        if (const auto xtp = dynamic_cast<const expressions::symbol *> (tp); xtp != nullptr) {
            if (xtp->name == "Value") {
                if (ep == nullptr || 
                    dynamic_cast<const expressions::boolean *> (ep) != nullptr || 
                    dynamic_cast<const expressions::natural *> (ep) != nullptr || 
                    dynamic_cast<const expressions::string *> (ep) != nullptr) return yes;
                
                if (const auto lep = dynamic_cast<const expressions::list *> (ep); lep != nullptr) {
                    intuit return_value = yes;
                    
                    for (Expression &ex : lep->val) {
                        auto castable = cast (t, ex);
                        if (castable == no) return no;
                        if (castable == unknown) return_value = unknown;
                    }
                        
                    return return_value;
                }
                
                // we don't even have a struct yet but when we do, check if all elements are values.
            }
            
            if (ep == nullptr) return xtp->name == "Null" ? yes : no;
            
            if (const auto bep = dynamic_cast<const expressions::boolean *> (ep); bep != nullptr) 
                return xtp->name == "Bool" ? yes : no;
            
            if (const auto nep = dynamic_cast<const expressions::natural *> (ep); nep != nullptr) 
                return xtp->name == "N" ? yes : no;
            
            if (const auto sep = dynamic_cast<const expressions::string *> (ep); sep != nullptr) 
                return xtp->name == "String" ? yes : no;
            
            if (const auto lep = dynamic_cast<const expressions::list *> (ep); lep != nullptr) 
                return xtp->name == "String" ? yes : no;
            
            return no;
        }
        
        if (const auto ltp = dynamic_cast<const expressions::list *> (tp); ltp != nullptr) {
            const auto lep = dynamic_cast<const expressions::list *> (ep); 
            if (lep == nullptr) return no;
            if (lep->val.size () != ltp->val.size ()) return no;
            
            intuit return_value = yes;
            
            auto b = ltp->val.begin ();
            for (Expression &ex : lep->val) {
                auto castable = cast (type {*b}, ex);
                if (castable == no) return no;
                if (castable == unknown) return_value = unknown;
                b++;
            }
                        
            return return_value;
            
        }
        
        return unknown;
    }
    /*
    intuitionistic_partial_ordering compare (Type &a, Type &b) {
        // if they are equal, 
        
        // A | B is bigger than A 
        
        // A & B is smaller than A
        
        // A | B => C is bigger than A => C 
    }*/

}
