#include <Diophant/value.hpp>
#include <Diophant/parser.hpp>

namespace Diophant {

    bool valid_value (Expression &e, data::set<expressions::symbol> free = {}) {
        const auto *x = e.get ();
        
        if (x == nullptr) return true;
        if (auto b = dynamic_cast<const expressions::boolean *> (x); b != nullptr) return true;

        if (auto b = dynamic_cast<const expressions::natural *> (x); b != nullptr) return true;

        if (auto b = dynamic_cast<const expressions::integer *> (x); b != nullptr) return true;
        if (auto b = dynamic_cast<const expressions::rational *> (x); b != nullptr) return true;
        if (auto b = dynamic_cast<const expressions::string *> (x); b != nullptr) return true;
        
        if (auto b = dynamic_cast<const expressions::list *> (x); b != nullptr) {
            for (Expression &n : b->val) if (!valid_value (n)) return false;
            return true;
        }
        
        if (auto b = dynamic_cast<const expressions::map *> (x); b != nullptr) {
            for (const entry<Expression, Expression> &n : b->val) if (!valid_value (n.Key) && valid_value (n.Value)) return false;
            return true;
        }
        
        if (auto b = dynamic_cast<const expressions::lambda *> (x); b != nullptr) {
            for (auto x : b->arguments) free = free.insert (x);
            return valid_value (b->body, free);
        }
        
        if (auto b = dynamic_cast<const expressions::symbol *> (x); b != nullptr)
            return free.contains (*b);

        return false;
        
    }
    
    bool value::valid () const {
        return valid_value (*this);
    }
    
    value run (const string &x) {
        Parser p {};
        initialize (p.machine);
        p.read_line (x);
        auto expr = p.run ();
        value result {expr};
        if (result.valid ()) return result;
        throw exception {} << "invalid return value: " << expr;
    }
    
}
