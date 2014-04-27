
#include <boost/test/unit_test.hpp>
#include "mathUtil.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(mathUtilTest)

BOOST_AUTO_TEST_CASE(resizeToEnclose_square_square) {
	sf::FloatRect rect{3, 4, 5, 5};

	sf::FloatRect square = resizeToEnclose(rect, 1.f);

	BOOST_CHECK_CLOSE(square.left, 3.f, 0.001);
	BOOST_CHECK_CLOSE(square.top, 4.f, 0.001);
	BOOST_CHECK_CLOSE(square.width, 5.f, 0.001);
	BOOST_CHECK_CLOSE(square.height, 5.f, 0.001);
}

BOOST_AUTO_TEST_CASE(resizeToEnclose_square_wide) {
	sf::FloatRect rect{3, 4, 7, 5};

	sf::FloatRect square = resizeToEnclose(rect, 1.f);

	BOOST_CHECK_CLOSE(square.left, 3.f, 0.001);
	BOOST_CHECK_CLOSE(square.top, 3.f, 0.001);
	BOOST_CHECK_CLOSE(square.width, 7.f, 0.001);
	BOOST_CHECK_CLOSE(square.height, 7.f, 0.001);
}

BOOST_AUTO_TEST_CASE(resizeToEnclose_square_tall) {
	sf::FloatRect rect{3, 4, 5, 7};

	sf::FloatRect square = resizeToEnclose(rect, 1.f);

	BOOST_CHECK_CLOSE(square.left, 2.f, 0.001);
	BOOST_CHECK_CLOSE(square.top, 4.f, 0.001);
	BOOST_CHECK_CLOSE(square.width, 7.f, 0.001);
	BOOST_CHECK_CLOSE(square.height, 7.f, 0.001);
}

BOOST_AUTO_TEST_CASE(addToBoundingBox_point_inside) {
	sf::FloatRect rect{3, 4, 5, 7};

	addToBoundingBox(rect, sf::Vector2f{4, 5});

	BOOST_CHECK_CLOSE(rect.left, 3.f, 0.001);
	BOOST_CHECK_CLOSE(rect.top, 4.f, 0.001);
	BOOST_CHECK_CLOSE(rect.width, 5.f, 0.001);
	BOOST_CHECK_CLOSE(rect.height, 7.f, 0.001);
}

BOOST_AUTO_TEST_CASE(addToBoundingBox_point_left) {
	sf::FloatRect rect{3, 4, 5, 7};

	addToBoundingBox(rect, sf::Vector2f{2, 5});

	BOOST_CHECK_CLOSE(rect.left, 2.f, 0.001);
	BOOST_CHECK_CLOSE(rect.top, 4.f, 0.001);
	BOOST_CHECK_CLOSE(rect.width, 6.f, 0.001);
	BOOST_CHECK_CLOSE(rect.height, 7.f, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

