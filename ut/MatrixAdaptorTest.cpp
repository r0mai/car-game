#include <boost/test/unit_test.hpp>
#include "MatrixAdaptor.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(Line2Test)

BOOST_AUTO_TEST_CASE(point_inside_range) {
	std::size_t width = 3, height = 5;
	MatrixAdaptor m{width, height};
	std::size_t x = 2, y=1;
	BOOST_CHECK_EQUAL((m[{x,y}]), width * y + x);
}

BOOST_AUTO_TEST_CASE(too_big_x) {
	std::size_t width = 3, height = 5;
	MatrixAdaptor m{width, height};
	std::size_t x = 4, y = 1;
	BOOST_CHECK_EQUAL((m[{x,y}]), MatrixAdaptor::outsideRange());
}

BOOST_AUTO_TEST_CASE(too_big_y) {
	std::size_t width = 7, height = 5;
	MatrixAdaptor m{width, height};
	std::size_t x = 1, y = 6;
	BOOST_CHECK_EQUAL((m[{x,y}]), MatrixAdaptor::outsideRange());
}

BOOST_AUTO_TEST_SUITE_END()
