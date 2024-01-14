
#include <Diophant/replace.hpp>
#include <Diophant/machine.hpp>
#include <Diophant/evaluate.hpp>
#include <Diophant/match.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/lambda.hpp>

namespace Diophant {

    std::partial_ordering operator <=> (const parameters &a, const parameters &b) {
        if (a.size () != b.size ()) return a.size () <=> b.size ();
        auto ai = a.begin ();
        auto bi = b.begin ();
        std::partial_ordering last = std::partial_ordering::equivalent;
        while (ai != a.end ()) {
            std::partial_ordering next = *ai <=> *bi;
            if (next == std::partial_ordering::unordered ||
                next == std::partial_ordering::less && last == std::partial_ordering::greater ||
                next == std::partial_ordering::greater && last == std::partial_ordering::less)
                return std::partial_ordering::unordered;
            last = next;
            ai++;
            bi++;
        }

        return last;
    }

    subject::subject (Symbol &x, stack<Expression> z) : subject {x} {
        parameters.resize (z.size ());

        for (auto &p : parameters) {
            p = pattern {z.first ()};
            z = z.rest ();
        }
    }

    std::ostream &operator << (std::ostream &o, const predicate &p) {
        o << ": " << p.type;
        if (p.expr) o << " = " << *p.expr;
        return o;
    }

    maybe<replacements> match (const parameters &p, stack<Expression> x) {
        if (p.size () != data::size (x)) return {};
        replacements r;
        int i = 0;
        for (Expression &e : x) {
            auto rr = combine (r, match (p[i], e));
            if (!rr) return {};
            r = *rr;
        }
        return {r};
    }

    // return the conflicting entry if there is one.
    // will not modify the overloads if there is a conflict.
    const entry<parameters, predicate> *insert (Machine::overloads &o, const entry<parameters, predicate> &e) {
        stack<entry<parameters, predicate> &> left;
        Machine::overloads right = o;

        while (data::size (right) > 0) {
            entry<parameters, predicate> &next = right.first ();

            // the key will be greater than the next key if it is a
            // bigger pattern or if it is more general than next.
            if (e.Key > next.Key) {
                left <<= next;
                right = right.rest ();
                continue;
            }

            // if an entry already exists, we are allowed to
            //  * enter a value if one is not already given
            //  * provide a more specific type than the one already given.
            if (e.Key == next.Key) {
                if (e.Value.type > next.Value.type) return &next;

                if (!bool (e.Value.expr)) {
                    // next = entry<Pattern, predicate> {e.Key, predicate {e.Value.type, next.Value.expr}};
                    next.Value.type = e.Value.type;
                    return nullptr;
                }

                if (!bool (next.Value.expr) || (bool (next.Value.expr) && *next.Value.expr == *e.Value.expr)) {
                    next.Value = e.Value;
                    return nullptr;
                }

                return &next;
            }

            // we know that e.Key >= next.Key is false. Therefore
            // you might expect e.Key < next.Key. However, this
            // is not necessarily true because patterns can be
            // the same size, unequal, and neither more or less
            // general than one another. In that case there is a
            // conflict because there is no way to know what order
            // the two conflicting rules ought to be applied.
            if (e.Key < next.Key) {
                right <<= e;
                while (left.size () > 0) {
                    right <<= left.first ();
                    left = left.rest ();
                }
                o = right;
                return nullptr;
            }

            return &next;
        }

        return nullptr;
    }

    struct definition_match {
        Diophant::replacements replacements;
        const Diophant::predicate &predicate;
    };

    maybe<definition_match> machine_match (const Machine::overloads &o, stack<Expression> x) {
        for (const auto &e : o) {
            if (e.Key.size () > x.size ()) return {};
            if (e.Key.size () < x.size ()) continue;

            auto r = match (e.Key, x);
            if (!r) continue;

            return definition_match {*r, e.Value};
        }

        return {};
    }

    maybe<Expression> match_and_replace (const Machine::overloads &o, stack<Expression> x) {
        auto w = machine_match (o, x);
        if (!w || !w->predicate.expr) return {};
        return replace (*w->predicate.expr, w->replacements);
    }

    subject subject::read (Expression &x) {
        auto p = x.get ();
        if (p == nullptr) goto fail;

        if (auto px = dynamic_cast<Symbol *> (p); px != nullptr)
            return subject (*px);

        if (auto pc = dynamic_cast<const expressions::call *> (p); pc != nullptr) {
            expression fun = pc->function;
            stack<Expression> arguments {pc->argument};

            while (true) {
                const auto *f = fun.get ();
                if (const auto *fx = dynamic_cast<Symbol *> (f); bool (fx))
                    return subject {*fx, arguments};

                if (const auto *fc = dynamic_cast<const expressions::call *> (f); bool (fc)) {
                    fun = fc->function;
                    arguments <<= fc->argument;
                    continue;
                }

                goto fail;
            }
        }

        if (auto pb = dynamic_cast<const expressions::binary_expression *> (p); pb != nullptr)
            throw exception {} << "cannot define " << x;

        if (auto pu = dynamic_cast<const expressions::unary_expression *> (p); pu != nullptr)
            throw exception {} << "cannot define " << x;

        fail:
        throw exception {} << "cannot define " << x;
    }

    Machine::Machine () {
/*
        define (Pattern {"null"}, Type {"null"}, Expression::null ());
        define (Pattern {"true"}, Type {"boolean"}, Expression::boolean (true));
        define (Pattern {"false"}, Type {"boolean"}, Expression::boolean (false));

        define (Pattern {"-x.Q"}, Type {"Q"});
        define (Pattern {"x.Q + y.Q"}, Type {"Q"});
        define (Pattern {"x.Q * y.Q"}, Type {"Q"});
        define (Pattern {"x.Q - y.Q"}, Type {"Q"});
        define (Pattern {"x.Q / y.Q"}, Type {"Q"});

        define (Pattern {"x.bool || y.bool"}, Type {"boolean"});
        define (Pattern {"x.bool && y.bool"}, Type {"boolean"});
        define (Pattern {"!x.bool"}, Type {"boolean"});

        define (Pattern {"x.Q == y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q != y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q <= y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q >= y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q < y.Q"}, Type {"boolean"});
        define (Pattern {"x.Q > y.Q"}, Type {"boolean"});
*/
    }

    Expression evaluate_second_step (Machine &m, Expression &x, data::set<expressions::symbol> fixed) {

        auto p = x.get ();

        if (auto pz = dynamic_cast<const expressions::list *> (p); pz != nullptr) {
            stack<Expression> evaluated;
            bool changed = false;
            for (Expression &e : pz->val) {
                Expression ex = evaluate (e, m, fixed);
                if (ex != e) changed = true;
                evaluated <<= ex;
            }

            return changed ? make::list (evaluated) : x;
        }

        if (auto pm = dynamic_cast<const expressions::map *> (p); pm != nullptr) {
            stack<entry<Expression, Expression>> evaluated;
            bool changed = false;
            for (const entry<Expression, Expression> &e : pm->val) {
                Expression ex = evaluate (e.Value, m, fixed);
                if (ex != e.Value) changed = true;
                evaluated <<= entry<Expression, Expression> {e.Key, ex};
            }

            return changed ? make::map (evaluated) : x;
        }

        if (auto pb = dynamic_cast<const expressions::binary_expression *> (p); pb != nullptr) {
            auto left = evaluate (pb->left, m, fixed);
            auto right = evaluate (pb->right, m, fixed);
            // TODO check definitions
            return left == pb->left && right == pb->right ? x : expressions::binary_expression::make (pb->op, left, right);
        }

        if (auto pu = dynamic_cast<const expressions::unary_expression *> (p); pu != nullptr) {
            auto expr = evaluate (pu->expression, m, fixed);
            // TODO check definitions
            return expr == pu->expression ? x : expressions::unary_expression::make (pu->op, expr);
        }

        return x;
    }

    // the first step is to evaluate parts of the expression that require looking up
    // definitions in the machine. That means symbols and calls.
    Expression Machine::evaluate (Expression &x, data::set<expressions::symbol> fixed) {

        auto p = x.get ();
        if (p == nullptr) return x;

        if (auto px = dynamic_cast<const expressions::symbol *> (p); px != nullptr) {
            if (fixed.contains (*px)) return x;
            auto v = this->definitions.contains (*px);
            if (!v) return x;
            auto w = match_and_replace (*v, {});
            if (!bool (w)) return x;
            return *w;
        }

        if (auto pc = dynamic_cast<const expressions::call *> (p); pc != nullptr) {
            // first evaluate function and argument.
            expression fun = Diophant::evaluate (pc->function, *this, fixed);
            expression arg = Diophant::evaluate (pc->argument, *this, fixed);
            auto f = fun.get ();

            // check function for lambda
            if (auto fl = dynamic_cast<const expressions::lambda *> (f); fl != nullptr)
                return Diophant::evaluate (replace (fl->body, {{fl->argument, arg}}), *this, fixed);

            stack<Expression> args {arg};
            Symbol *q = nullptr;

            while (true) {
                // check for symbol
                if (q = dynamic_cast<const expressions::symbol *> (f); q != nullptr) {
                    if (fixed.contains (*q)) goto end_call;
                    break;
                }

                // check for call
                if (auto fc = dynamic_cast<const expressions::call *> (f); fc != nullptr) {
                    f = fc->function.get ();
                    args <<= fc->argument;
                    continue;
                }

                // Something we can't work with is here so we return.
                goto end_call;
            }

            // look up what we got in the dictionary here.
            {
                auto v = this->definitions.contains (*q);
                if (!v) goto end_call;
                auto w = match_and_replace (*v, args);
                if (!bool (w)) goto end_call;
                return *w;
            }

            end_call:
            return fun == pc->function && arg == pc->argument ? x : make::call (fun, arg);
        }

        return evaluate_second_step (*this, x, fixed);
    }

    void Machine::declare (const subject &z, Type t) {
        //
        auto v = definitions.contains (z.root);
        if (!v) {
            definitions = definitions.insert (z.root, overloads {} << entry<parameters, predicate> {z.parameters, predicate {t}});
            return;
        }

        auto w = insert (*v, {z.parameters, predicate {t}});
        if (!w) return;
        throw exception {} << "conflicting definition " << subject {z.root, w->Key} << w->Value;

    }

    void Machine::define (const subject &z, const predicate &p) {
        std::cout << " defining " << z << p << std::endl;

        auto v = definitions.contains (z.root);
        if (!v) {
            definitions = definitions.insert (z.root, overloads {{z.parameters, p}});
            return;
        }

        auto w = insert (*v, {z.parameters, p});
        if (!w) return;
        throw exception {} << "conflicting definition " << subject {z.root, w->Key} << w->Value;
    }

}
