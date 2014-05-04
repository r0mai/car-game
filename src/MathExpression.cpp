
#include "MathExpression.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(
	car::BinaryOperatorBase,
	(car::MathExpression, left)
	(car::MathExpression, right)
)

BOOST_FUSION_ADAPT_STRUCT(
	car::UnaryOperatorBase,
	(car::MathExpression, expr)
)

namespace car {

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

typedef ascii::space_type Skipper;

template<class Iterator>
struct MathExpressionGrammar : qi::grammar<Iterator, MathExpression(), Skipper> {
	MathExpressionGrammar() : MathExpressionGrammar::base_type(expression) {

	}
	qi::rule<Iterator, MathExpression(), Skipper> expression;
};

MathExpression parseMathExpression(const std::string& input) {
	MathExpression result;

	std::string::const_iterator begin = input.cbegin();
	std::string::const_iterator end = input.cend();

	MathExpressionGrammar<std::string::const_iterator> grammar;

	bool success = qi::phrase_parse(
		begin, end,
		grammar,
		Skipper(),
		result);

	if (!success || begin != end) {
		throw FormulaException{"Error parsing \"" + input + "\"."};
	}
	return result;
}

}

