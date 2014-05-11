
#include "MathExpression.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace car {

namespace fusion = boost::fusion;
namespace phx = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

const std::string OperatorLess::operatorString = "<";
const std::string OperatorGreater::operatorString = ">";
const std::string OperatorLessEqual::operatorString = "<=";
const std::string OperatorGreaterEqual::operatorString = ">=";
const std::string OperatorAdd::operatorString = "+";
const std::string OperatorSubtract::operatorString = "-";
const std::string OperatorMultiply::operatorString = "*";
const std::string OperatorDivide::operatorString = "/";

typedef ascii::space_type Skipper;

template<class Op>
MathExpression makeBinaryOperator(const MathExpression& left, const MathExpression& right) {
	return BinaryOperator<Op>(left, right);
}

template<class Op>
MathExpression makeUnaryOperator(const MathExpression& expr) {
	return UnaryOperator<Op>(expr);
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
		using qi::float_;
		using qi::char_;

		expression = comparsionExpression.alias();

		comparsionExpression =
			additiveExpression[_val = _1] >>
			*('<' >> additiveExpression[_val = phx::bind(makeBinaryOperator<OperatorLess>, _val, _1)] |
			"<=" >> additiveExpression[_val = phx::bind(makeBinaryOperator<OperatorLessEqual>, _val, _1)] |
			'>' >> additiveExpression[_val = phx::bind(makeBinaryOperator<OperatorGreater>, _val, _1)] |
			">=" >> additiveExpression[_val = phx::bind(makeBinaryOperator<OperatorGreaterEqual>, _val, _1)]);

		additiveExpression =
			multiplicativeExpression[_val = _1] >>
			*('+' >> multiplicativeExpression[_val = phx::bind(makeBinaryOperator<OperatorAdd>, _val, _1)] |
			'-' >> multiplicativeExpression[_val = phx::bind(makeBinaryOperator<OperatorSubtract>, _val, _1)]);

		multiplicativeExpression =
			unraryPlusMinusExpression[_val = _1] >>
			*('*' >> unraryPlusMinusExpression[_val = phx::bind(makeBinaryOperator<OperatorMultiply>, _val, _1)] |
			'/' >> unraryPlusMinusExpression[_val = phx::bind(makeBinaryOperator<OperatorDivide>, _val, _1)] |
			!(char_('+') | char_('-')) >> unraryPlusMinusExpression[_val = phx::bind(makeBinaryOperator<OperatorMultiply>, _val, _1)]);

		unraryPlusMinusExpression =
			primary[_val = _1] |
			'-' >> unraryPlusMinusExpression[_val = phx::bind(makeUnaryOperator<OperatorMinus>, _1)] |
			'+' >> unraryPlusMinusExpression[_val = _1];

		symbol = (+alpha)[_val = phx::bind(makeSymbol, _1)];

		primary %= float_ | symbol | ('(' >> expression >> ')');
	}

	qi::rule<Iterator, MathExpression(), Skipper> expression;
	qi::rule<Iterator, MathExpression(), Skipper> comparsionExpression;
	qi::rule<Iterator, MathExpression(), Skipper> additiveExpression;
	qi::rule<Iterator, MathExpression(), Skipper> multiplicativeExpression;
	qi::rule<Iterator, MathExpression(), Skipper> unraryPlusMinusExpression;
	qi::rule<Iterator, MathExpression(), Skipper> symbol;
	qi::rule<Iterator, MathExpression(), Skipper> primary;
};

template<class Iterator>
MathExpression parseMathExpression(Iterator begin, Iterator end) {

	MathExpression result;

	MathExpressionGrammar<Iterator> grammar;

	bool success = qi::phrase_parse(
		begin, end,
		grammar,
		Skipper(),
		result);

	if (!success || begin != end) {
		throw FormulaException{"Error parsing, unparsed part: \"" + std::string(begin, end) + "\"."};
	}
	return result;
}

MathExpression parseMathExpression(const std::string& input) {
	return parseMathExpression(input.begin(), input.end());
}

struct EvaluateVisitor : boost::static_visitor<FormulaValue> {
	EvaluateVisitor(const SymbolTable& symbolTable) :
		symbolTable(symbolTable) {}

	FormulaValue operator()(const FormulaValue& value) const {
		return value;
	}

	FormulaValue operator()(const Symbol& symbol) const {
		auto it = symbolTable.find(symbol);
		if (it == symbolTable.end()) {
			throw FormulaException{"Symbol \"" + symbol + "\" not found in symbol map."};
		}
		return it->second;
	}

	FormulaValue operator()(const BinaryOperator<OperatorLess>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) <
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorLessEqual>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) <=
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorGreater>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) >
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorGreaterEqual>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) >=
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorAdd>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) +
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorSubtract>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) -
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorMultiply>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) *
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const BinaryOperator<OperatorDivide>& binary) const {
		return
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.left) /
			boost::apply_visitor(EvaluateVisitor{symbolTable}, binary.right);
	}

	FormulaValue operator()(const UnaryOperator<OperatorMinus>& unary) const {
		return -boost::apply_visitor(EvaluateVisitor{symbolTable}, unary.expr);
	}

private:
	const SymbolTable& symbolTable;
};

FormulaValue evaluateMathExpressionFromString(const std::string& input, const SymbolTable& symbolTable) {
	return evaluateMathExpression(parseMathExpression(input), symbolTable);
}

FormulaValue evaluateMathExpression(const MathExpression& expression, const SymbolTable& symbolTable) {
	return boost::apply_visitor(EvaluateVisitor{symbolTable}, expression);
}

struct PrintVisitor : boost::static_visitor<void> {

	PrintVisitor(std::ostream& os, int lastPrecedence = 0) : os(os), precedence(lastPrecedence) {}

	void operator()(const FormulaValue& value) const {
		os << value;
	}

	void operator()(const Symbol& symbol) const {
		os << symbol;
	}

	template<class Tag>
	void operator()(const BinaryOperator<Tag>& binary) const {
		if (precedence > Tag::precedence) { os << '('; }

		boost::apply_visitor(PrintVisitor{os, Tag::precedence}, binary.left);
		os << Tag::operatorString;
		boost::apply_visitor(PrintVisitor{os, Tag::precedence}, binary.right);

		if (precedence > Tag::precedence) { os << ')'; }
	}

	void operator()(const UnaryOperator<OperatorMinus>& unary) const {
		os << '-';
		if (precedence >= OperatorMultiply::precedence) { os << '('; }

		boost::apply_visitor(PrintVisitor{os, OperatorMultiply::precedence}, unary.expr);

		if (precedence >= OperatorAdd::precedence) { os << ')'; }
	}

private:
	std::ostream& os;
	int precedence;
};

std::ostream& operator<<(std::ostream& os, const MathExpression& expression) {
	boost::apply_visitor(PrintVisitor{os}, expression);
	return os;
}

std::istream& operator>>(std::istream& is, MathExpression& expression) {
	is.unsetf(std::ios::skipws);

	boost::spirit::istream_iterator begin(is);
	boost::spirit::istream_iterator end;

	expression = parseMathExpression(begin, end);

	return is;
}

}

