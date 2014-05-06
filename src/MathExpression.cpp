
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

template<class Op>
MathExpression makeBinaryOperator(const MathExpression& left, const MathExpression& right) {
	return BinaryOperator<Op>(left, right);
}

MathExpression makeSymbol(const std::vector<char>& symbol) {
	return MathExpression(std::string(symbol.begin(), symbol.end()));
}

template<class Iterator>
struct MathExpressionGrammar : qi::grammar<Iterator, MathExpression(), Skipper> {

	MathExpressionGrammar() : MathExpressionGrammar::base_type(expression) {
		using qi::_val;
		using qi::_1;
		using qi::_2;
		using qi::alnum;
		using qi::alpha;

		expression = additiveExpression.alias();

		additiveExpression = multiplicativeExpression[_val = _1] >>
			*(('+' >> multiplicativeExpression)[_val = phx::bind(makeBinaryOperator<OperatorAdd>, _val, _1)] |
			('-' >> multiplicativeExpression)[_val = phx::bind(makeBinaryOperator<OperatorSubtract>, _val, _1)]);

		multiplicativeExpression = primary[_val = _1] >>
			*(('*' >> primary)[_val = phx::bind(makeBinaryOperator<OperatorMultiply>, _val, _1)] |
			('/' >> primary)[_val = phx::bind(makeBinaryOperator<OperatorDivide>, _val, _1)]);

		symbol = (+alpha)[_val = phx::bind(makeSymbol, _1)];

		primary %= qi::float_ | symbol;
	}

	qi::rule<Iterator, MathExpression(), Skipper> expression;
	qi::rule<Iterator, MathExpression(), Skipper> additiveExpression;
	qi::rule<Iterator, MathExpression(), Skipper> multiplicativeExpression;
	qi::rule<Iterator, MathExpression(), Skipper> symbol;
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

