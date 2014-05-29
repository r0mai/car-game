
#ifndef MATHEXPRESSION_HPP_
#define MATHEXPRESSION_HPP_

#include <map>
#include <string>
#include <stdexcept>
#include <istream>
#include <ostream>

#include <boost/variant.hpp>

namespace car {

struct FormulaException : std::runtime_error {
	using std::runtime_error::runtime_error;
};

typedef float FormulaValue;
typedef std::string Symbol;
typedef std::map<Symbol, FormulaValue> SymbolTable;

struct OperatorLess {
	static const int precedence = 0;
	static const std::string operatorString;
};

struct OperatorLessEqual {
	static const int precedence = 0;
	static const std::string operatorString;
};

struct OperatorGreater {
	static const int precedence = 0;
	static const std::string operatorString;
};

struct OperatorGreaterEqual {
	static const int precedence = 0;
	static const std::string operatorString;
};

struct OperatorAdd {
	static const int precedence = 1;
	static const std::string operatorString;
};

struct OperatorSubtract {
	static const int precedence = 1;
	static const std::string operatorString;
};

struct OperatorMultiply {
	static const int precedence = 2;
	static const std::string operatorString;
};

struct OperatorDivide {
	static const int precedence = 2;
	static const std::string operatorString;
};

template<class OperatorTag>
struct BinaryOperator;

struct OperatorMinus;

template<class OperatorTag>
struct UnaryOperator;

typedef boost::variant<
	FormulaValue,
	Symbol,
	boost::recursive_wrapper<UnaryOperator<OperatorMinus>>,
	boost::recursive_wrapper<BinaryOperator<OperatorLess>>,
	boost::recursive_wrapper<BinaryOperator<OperatorLessEqual>>,
	boost::recursive_wrapper<BinaryOperator<OperatorGreater>>,
	boost::recursive_wrapper<BinaryOperator<OperatorGreaterEqual>>,
	boost::recursive_wrapper<BinaryOperator<OperatorAdd>>,
	boost::recursive_wrapper<BinaryOperator<OperatorSubtract>>,
	boost::recursive_wrapper<BinaryOperator<OperatorMultiply>>,
	boost::recursive_wrapper<BinaryOperator<OperatorDivide>>
> MathExpression;

template<class Op>
struct UnaryOperator {

	typedef Op OperatorTag;

	UnaryOperator() = default;
	UnaryOperator(const MathExpression& expr) :
		expr(expr) {}

	MathExpression expr;
};

template<class Op>
struct BinaryOperator {

	typedef Op OperatorTag;

	BinaryOperator() = default;
	BinaryOperator(const MathExpression& left, const MathExpression& right) :
		left(left), right(right) {}

	MathExpression left;
	MathExpression right;
};

MathExpression parseMathExpression(const std::string& input);
FormulaValue evaluateMathExpressionFromString(const std::string& input, const SymbolTable& symbolTable = SymbolTable{});
FormulaValue evaluateMathExpression(const MathExpression& expr, const SymbolTable& symbolTable = SymbolTable{});

std::ostream& operator<<(std::ostream& os, const MathExpression& expression);
std::istream& operator>>(std::istream& is, MathExpression& expression);

}

#endif
