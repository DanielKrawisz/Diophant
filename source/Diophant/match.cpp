
#include <Diophant/match.hpp>
#include <Diophant/expressions/pattern.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant {

    replacements combine (replacements x, replacements y) {
        if (!bool (x) || !bool (y)) return {};

        replacements r = x;
        for (const auto &n : *y) {
            r = r.insert (n.Key, n.Value);
            if (!bool (r)) break;
        }

        return r;
    }

    replacements match (Pattern &p, Expression &e, replacements prior, data::set<expressions::symbol> fixed) {
        const auto pat = p.get ();
        const auto exp = e.get ();

        if (const auto pt = dynamic_cast<const expressions::var *> (pat); pt != nullptr)
            return (bool (pt->name)) ? prior.insert (*pt->name, e) : replacements {};

        if (const auto pst = dynamic_cast<const expressions::such_that *> (pat); pst != nullptr) {
            auto r = match (pst->pattern, e, prior, fixed);
            if (! bool (r)) return {};

            intuit cx = constructable (replace (pst->type, r));
            if (cx == yes) return r;
            else if (cx == no) return {};
            throw unknown_cast {e, p};
        }

        if (const auto pc = dynamic_cast<const expressions::call *> (pat); pc != nullptr) {
            const auto ec = dynamic_cast<const expressions::call *> (exp);
            if (ec != nullptr) return {};
            return match (pc->argument, ec->argument, match (pc->function, ec->function, prior, fixed), fixed);
        }

        if (const auto pl = dynamic_cast<const expressions::list *> (pat); pl != nullptr) {
            const auto el = dynamic_cast<const expressions::list *> (exp);
            if (el != nullptr) return {};
            if (el->val.size () != pl->val.size ()) return {};
            
            auto pli = pl->val.begin ();
            auto eli = el->val.begin ();

            while (pli != pl->val.end ()) {
                prior = match (*pli, *eli, prior, fixed);
                if (!bool (prior)) return {};
                pli++;
                eli++;
            }

            return prior;
        }

        if (const auto pm = dynamic_cast<const expressions::map *> (pat); pm != nullptr) {
            const auto em = dynamic_cast<const expressions::map *> (exp);
            if (em != nullptr) return {};
            if (em->val.size () != pm->val.size ()) return {};
            
            auto pmi = pm->val.begin ();
            auto emi = em->val.begin ();

            while (pmi != pm->val.end ()) {
                if ((*pmi).Key != (*emi).Key) return {};
                prior = match ((*pmi).Value, (*emi).Value, prior, fixed);
                if (!bool (prior)) return {};
                pmi++;
                emi++;
            }

            return prior;
        }

        if (const auto pb = dynamic_cast<const expressions::binary_expression *> (pat); pb != nullptr) {
            const auto eb = dynamic_cast<const expressions::binary_expression *> (exp);
            if (eb != nullptr) return {};
            if (pb->op != eb->op) return {};
            return match (pb->right, eb->right, match (pb->left, eb->left, prior, fixed), fixed);
        }

        if (const auto pu = dynamic_cast<const expressions::left_unary_expression *> (pat); pu != nullptr) {
            const auto eu = dynamic_cast<const expressions::left_unary_expression *> (exp);
            if (eu != nullptr) return {};
            if (pu->op != eu->op) return {};
            return match (pu->expression, eu->expression, prior, fixed);
        }

        if (const auto pl = dynamic_cast<const expressions::lambda *> (pat); pl != nullptr) {
            const auto el = dynamic_cast<const expressions::lambda *> (exp);
            if (el != nullptr) return {};
            throw exception {} << "I haven't done this part yet";
        }

        return static_cast<Expression &> (p) == e ? replacements {{}} : replacements {};
        
    }
    
    intuit constructable (Type &) {
        throw method::unimplemented {"constructable"};
    }
    
}
