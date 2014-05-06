
#include <boost/test/unit_test.hpp>

#include "MathExpression.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(MathExpressionTest)

BOOST_AUTO_TEST_CASE(test_float_parse_1) {
	BOOST_CHECK_CLOSE(evaluateMathExpression("2"), FormulaValue(2), 0.001);
}

BOOST_AUTO_TEST_CASE(test_float_parse_2) {
	BOOST_CHECK_CLOSE(evaluateMathExpression("2.0"), FormulaValue(2), 0.001);
}

BOOST_AUTO_TEST_CASE(test_add_mul_precedence) {
	BOOST_CHECK_CLOSE(evaluateMathExpression("2+3*4"), FormulaValue(14), 0.001);
}

BOOST_AUTO_TEST_CASE(test_symbol_parsing_1) {
	SymbolTable table = {{"s", FormulaValue(3.0)}};
	BOOST_CHECK_CLOSE(evaluateMathExpression("s", table), FormulaValue(3.0), 0.001);
}

BOOST_AUTO_TEST_CASE(test_symbol_parsing_2) {
	SymbolTable table = {{"s", FormulaValue(3.0)}};
	BOOST_CHECK_CLOSE(evaluateMathExpression("s+1", table), FormulaValue(4.0), 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

