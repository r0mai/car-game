#include <boost/test/unit_test.hpp>
#include "MatrixAdaptor.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(Line2Test)

BOOST_AUTO_TEST_CASE(point_inside_range) {
	std::size_t width = 3, height = 5;
	MatrixAdaptor m{width, height};
	MatrixCoordinate p{1, 2};
	BOOST_CHECK_LT(m.positionFromCoordinate(p), width*height);
}

BOOST_AUTO_TEST_CASE(too_big_x) {
	std::size_t width = 3, height = 5;
	MatrixAdaptor m{width, height};
	MatrixCoordinate p{4, 1};
	BOOST_CHECK_EQUAL(m.positionFromCoordinate(p), MatrixAdaptor::outsideRange());
}

BOOST_AUTO_TEST_CASE(too_big_y) {
	std::size_t width = 7, height = 5;
	MatrixAdaptor m{width, height};
	MatrixCoordinate p{1, 6};
	BOOST_CHECK_EQUAL(m.positionFromCoordinate(p), MatrixAdaptor::outsideRange());
}

BOOST_AUTO_TEST_CASE(consistent_conversion_from_coordinate_to_position_and_back) {
	std::size_t width = 7, height = 5;
	MatrixAdaptor m{width, height};
	MatrixCoordinate p{3, 4};
	auto result = m.coordinateFromPosition(m.positionFromCoordinate(p));
	BOOST_CHECK_EQUAL(result.x, p.x);
	BOOST_CHECK_EQUAL(result.y, p.y);
}

BOOST_AUTO_TEST_CASE(coordinateFromPosition_inside_range) {
	std::size_t width = 3, height = 2;
	MatrixAdaptor m{width, height};
	std::size_t coordinate = m.size() - 1;
	auto position = m.coordinateFromPosition(coordinate);
	BOOST_CHECK_NE(position.x, MatrixAdaptor::outsideRange());
	BOOST_CHECK_NE(position.y, MatrixAdaptor::outsideRange());
}

BOOST_AUTO_TEST_CASE(coordinateFromPosition_outside_range) {
	std::size_t width = 3, height = 2;
	MatrixAdaptor m{width, height};
	std::size_t coordinate = m.size() + 1;
	auto position = m.coordinateFromPosition(coordinate);
	BOOST_CHECK_EQUAL(position.x, MatrixAdaptor::outsideRange());
	BOOST_CHECK_EQUAL(position.y, MatrixAdaptor::outsideRange());
}

BOOST_AUTO_TEST_CASE(consistent_conversion_from_position_to_coordinate_and_back) {
	std::size_t width = 5, height = 4;
	MatrixAdaptor m{width, height};
	std::size_t coordinate = 6;
	BOOST_CHECK_EQUAL(m.positionFromCoordinate(m.coordinateFromPosition(coordinate)), coordinate);
}

BOOST_AUTO_TEST_SUITE_END()
