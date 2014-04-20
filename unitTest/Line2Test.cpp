#include <boost/test/unit_test.hpp>
#include "Line2.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(Line2Test)

BOOST_AUTO_TEST_CASE(intersects_intersecting_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {0.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 0.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()


