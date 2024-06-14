
#include <Diophant/expressions/lambda.hpp>
#include <data/io/unimplemented.hpp>

namespace Diophant::expressions {

    bool lambda::operator == (const abstract &x) const {
        try {
            auto z = dynamic_cast<const lambda &> (x);
            if (arguments.size () != z.arguments.size ()) return false;
            replacements r {{}};
            auto aa = arguments;
            auto zz = z.arguments;
            while (!data::empty (aa)) {
                r = r.insert (data::first (aa), make::symbol (data::first (zz).name));
                aa = data::rest (aa);
                zz = data::rest (zz);
            }
            return replace (body, r) == z.body;
        } catch (std::bad_cast) {
            return false;
        }
    }
    
}
