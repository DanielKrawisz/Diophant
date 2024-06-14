
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
#include <Diophant/expressions/function.hpp>

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

    parameters::parameters (list<Expression> z, Type st) {
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

    maybe<replacements> match (const parameters &p, list<Expression> x) {
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
        auto v = m.definitions.find (z.root);
        if (v == m.definitions.end ()) {
            Machine::overloads o {{Machine::definition {z.parameters, x}}};
            m.definitions[z.root] = o;
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
        /*
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
        make::symbol ("+", registered);
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
        make::symbol ("|", registered);*/
    }

    void initialize (Machine &m) {
        
        m.define (string {"true"}, make::boolean (true));
        m.define (string {"false"}, make::boolean (false));
        m.define (string {"`+`"}, make::built_in_function<data::N, const data::N &, const data::N &> (&plus_N));
        m.define (string {"`*`"}, make::built_in_function<data::N, const data::N &, const data::N &> (&times_N));
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
        
    }

    Expression eval (Expression &x, Machine &m, data::set<symbol> fixed) {
        // evaluate again and again until the result doesn't change
        expression last = x;
        int i = 0;
        while (true) {
            expression next = m.evaluate (last, fixed);
            if (next == last) return last;
            last = next;
        }
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

    maybe<expressions::call> match_and_call (const Machine::overloads &o, list<Expression> x);

    Expression Machine::evaluate (Expression &x, data::set<symbol> fixed) {
        auto p = x.get ();
        if (p == nullptr) return x;

        if (auto q = std::dynamic_pointer_cast<const expressions::symbol> (x); q != nullptr) {
            if (fixed.contains (*q)) return x;
            auto v = this->definitions.find (*q);
            if (v == this->definitions.end ()) return x;
            auto w = match_and_call (v->second, {});
            return bool (w) ? w->function : x;
        }

        if (auto pa = dynamic_cast<const expressions::call *> (p); pa != nullptr) {

            auto eval_arg = [this, &fixed] (Expression &x) -> Expression {
                return eval (x, *this, fixed);
            };

            expressions::call call {pa->function, data::for_each (eval_arg, pa->arguments)};

            while (true) {

                // check for apply
                if (auto ca = std::dynamic_pointer_cast<const expressions::call> (call.function); ca != nullptr) {
                    call = expressions::call {ca->function, data::for_each (eval_arg, ca->arguments) + call.arguments};

                // check for lambda
                } else if (auto la = std::dynamic_pointer_cast<const expressions::lambda> (call.function); la != nullptr) {
                    call = expressions::call {(*la) (data::first (call.arguments)), data::rest (call.arguments)};
                    if (data::size (call.arguments) == 0) return eval (call.function, *this, fixed);

                // check for function
                } else if (auto fa = std::dynamic_pointer_cast<const expressions::function> (call.function); fa != nullptr) {
                    uint32 na = fa->number_of_args ();
                    if (data::size (call.arguments) >= na)
                        call = expressions::call {
                            fa->operator () (data::take (call.arguments, na)),
                            data::drop (call.arguments, na)};
                    if (data::size (call.arguments) == 0) return call.function;
                    else break;

                // check for symbol
                } else if (auto q = std::dynamic_pointer_cast<const expressions::symbol> (call.function); q != nullptr) {
                    if (fixed.contains (*q)) break;
                    auto v = this->definitions.find (*q);
                    if (v == this->definitions.end ()) break;
                    auto w = match_and_call (v->second, call.arguments);
                    if (!bool (w)) break;
                    call = *w;
                    continue;
                }

                auto result = eval (call.function, *this, fixed);
                if (result == call.function) break;
            }

            end_call:
            return call.function == pa->function && call.arguments == pa->arguments ? x : make::call (call.function, call.arguments);
        }
        /*
        if (auto pb = dynamic_cast<const expressions::binary_expression *> (p); pb != nullptr) {
            std::cout << " found binary expression" << std::endl;
        }*/

        if (auto pz = dynamic_cast<const expressions::list *> (p); pz != nullptr) {
            stack<Expression> evaluated;
            bool changed = false;
            for (Expression &e : pz->val) {
                Expression ex = eval (e, *this, fixed);
                if (ex != e) changed = true;
                evaluated <<= ex;
            }

            return changed ? make::list (reverse (evaluated)) : x;
        }

        if (auto pm = dynamic_cast<const expressions::map *> (p); pm != nullptr) {
            stack<entry<Expression, Expression>> evaluated;
            bool changed = false;
            for (const entry<Expression, Expression> &e : pm->val) {
                Expression ex = eval (e.Value, *this, fixed);
                if (ex != e.Value) changed = true;
                evaluated <<= entry<Expression, Expression> {e.Key, ex};
            }

            return changed ? make::map (evaluated) : x;
        }

        if (auto pif = dynamic_cast<const expressions::dif *> (p); pif != nullptr) {
            auto cond = eval (pif->Condition, *this, fixed);
            if (auto bb = dynamic_cast<const expressions::boolean *> (cond.get ()); bb != nullptr) {
                return bb->val ? eval (pif->Then, *this, fixed) : eval (pif->Else, *this, fixed);
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

    maybe<expressions::call> match_and_call (const Machine::overloads &o, list<Expression> x) {
        for (const auto &e : o) {
            auto num_args = e.key.params.size ();
            if (num_args > x.size ()) return {};
            
            auto r = Diophant::match (e.key, take (x, num_args));
            if (!r) continue;

            if (bool (e.value)) return expressions::call {replace (*e.value, *r), drop (x, num_args)};
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
