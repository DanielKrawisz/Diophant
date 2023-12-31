#include <Diophant/machine.hpp>
#include <Diophant/expressions/symbol.hpp>
#include <Diophant/expressions/apply.hpp>

namespace Diophant {
    maybe<replacements> expression::match (Expression &e, const Machine &m) const {
        const expressions::abstract *n = this->get ();
        
        const expressions::pattern *p = dynamic_cast<const expressions::pattern *> (n);
        
        if (p == nullptr) return *this == e ? maybe<replacements> {replacements {}} : maybe<replacements> {};

        return p->match (e, m);
    }

    expression::operator Pattern () const {
        const expressions::abstract *n = this->get ();
        
        const expressions::symbol *x = dynamic_cast<const expressions::symbol *> (n);
        
        if (x != nullptr) return pattern {*x};
        
        const expressions::apply *a = dynamic_cast<const expressions::apply *> (n);
        
        if (a == nullptr) return pattern {};
        
        Pattern p (a->function);
        
        if (!p.valid ()) return pattern {};
        
        return pattern {p.root, p.parameters << a->argument};
        
    }
}

