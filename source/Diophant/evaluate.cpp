
#include <Diophant/expressions/builder.hpp>
#include <Diophant/expressions/lambda.hpp>
#include <Diophant/expressions/call.hpp>
#include <Diophant/expressions/values.hpp>

namespace Diophant {
    
    Expression evaluate(Expression expr) {
        auto p = expr._.get();
        
        auto to_call = dynamic_cast<const expressions::call *>(p);
        if (to_call != nullptr) {
            auto to_lambda = dynamic_cast<const expressions::lambda*>(to_call->Function._.get());
            if (to_lambda != nullptr) return replace(to_lambda->Body, to_lambda->Argument, to_call->Argument);
            
            return call(evaluate(to_call->Function), evaluate(to_call->Argument));
        }
        
        auto to_list = dynamic_cast<const expressions::list *>(p);
        if (to_list != nullptr) {
            auto list = new expressions::list({});
            for (Expression e : to_list->val) list->val = list->val << evaluate(e);
            expression r;
            r._.reset(list);
            return r;
        }
        
        return expr;
        
    }
}

#include <boost/spirit/include/qi_match.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_plus.hpp>
#include <boost/spirit/include/qi_match.hpp>

using it = boost::spirit::basic_istream_iterator<char>;

namespace Diophant {
    namespace qi = boost::spirit::qi;
    
    std::istream &operator>>(std::istream &i, expression &e) {
        
        expression::builder builder{};
        
        auto read_symbol = [&builder](it begin, it end) -> void {
            builder.symbol(std::string{begin, end});
        };
        
        auto read_natural = [&builder](it begin, it end) -> void {
            builder.natural(data::N{std::string{begin, end}});
        };
        
        auto read_string = [&builder](it begin, it end) -> void {
            std::string strstr{begin, begin};
            builder.string(strstr.substr(1, strstr.size() - 2));
        };
        
        auto read_call = [&builder](it, it) -> void {
            builder.call();
        };
        
        auto read_lambda = [&builder](it, it) -> void {
            builder.lambda();
        };
        
        auto read_open_parens = [&builder](it) -> void {
            builder.open_parenthesis();
        };
        
        auto read_close_parens = [&builder](it) -> void {
            builder.close_parenthesis();
        };
        
        auto read_open_bracket = [&builder](it) -> void {
            builder.open_bracket();
        };
        
        auto read_close_bracket = [&builder](it) -> void {
            builder.close_bracket();
        };
        
        auto read_comma = [&builder](it) -> void {
            builder.comma();
        };
        
        qi::rule<it> subexpression;
        qi::rule<it> expression;
        
        qi::rule<it> optional_whitespace = *qi::ascii::space;
        
        qi::rule<it> required_whitespace = +qi::ascii::space;
        
        auto symbol = (qi::ascii::alpha >> *qi::ascii::alnum)[read_symbol];
        
        auto string = (qi::ascii::char_('"') >> 
            *(qi::ascii::print - qi::ascii::char_('"')) >> 
            qi::ascii::char_('"'))[read_string];
        
        auto natural = ((qi::ascii::char_('1', '9') >> *qi::ascii::digit) | 
            (qi::ascii::string("0x") >> *qi::ascii::xdigit))[read_natural];
            
        auto lambda = (qi::ascii::char_('\\') >> symbol >> required_whitespace >> subexpression)[read_lambda];
        
        auto call = subexpression >> *(required_whitespace >> subexpression[read_call]);
            
        auto parenthetical = (qi::ascii::char_('(')[read_open_parens] >> 
            expression >> qi::ascii::char_(')')[read_close_parens]);
            
        auto list = (qi::ascii::char_('[')[read_open_bracket] >> 
            (expression >> *(qi::ascii::char_(',')[read_comma] >> expression)) >> 
            qi::ascii::char_("]")[read_close_bracket]);
        
        subexpression = parenthetical | list | lambda | symbol | natural | string;
        
        expression = (optional_whitespace >> call >> optional_whitespace);
        
        i >> boost::spirit::qi::match(expression);
        e = builder.complete();
        
        return i;
    }
    
}
