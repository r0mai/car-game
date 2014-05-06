
#ifndef MATHEXPRESSION_HPP_
#define MATHEXPRESSION_HPP_

#include <map>
#include <string>
#include <stdexcept>

#include <boost/variant.hpp>

namespace car {

struct FormulaException : std::runtime_error {
	using std::runtime_error::runtime_error;
};

typedef float FormulaValue;
typedef std::string Symbol;
typedef std::map<Symbol, FormulaValue> SymbolTable;

struct OperatorAdd;
struct OperatorSubtract;
struct OperatorMultiply;
struct OperatorDivide;

template<class OperatorTag>
struct BinaryOperator;

struct OperatorMinus;
struct OperatorPlus;

template<class OperatorTag>
struct UnaryOperator;

typedef boost::variant<
	FormulaValue,
	Symbol,
   	boost::recursive_wrapper<UnaryOperator<OperatorMinus>>,
   	boost::recursive_wrapper<UnaryOperator<OperatorPlus>>,
   	boost::recursive_wrapper<BinaryOperator<OperatorAdd>>,
   	boost::recursive_wrapper<BinaryOperator<OperatorSubtract>>,
   	boost::recursive_wrapper<BinaryOperator<OperatorMultiply>>,
   	boost::recursive_wrapper<BinaryOperator<OperatorDivide>>
> MathExpression;

template<class OperatorTag>
struct UnaryOperator {
	UnaryOperator() = default;
	UnaryOperator(const MathExpression& expr) :
		expr(expr) {}

	MathExpression expr;
};

template<class OperatorTag>
struct BinaryOperator {
	BinaryOperator() = default;
	BinaryOperator(const MathExpression& left, const MathExpression& right) :
		left(left), right(right) {}

	MathExpression left;
	MathExpression right;
};

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

	FormulaValue operator()(const UnaryOperator<OperatorPlus>& unary) const {
		return +boost::apply_visitor(EvaluateVisitor{symbolTable}, unary.expr);
	}

private:
	const SymbolTable& symbolTable;
};

MathExpression parseMathExpression(const std::string& input);
FormulaValue evaluateMathExpression(const std::string& input, const SymbolTable& symbolTable = SymbolTable{});

}

#endif
