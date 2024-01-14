
#include <Diophant/match.hpp>
#include <Diophant/expressions/pattern.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/lambda.hpp>

namespace Diophant {

    maybe<replacements> combine (maybe<replacements> x, maybe<replacements> y) {
        if (!x || !y) return {};

        replacements r = *x;
        for (const auto &n : *y) {
            auto v = r.contains (n.Key);
            if (!v) r = r.insert (n);
            else if (v && *v != n.Value) return {};
        }

        return r;
    }

    maybe<replacements> match (Pattern &p, Expression &e, data::set<expressions::symbol> fixed) {
        const auto pat = p.get ();
        const auto exp = e.get ();

        if (const auto pt = dynamic_cast<const expressions::typed *> (pat); pt != nullptr)
            return type_of (e) <= pt->type ? maybe<replacements> {{{pt->var, e}}} : maybe<replacements> {};

        if (const auto pc = dynamic_cast<const expressions::call *> (pat); pc != nullptr) {
            const auto ec = dynamic_cast<const expressions::call *> (exp);
            if (ec != nullptr) return {};
            return combine (match (pc->function, ec->function, fixed), match (pc->argument, ec->argument, fixed));
        }

        if (const auto pl = dynamic_cast<const expressions::list *> (pat); pl != nullptr) {
            const auto el = dynamic_cast<const expressions::list *> (exp);
            if (el != nullptr) return {};
            if (el->val.size () != pl->val.size ()) return {};
            replacements r;
            auto pli = pl->val.begin ();
            auto eli = el->val.begin ();

            while (pli != pl->val.end ()) {
                auto rr = combine (r, match (*pli, *eli, fixed));
                if (!rr) return {};
                r = *rr;
                pli++;
                eli++;
            }

            return r;
        }

        if (const auto pm = dynamic_cast<const expressions::map *> (pat); pm != nullptr) {
            const auto em = dynamic_cast<const expressions::map *> (exp);
            if (em != nullptr) return {};
            if (em->val.size () != pm->val.size ()) return {};
            replacements r;
            auto pmi = pm->val.begin ();
            auto emi = em->val.begin ();

            while (pmi != pm->val.end ()) {
                if ((*pmi).Key != (*emi).Key) return {};
                auto rr = combine (r, match ((*pmi).Value, (*emi).Value, fixed));
                if (!rr) return {};
                r = *rr;
                pmi++;
                emi++;
            }

            return r;
        }

        if (const auto pb = dynamic_cast<const expressions::binary_expression *> (pat); pb != nullptr) {
            const auto eb = dynamic_cast<const expressions::binary_expression *> (exp);
            if (eb != nullptr) return {};
            if (pb->op != eb->op) return {};
            return combine (match (pb->left, eb->left, fixed), match (pb->right, eb->right, fixed));
        }

        if (const auto pu = dynamic_cast<const expressions::unary_expression *> (pat); pu != nullptr) {
            const auto eu = dynamic_cast<const expressions::unary_expression *> (exp);
            if (eu != nullptr) return {};
            if (pu->op != eu->op) return {};
            return match (pu->expression, eu->expression, fixed);
        }

        if (const auto pl = dynamic_cast<const expressions::lambda *> (pat); pl != nullptr) {
            const auto el = dynamic_cast<const expressions::lambda *> (exp);
            if (el != nullptr) return {};
            throw exception {} << "I haven't done this part yet";
        }

        return static_cast<Expression &> (p) == e ? maybe<replacements> {replacements {}} : maybe<replacements> {};
        
    }
    
}
