#include <boost/test/unit_test.hpp>
#include "Line2.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(Line2Test)

BOOST_AUTO_TEST_CASE(intersects_intersecting_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {-1.f, 4.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 0.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_lines_intersecting_outside_first) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{3.f, 1.f}, {1.f, 3.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_lines_intersecting_outside_second) {
	Line2f line1{{3.f, 1.f}, {1.f, 3.f}};
	Line2f line2{{0.f, 0.f}, {1.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_lines_intersecting_outside_both) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{3.f, 0.f}, {2.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_parallel_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{0.1f, 0.f}, {1.1f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_coinciding_lines_far_from_each_other) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{2.f, 4.f}, {3.f, 6.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_overlapping_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{0.5f, 1.f}, {2.f, 4.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.x, 0.5f);
	BOOST_CHECK_LE(intersectionPoint.x, 1.0f);
	BOOST_CHECK_GE(intersectionPoint.y, 1.0f);
	BOOST_CHECK_LE(intersectionPoint.y, 2.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_full_overlapping_lines1) {
	Line2f line1{{0.f, 0.f}, {2.f, 4.f}};
	Line2f line2{{0.5f, 1.f}, {1.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.x, 0.5f);
	BOOST_CHECK_LE(intersectionPoint.x, 1.0f);
	BOOST_CHECK_GE(intersectionPoint.y, 1.0f);
	BOOST_CHECK_LE(intersectionPoint.y, 2.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_full_overlapping_lines2) {
	Line2f line1{{0.5f, 1.f}, {1.f, 2.f}};
	Line2f line2{{0.f, 0.f}, {2.f, 4.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.x, 0.5f);
	BOOST_CHECK_LE(intersectionPoint.x, 1.0f);
	BOOST_CHECK_GE(intersectionPoint.y, 1.0f);
	BOOST_CHECK_LE(intersectionPoint.y, 2.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_vertical_and_horizontal_lines_intersect) {
	Line2f line1{{-1.f, -1.f}, {1.f, -1.f}};
	Line2f line2{{0.f, 0.f}, {0.f, -2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x,  0.f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, -1.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_vertical_and_horizontal_lines_not_intersect) {
	Line2f line1{{-1.f, -1.f}, {1.f, -1.f}};
	Line2f line2{{2.f, 0.f}, {2.f, -2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_vertical_lines_parallel) {
	Line2f line1{{-1.f, -1.f}, {-1.f, 2.f}};
	Line2f line2{{-2.f, 0.f}, {-2.f, 3.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_vertical_lines_coincide_far_from_each_other) {
	Line2f line1{{-1.f, -1.f}, {-1.f, 2.f}};
	Line2f line2{{-1.f, -2.f}, {-1.f, -3.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_vertical_lines_overlap) {
	Line2f line1{{-1.f, -1.f}, {-1.f, 2.f}};
	Line2f line2{{-1.f, 3.f}, {-1.f, 0.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.y, 0.0f);
	BOOST_CHECK_LE(intersectionPoint.y, 2.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.x, -1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_vertical_lines_full_overlap1) {
	Line2f line1{{1.f, -1.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {1.f, 1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.y, 0.0f);
	BOOST_CHECK_LE(intersectionPoint.y, 1.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_vertical_lines_full_overlap2) {
	Line2f line1{{1.f, 0.f}, {1.f, 1.f}};
	Line2f line2{{1.f, -1.f}, {1.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.y, 0.0f);
	BOOST_CHECK_LE(intersectionPoint.y, 1.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_horizontal_lines_parallel) {
	Line2f line1{{-1.f, -1.f}, {2.f, -1.f}};
	Line2f line2{{0.f, -2.f}, {3.f, -2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_horizontal_lines_coincide_far_from_each_other) {
	Line2f line1{{-1.f, -1.f}, {2.f, -1.f}};
	Line2f line2{{-2.f, -1.f}, {-3.f, -1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersects(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersects_horizontal_lines_overlap) {
	Line2f line1{{-1.f, -1.f}, {2.f, -1.f}};
	Line2f line2{{3.f, -1.f}, {0.f, -1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.x, 0.0f);
	BOOST_CHECK_LE(intersectionPoint.x, 2.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.y, -1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_horizontal_lines_full_overlap1) {
	Line2f line1{{-1.f, 1.f}, {2.f, 1.f}};
	Line2f line2{{0.f, 1.f}, {1.f, 1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.x, 0.0f);
	BOOST_CHECK_LE(intersectionPoint.x, 1.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersects_horizontal_lines_full_overlap2) {
	Line2f line1{{0.f, 1.f}, {1.f, 1.f}};
	Line2f line2{{-1.f, 1.f}, {2.f, 1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersects(line1, line2, &intersectionPoint));
	BOOST_CHECK_GE(intersectionPoint.x, 0.0f);
	BOOST_CHECK_LE(intersectionPoint.x, 1.0f);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}



BOOST_AUTO_TEST_CASE(intersectsInfinite_intersecting_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {-1.f, 4.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 0.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_lines_intersecting_outside_first) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 4.f}, {2.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 3.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_lines_intersecting_outside_second) {
	Line2f line1{{1.f, 4.f}, {2.f, 2.f}};
	Line2f line2{{0.f, 0.f}, {1.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 3.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_lines_intersecting_outside_both) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{3.f, 0.f}, {2.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, 3.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_parallel_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{0.1f, 0.f}, {1.1f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersectsInfinite(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_coinciding_lines_far_from_each_other) {
	Line2f line1{{-1.f, -2.f}, {1.f, 2.f}};
	Line2f line2{{2.f, 4.f}, {3.f, 6.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_overlapping_lines) {
	Line2f line1{{-1.f, -2.f}, {1.f, 2.f}};
	Line2f line2{{0.5f, 1.f}, {2.f, 4.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_full_overlapping_lines1) {
	Line2f line1{{-1.f, -2.f}, {2.f, 4.f}};
	Line2f line2{{0.5f, 1.f}, {1.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_full_overlapping_lines2) {
	Line2f line1{{0.5f, 1.f}, {1.f, 2.f}};
	Line2f line2{{-1.f, -2.f}, {2.f, 4.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x / intersectionPoint.y, 0.5f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_and_horizontal_lines_intersect) {
	Line2f line1{{-1.f, -1.f}, {1.f, -1.f}};
	Line2f line2{{0.f, 0.f}, {0.f, -2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x,  0.f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, -1.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_and_horizontal_lines_intersect_outside) {
	Line2f line1{{-1.f, -1.f}, {1.f, -1.f}};
	Line2f line2{{2.f, 0.f}, {2.f, -2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x,  2.f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint.y, -1.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_lines_parallel) {
	Line2f line1{{-1.f, -1.f}, {-1.f, 2.f}};
	Line2f line2{{-2.f, 0.f}, {-2.f, 3.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersectsInfinite(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_lines_coincide_far_from_each_other) {
	Line2f line1{{-1.f, -1.f}, {-1.f, 2.f}};
	Line2f line2{{-1.f, -2.f}, {-1.f, -3.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x,  -1.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_lines_overlap) {
	Line2f line1{{-1.f, -1.f}, {-1.f, 2.f}};
	Line2f line2{{-1.f, 3.f}, {-1.f, 0.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, -1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_lines_full_overlap1) {
	Line2f line1{{1.f, -1.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {1.f, 1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_vertical_lines_full_overlap2) {
	Line2f line1{{1.f, 0.f}, {1.f, 1.f}};
	Line2f line2{{1.f, -1.f}, {1.f, 2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.x, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_horizontal_lines_parallel) {
	Line2f line1{{-1.f, -1.f}, {2.f, -1.f}};
	Line2f line2{{0.f, -2.f}, {3.f, -2.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(!intersectsInfinite(line1, line2, &intersectionPoint));
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_horizontal_lines_coincide_far_from_each_other) {
	Line2f line1{{-1.f, -1.f}, {2.f, -1.f}};
	Line2f line2{{-2.f, -1.f}, {-3.f, -1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.y, -1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_horizontal_lines_overlap) {
	Line2f line1{{-1.f, -1.f}, {2.f, -1.f}};
	Line2f line2{{3.f, -1.f}, {0.f, -1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.y, -1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_horizontal_lines_full_overlap1) {
	Line2f line1{{-1.f, 1.f}, {2.f, 1.f}};
	Line2f line2{{0.f, 1.f}, {1.f, 1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersectsInfinite_horizontal_lines_full_overlap2) {
	Line2f line1{{0.f, 1.f}, {1.f, 1.f}};
	Line2f line2{{-1.f, 1.f}, {2.f, 1.f}};

	sf::Vector2f intersectionPoint;
	BOOST_REQUIRE(intersectsInfinite(line1, line2, &intersectionPoint));
	BOOST_CHECK_CLOSE(intersectionPoint.y, 1.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(is_parallel_not_parallel_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {-1.f, 4.f}};

	BOOST_CHECK(!isParallel(line1, line2));
}

BOOST_AUTO_TEST_CASE(is_parallel_parallel_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{-1.f, -1.f}, {1.f, 3.f}};

	BOOST_CHECK(isParallel(line1, line2));
}

BOOST_AUTO_TEST_CASE(is_parallel_coinciding_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{-1.f, -2.f}, {-4.f, -8.f}};

	BOOST_CHECK(isParallel(line1, line2));
}

BOOST_AUTO_TEST_CASE(is_parallel_horizontal_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 0.f}};
	Line2f line2{{-1.f, -2.f}, {-4.f, -2.f}};

	BOOST_CHECK(isParallel(line1, line2));
}

BOOST_AUTO_TEST_CASE(is_parallel_vertical_lines) {
	Line2f line1{{0.f, 0.f}, {0.f, -3.f}};
	Line2f line2{{-1.f, 2.f}, {-1.f, -2.f}};

	BOOST_CHECK(isParallel(line1, line2));
}

BOOST_AUTO_TEST_CASE(is_parallel_horizontal_and_vertical_lines) {
	Line2f line1{{0.f, 0.f}, {0.f, -3.f}};
	Line2f line2{{-1.f, 2.f}, {1.f, 2.f}};

	BOOST_CHECK(!isParallel(line1, line2));
}



BOOST_AUTO_TEST_SUITE_END()


