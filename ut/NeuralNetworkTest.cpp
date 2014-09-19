
#include <boost/test/unit_test.hpp>

#include "NeuralNetwork.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(NeuralNetworkTest)

BOOST_AUTO_TEST_CASE(getWeight_returns_correct_value_0_hidderlayer) {
	NeuralNetwork nn(0, 2, 2, 3, false);

	BOOST_CHECK_EQUAL(nn.getWeights()->size(), 3*3);
	BOOST_CHECK_EQUAL(nn.getWeightCount(), 3*3);
}

BOOST_AUTO_TEST_CASE(getWeight_returns_correct_value_1_hidderlayer) {
	NeuralNetwork nn(1, 2, 2, 3, false);

	BOOST_CHECK_EQUAL(nn.getWeights()->size(), 3*2 + 3*3);
	BOOST_CHECK_EQUAL(nn.getWeightCount(), 3*2 + 3*3);
}

BOOST_AUTO_TEST_CASE(getWeight_returns_correct_value_2_hidderlayer) {
	NeuralNetwork nn(2, 2, 2, 3, false);

	BOOST_CHECK_EQUAL(nn.getWeights()->size(), 3*2 + 3*2 + 3*3);
	BOOST_CHECK_EQUAL(nn.getWeightCount(), 3*2 + 3*2 + 3*3);
}

BOOST_AUTO_TEST_SUITE_END()

