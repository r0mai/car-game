
#include <boost/test/unit_test.hpp>

#include "Line2.hpp"

BOOST_AUTO_TEST_SUITE( LineTest )

using namespace car;

BOOST_AUTO_TEST_CASE(intersectWithLine_test) {
	Line2f line1(-5, 0, 10, 0);
	Line2f line2(2, -5, 2, 11);

	sf::Vector2f out1;
	BOOST_REQUIRE(line1.intersectWithLine(line2, true, &out1));
	BOOST_CHECK_CLOSE(out1.x, 2.f, 0.001);
	BOOST_CHECK_CLOSE(out1.y, 0.f, 0.001);

	sf::Vector2f out2;
	BOOST_REQUIRE(line2.intersectWithLine(line1, true, &out2));
	BOOST_CHECK_CLOSE(out2.x, 2.f, 0.001);
	BOOST_CHECK_CLOSE(out2.y, 0.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectWithRay_test_1) {
	Line2f line(-5, 0, 10, 0);
	sf::Vector2f origin(2, 2);
	sf::Vector2f direction(1, -1);

	sf::Vector2f out;
	BOOST_REQUIRE(line.intersectWithRay(origin, direction, &out));
	BOOST_CHECK_CLOSE(out.x, 4.f, 0.001);
	BOOST_CHECK_CLOSE(out.y, 0.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectWithRay_test_2) {
	Line2f line(-5, 0, 3, 0);
	sf::Vector2f origin(2, 2);
	sf::Vector2f direction(1, -1);

	sf::Vector2f out;
	BOOST_CHECK(!line.intersectWithRay(origin, direction, &out));
}

BOOST_AUTO_TEST_CASE(intersectWithRay_test_3) {
	Line2f line(-5, 0, 10, 0);
	sf::Vector2f origin(2, 2);
	sf::Vector2f direction(-1, 1);

	sf::Vector2f out;
	BOOST_CHECK(!line.intersectWithRay(origin, direction, &out));
}

BOOST_AUTO_TEST_SUITE_END()

