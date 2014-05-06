
#include "MathExpression.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_TPL_STRUCT(
	(OperatorTag),
	(car::BinaryOperator) (OperatorTag),
	(car::MathExpression, left)
	(car::MathExpression, right)
)

BOOST_FUSION_ADAPT_TPL_STRUCT(
	(OperatorTag),
	(car::UnaryOperator) (OperatorTag),
	(car::MathExpression, expr)
)

namespace car {

namespace fusion = boost::fusion;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

typedef ascii::space_type Skipper;

MathExpression makeAddition(const MathExpression& left, const MathExpression& right) {
	return BinaryOperator<OperatorAdd>(left, right);
}

MathExpression makeMultiplication(const MathExpression& left, const MathExpression& right) {
	return BinaryOperator<OperatorMultiply>(left, right);
}

template<class Iterator>
struct MathExpressionGrammar : qi::grammar<Iterator, MathExpression(), Skipper> {

	MathExpressionGrammar() : MathExpressionGrammar::base_type(expression) {
		using qi::_val;
		using qi::_1;
		using qi::_2;

		expression = additiveExpression.alias();

		additiveExpression = multiplicativeExpression[_val = _1] >>
			*(('+' >> multiplicativeExpression)[_val = phx::bind(makeAddition, _val, _1)]);

		multiplicativeExpression = primary[_val = _1] >>
			*(('*' >> primary)[_val = phx::bind(makeMultiplication, _val, _1)]);

		primary = qi::float_;
	}

	qi::rule<Iterator, MathExpression(), Skipper> expression;
	qi::rule<Iterator, MathExpression(), Skipper> additiveExpression;
	qi::rule<Iterator, MathExpression(), Skipper> multiplicativeExpression;
	qi::rule<Iterator, MathExpression(), Skipper> primary;
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
		throw FormulaException{"Error parsing \"" + input + "\", unparsed part: \"" + std::string(begin, end) + "\"."};
	}
	return result;
}

FormulaValue evaluateMathExpression(const std::string& input, const SymbolTable& symbolTable) {
	auto expression = parseMathExpression(input);
	return boost::apply_visitor(EvaluateVisitor{symbolTable}, expression);
}

}

