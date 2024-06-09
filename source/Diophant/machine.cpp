
#include <Diophant/replace.hpp>
#include <Diophant/machine.hpp>
#include <Diophant/evaluate.hpp>
#include <Diophant/match.hpp>
#include <Diophant/expressions/apply.hpp>
#include <Diophant/expressions/binary.hpp>
#include <Diophant/expressions/unary.hpp>
#include <Diophant/expressions/values.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/pattern.hpp>
#include <Diophant/expressions/if.hpp>
#include <Diophant/expressions/let.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant {

    std::ostream &indent (std::ostream &o, int ind) {
        for (int i = 0; i < ind; i++) o << "  ";
        return o;
    }

    std::ostream &print (std::ostream &o, Machine::overloads v, int ind);

    std::ostream &print (std::ostream &o, Machine::definition v, int ind) {
        o << v.key;
        if (bool (v.value)) o << " := " << *v.value;
        else o << ";";
        
        if (v.more_specific.size () != 0) return print (o << " // ", v.more_specific, ind + 1);
        return o;
    }
    
    std::ostream inline &operator << (std::ostream &o, const Machine::definition &t) {
        return print (o, t, 0);
    }

    std::ostream &print (std::ostream &o, Machine::overloads v, int ind) {
        o << "{\n";
        for (const auto &t : v) print (o, t, ind + 1);
        return indent (o, ind) << "}";
    }

    std::ostream &operator << (std::ostream &o, const Machine &m) {
        o << "Machine \n{";
        for (const auto &e : m.definitions) {
            o << "\n  " << e.first << " : ";
            print (o, e.second, 1);
        }
        return o << "\n}";
    }

    parameters::parameters (stack<Expression> z, Type st) {
        params.resize (z.size ());

        for (auto &p : params) {
            p = pattern {z.first ()};
            z = z.rest ();
        }

        such_that = st;
    }

    maybe<Expression> condition (const parameters &p, stack<Expression> x) {
        if (p.params.size () != data::size (x)) return {};

        auto pi = p.params.begin ();
        auto xi = x.begin ();

        replacements r {{}};

        while (pi != p.params.end ()) {
            r = combine (r, match (*pi, *xi));
            if (!bool (r)) return {};
        }

        return replace (p.such_that, *r);
    }

    maybe<replacements> match (const parameters &p, stack<Expression> x) {
        if (p.params.size () != data::size (x)) return {};
        
        /*
        auto c = condition (p, x);
        if (!bool (c)) return {};

        intuit cx = constructable (*c);

        if (cx == no) return {};
        if (cx == unknown) throw unknown_construction {p.such_that};*/

        replacements r {{}};
        int i = 0;
        
        for (Expression &e : x) {
            r = combine (r, match (p.params[i], e));
            if (!r) return {};
        }
        
        return {r};
    }

    // return the conflicting entry if there is one.
    // will not modify the overloads if there is a conflict.
    const Machine::definition *insert (Machine::overloads &o, const Machine::definition &e);

    void def (const subject &z, maybe<expression> x, Machine &m) {
        //
        auto v = m.definitions.find (*z.root);
        if (v == m.definitions.end ()) {
            Machine::overloads o {{Machine::definition {z.parameters, x}}};
            m.definitions[*z.root] = o;
            return;
        }

        auto w = insert (v->second, Machine::definition {z.parameters, x});
        if (!w) return;
        throw exception {} << "conflicting definition " << subject {z.root, w->key} << w->value;
    }

    void Machine::declare (const subject &z) {
        def (z, {}, *this);
    }

    void Machine::define (const subject &z, const Expression &p) {
        def (z, p, *this);
    }

    Machine::Machine () {
        
        make::symbol ("Impossible", registered);
        make::symbol ("Void", registered);
        make::symbol ("Bool", registered);
        make::symbol ("Intuit", registered);
        make::symbol ("Sign", registered);
        make::symbol ("N", registered);
        make::symbol ("Z", registered);
        make::symbol ("Q", registered);
        make::symbol ("Float", registered);
        make::symbol ("String", registered);
        make::symbol ("Int", registered);
        make::symbol ("Uint", registered);
        make::symbol ("List", registered);
        make::symbol ("Tuple", registered);
        make::symbol ("Struct", registered);
        make::symbol ("Array", registered);
        
        make::symbol ("null", registered);
        make::symbol ("true", registered);
        make::symbol ("false", registered);
        make::symbol ("unknown", registered);
        make::symbol ("positive", registered);
        make::symbol ("negative", registered);
        make::symbol ("zero", registered);
        make::symbol ("infinity", registered);
        
        make::symbol ("!l", registered);
        make::symbol ("!r", registered);
        make::symbol ("++l", registered);
        make::symbol ("--l", registered);
        make::symbol ("^", registered);
        make::symbol ("*", registered);
        make::symbol ("%", registered);
        make::symbol ("/", registered);
        make::symbol ("/%", registered);
        make::symbol ("<", registered);
        make::symbol (">", registered);
        make::symbol ("<=", registered);
        make::symbol (">=", registered);
        make::symbol ("==", registered);
        make::symbol ("!=", registered);
        make::symbol ("&&", registered);
        make::symbol ("||", registered);
        make::symbol ("=", registered);
        make::symbol ("/=", registered);
        make::symbol ("<==>", registered);
        make::symbol (":", registered);
        make::symbol ("&", registered);
        make::symbol ("|", registered);
    }

    void initialize (Machine &m) {
        type::symbols () = &m.registered;
        
        m.define (string {"true"}, make::boolean (true));
        m.define (string {"false"}, make::boolean (false));
        /*
        m.define (string {"_x == _x ? x:Value"}, string {"true"});
        m.define (string {"_x == _y ? x:Value & y:Value"}, string {"false"});
        m.define (string {"not true"}, string {"false"});
        m.define (string {"not false"}, string {"true"});
        m.define (string {"!_x ? x:Bool"}, string {"not"});
        m.define (string {"_x != _y ? x:Value & y:Value"}, string {"!(x == y)"});
        m.define (string {"and true true"}, string {"true"});
        m.define (string {"and _x _y ? x:Bool & y:Bool"}, string {"false"});
        m.define (string {"or false false"}, string {"false"});
        m.define (string {"or _x _y ? x:Bool & y:Bool"}, string {"false"});
        m.define (string {"xor _x _y ? x:Bool & y:Bool"}, string {"`!=`"});
        m.define (string {"implies _x _y ? x:Bool & y:Bool"}, string {"or y !x"});
        m.define (string {"nand true true"}, string {"false"});
        m.define (string {"nand _x _y ? x:Bool y:Bool"}, string {"true"});
        m.define (string {"nor false false"}, string {"true"});
        m.define (string {"nor _x _y ? x:Bool & y:Bool"}, string {"false"});
        m.define (string {"`&&`"}, string {"Bool => Bool => Bool # and"});
        m.define (string {"`||`"}, string {"Bool => Bool => Bool # or"});*/
        
        m.registered.update ();
        
    }

    // return the conflicting entry if there is one.
    // will not modify the overloads if there is a conflict.
    const Machine::definition *insert (Machine::overloads &o, const Machine::definition &e) {
        stack<const Machine::definition &> left;
        Machine::overloads right = o;
        
        // flip through the stack
        while (data::size (right) > 0) {
            Machine::definition &next = right.first ();
            
            // we order by the number of parameters, naturally.
            if (e.key.params.size () > next.key.params.size ()) goto flip;
            if (e.key.params.size () < next.key.params.size ()) {
                right << e;
                break;
            }
            
            {
                intuitionistic_partial_ordering o = e.key <=> next.key;

                if (intuit are_equal = o.equal (); are_equal == yes) {
                    if (!bool (next.value)) {
                        next.value = e.value;
                        return nullptr;
                    }

                    // note: this operation is not sufficient since there
                    // could be vars with different names.
                    return next.value == e.value ? nullptr : &next;
                } else if (are_equal == unknown) throw exception {} << "cannot detect equality of " << e.key << " and " << next.key;

                if (intuit is_sub = o.left_castable; is_sub == yes)
                    return insert (next.more_specific, e);
                else if (is_sub == unknown) throw exception {} << "cannot detect sub of " << e.key << " and " << next.key;

                if (intuit are_disjoint = o.disjoint; are_disjoint == no) return &next;
                else if (are_disjoint == unknown) throw exception {} << "cannot detect disjoint of " << e.key << " and " << next.key;
            }

            flip:
            left = data::prepend (left, next);
            right = data::rest (right);
        }

        // put the stack back together
        while (data::size (left) > 0) {
            right = data::prepend (right, data::first (left));
            left = data::rest (left);
        }

        o = right;

        return nullptr;
    }

    subject Machine::make_subject (Expression &x) {
        if (x.get () == nullptr) goto fail;

        if (auto px = std::dynamic_pointer_cast<const expressions::symbol> (x); px != nullptr)
            return subject (px);

        if (auto pc = std::dynamic_pointer_cast<const expressions::call> (x); pc != nullptr) {
            expression fun = pc->function;
            stack<Expression> arguments {pc->argument};

            while (true) {
                if (const auto fx = std::dynamic_pointer_cast<const expressions::symbol> (fun); bool (fx))
                    return subject {fx, arguments};

                if (const auto fc = std::dynamic_pointer_cast<const expressions::call> (fun); bool (fc)) {
                    fun = fc->function;
                    arguments <<= fc->argument;
                    continue;
                }

                goto fail;
            }
        }

        if (auto pst = std::dynamic_pointer_cast<const expressions::such_that> (x); pst != nullptr) {
            auto x = make_subject (pst->pattern);
            x.parameters.such_that = pst->type;
            return x;
        }

        if (auto pb = std::dynamic_pointer_cast<const expressions::binary_expression> (x); pb != nullptr) {
            auto v = registered.find (binary_operator (pb->op));
            if (! bool (v)) throw exception {} << " unknown binary operator " << binary_operator (pb->op);
            subject x (v);
            x.parameters.params.resize (2);
            x.parameters.params[0] = pb->left;
            x.parameters.params[1] = pb->right;
            return x;
        }

        if (auto pu = std::dynamic_pointer_cast<const expressions::left_unary_expression> (x); pu != nullptr)
            throw exception {} << "not yet implemented: define " << x;

        fail:
        throw exception {} << "cannot define " << x;
    }

    maybe<Expression> match_and_call (const Machine::overloads &o, stack<Expression> x);

    Expression evaluate_second_step (Machine &m, Expression &x, data::set<expressions::symbol> fixed);

    // the first step is to evaluate parts of the expression that require looking up
    // definitions in the machine. That means symbols and calls.
    Expression Machine::evaluate (Expression &x, data::set<expressions::symbol> fixed) {
        auto p = x.get ();
        if (p == nullptr) return x;

        if (auto px = dynamic_cast<const expressions::symbol *> (p); px != nullptr) {
            if (fixed.contains (*px)) return x;
            auto v = this->definitions.find (*px);
            if (v == this->definitions.end ()) return x;
            auto w = match_and_call (v->second, {});
            return bool (w) ? *w : x;
        }

        if (auto pc = dynamic_cast<const expressions::call *> (p); pc != nullptr) {
            // first evaluate function and argument.
            expression fun = Diophant::evaluate (pc->function, *this, fixed);
            expression arg = Diophant::evaluate (pc->argument, *this, fixed);

            stack<Expression> args {arg};
            symbol q = nullptr;

            while (true) {
                // check for lambda
                if (auto l = std::dynamic_pointer_cast<const expressions::lambda> (fun); l != nullptr) {
                    fun = (*l) (data::first (args));
                    args = data::rest (args);
                    if (data::size (args) == 0) {
                        return Diophant::evaluate (fun, *this, fixed);
                    }
                }
                
                // check for symbol
                if (q = std::dynamic_pointer_cast<const expressions::symbol> (fun); q != nullptr) {
                    if (fixed.contains (*q)) goto end_call;
                    break;
                }

                // check for call
                if (auto fc = std::dynamic_pointer_cast<const expressions::call> (fun); fc != nullptr) {
                    fun = fc->function;
                    args <<= Diophant::evaluate (fc->argument, *this, fixed);
                    continue;
                }

                // Something we can't work with is here so we return.
                goto end_call;
            }

            // look up what we got in the dictionary here.
            {
                auto v = this->definitions.find (*q);
                if (v == this->definitions.end ()) goto end_call;
                auto w = match_and_call (v->second, args);
                if (!bool (w)) goto end_call;
                return *w;
            }

            end_call:
            return fun == pc->function && arg == pc->argument ? x : make::call (fun, arg);
        }

        return evaluate_second_step (*this, x, fixed);
    }

    Expression evaluate_second_step (Machine &m, Expression &x, data::set<expressions::symbol> fixed) {

        auto p = x.get ();

        if (auto pb = dynamic_cast<const expressions::binary_expression *> (p); pb != nullptr) {
            auto left = evaluate (pb->left, m, fixed);
            auto right = evaluate (pb->right, m, fixed);
            // TODO check definitions
            return left == pb->left && right == pb->right ? x : expressions::binary_expression::make (pb->op, left, right);
        }

        if (auto pu = dynamic_cast<const expressions::left_unary_expression *> (p); pu != nullptr) {
            auto expr = evaluate (pu->expression, m, fixed);
            // TODO check definitions
            return expr == pu->expression ? x : expressions::left_unary_expression::make (pu->op, expr);
        }

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

        if (auto pif = dynamic_cast<const expressions::dif *> (p); pif != nullptr) {
            auto cond = evaluate (pif->Condition, m, fixed);
            if (auto bb = dynamic_cast<const expressions::boolean *> (cond.get ()); bb != nullptr) {
                return bb->val ? evaluate (pif->Then, m, fixed) : evaluate (pif->Else, m, fixed);
            } else throw exception {} << " if condition failed to evaluate to Bool: " << pif->Condition;
        }

        if (auto plet = dynamic_cast<const expressions::let *> (p); plet != nullptr) {
            throw exception {} << " we don't evaluate let expressions yet";
        }

        return x;
    }

    struct definition_match {
        Diophant::replacements replacements;
        Expression &predicate;
    };

    maybe<Expression> match_and_call (const Machine::overloads &o, stack<Expression> x) {
        for (const auto &e : o) {
            if (e.key.params.size () > x.size ()) return {};
            if (e.key.params.size () < x.size ()) continue;
            
            auto r = match (e.key, x);
            if (!r) continue;

            if (bool (e.value)) return replace (*e.value, *r);
        }

        return {};
    }

    intuitionistic_partial_ordering operator <=> (const parameters &a, const parameters &b) {
        if (a.params.size () != b.params.size ()) return {yes, no, no};
        
        auto ai = a.params.begin ();
        auto bi = b.params.begin ();
        
        bidirectional_replacements mm {{{}}, {{}}};
        
        while (ai != a.params.end ()) {
            mm = compare (*ai, *bi, mm);
            ai++;
            bi++;
        }
        
        // if neither set of parameters can be replaced into each other, 
        // then the two patterns are disjoint. 
        if (!bool (mm.left) && !bool (mm.right)) return {yes, no, no};
        
        if (bool (mm.left) && bool (mm.right)) 
            return compare (replace (a.such_that, mm.left), b.such_that);
        
        if (bool (mm.left)) {
            auto x = compare (replace (a.such_that, mm.left), b.such_that);
            x.right_castable = no;
            return x;
        }
        
        auto x = compare (a.such_that, replace (b.such_that, mm.right));
        x.left_castable = no;
        return x;
    }

}
