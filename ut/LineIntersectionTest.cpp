#include <boost/test/unit_test.hpp>
#include "LineIntersection.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(LineIntersectionTest)

BOOST_AUTO_TEST_CASE(intersect_between_lines) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{1.f, 0.f}, {-1.f, 4.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 0.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 1.0f, 0.001);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine1(), 0.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine1(), 1.f);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine2(), 0.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine2(), 1.f);
}

BOOST_AUTO_TEST_CASE(intersect_after_first_line_in_second_line) {
	Line2f line1{{0.f, 0.f}, {1.f, 2.f}};
	Line2f line2{{2.f, 2.f}, {1.f, 4.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 3.0f, 0.001);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine1(), 1.f);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine2(), 0.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine2(), 1.f);
}

BOOST_AUTO_TEST_CASE(intersect_in_first_line_after_second_line) {
	Line2f line1{{2.f, 2.f}, {1.f, 4.f}};
	Line2f line2{{0.f, 0.f}, {1.f, 2.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 3.0f, 0.001);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine1(), 0.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine1(), 1.f);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine2(), 1.f);
}

BOOST_AUTO_TEST_CASE(intersect_before_first_line_in_second_line) {
	Line2f line1{{1.f, 2.f}, {0.f, 0.f}};
	Line2f line2{{2.f, 2.f}, {1.f, 4.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 3.0f, 0.001);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine1(), 0.f);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine2(), 0.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine2(), 1.f);
}

BOOST_AUTO_TEST_CASE(intersect_in_first_line_before_second_line) {
	Line2f line1{{2.f, 2.f}, {1.f, 4.f}};
	Line2f line2{{1.f, 2.f}, {0.f, 0.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 1.5f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 3.0f, 0.001);
	BOOST_CHECK_GT(intersection.getIntersectionPointRatioLine1(), 0.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine1(), 1.f);
	BOOST_CHECK_LT(intersection.getIntersectionPointRatioLine2(), 0.f);
}

BOOST_AUTO_TEST_CASE(intersect_at_line_start_points) {
	Line2f line1{{2.f, 3.f}, {5.f, -1.f}};
	Line2f line2{{2.f, 3.f}, {-2.f, 5.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 2.0f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 3.0f, 0.001);
	BOOST_CHECK_CLOSE(intersection.getIntersectionPointRatioLine1(), 0.f, 0.001);
	BOOST_CHECK_CLOSE(intersection.getIntersectionPointRatioLine2(), 0.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersect_at_line_end_points) {
	Line2f line1{{1.f, -2.f}, {4.f, 0.f}};
	Line2f line2{{0.f, 2.f}, {4.f, 0.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x, 4.0f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, 0.0f, 0.001);
	BOOST_CHECK_CLOSE(intersection.getIntersectionPointRatioLine1(), 1.f, 0.001);
	BOOST_CHECK_CLOSE(intersection.getIntersectionPointRatioLine2(), 1.f, 0.001);
}

BOOST_AUTO_TEST_CASE(intersect_at_first_line_start_second_line_end) {
	Line2f line1{{1.f, -2.f}, {4.f, 0.f}};
	Line2f line2{{0.f, 2.f}, {1.f, -2.f}};

	LineIntersection<float> intersection{line1, line2};
	auto intersectionPoint = intersection.getIntersectionPoint();

	BOOST_CHECK(!intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_REQUIRE(intersectionPoint);
	BOOST_CHECK_CLOSE(intersectionPoint->x,  1.0f, 0.001);
	BOOST_CHECK_CLOSE(intersectionPoint->y, -2.0f, 0.001);
	BOOST_CHECK_CLOSE(intersection.getIntersectionPointRatioLine1(), 0.f, 0.001);
	BOOST_CHECK_CLOSE(intersection.getIntersectionPointRatioLine2(), 1.f, 0.001);
}

BOOST_AUTO_TEST_CASE(parallel_lines) {
	Line2f line1{{1.f, 2.f}, {0.f, 0.f}};
	Line2f line2{{2.f, 2.f}, {1.f, 0.f}};

	LineIntersection<float> intersection{line1, line2};

	BOOST_CHECK(intersection.isParallel());
	BOOST_CHECK(!intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_CHECK(!intersection.getIntersectionPoint());
}

struct CoincidentLinesFixture {
	sf::Vector2f p1{1.f, 0.f};
	sf::Vector2f p2{3.f, 4.f};
	sf::Vector2f p3{4.f, 6.f};
	sf::Vector2f p4{5.f, 8.f};

	void checkIntersection(const Line2f& line1, const Line2f& line2) {
		LineIntersection<float> intersection{line1, line2};

		BOOST_CHECK(intersection.isParallel());
		BOOST_CHECK(intersection.isCoincident());
		BOOST_CHECK(!intersection.getIntersectionPoint());
		auto overlapLine = intersection.getOverlapLine();
		BOOST_REQUIRE(overlapLine);
		// We don't care which point is the start and which is the end.
		if (overlapLine->start.x > overlapLine->end.x) {
			std::swap(overlapLine->start, overlapLine->end);
		}
		BOOST_CHECK_CLOSE(overlapLine->start.x, p2.x, 0.001);
		BOOST_CHECK_CLOSE(overlapLine->start.y, p2.y, 0.001);
		BOOST_CHECK_CLOSE(overlapLine->end.x, p3.x, 0.001);
		BOOST_CHECK_CLOSE(overlapLine->end.y, p3.y, 0.001);
	}

	void checkTouch(const Line2f& line1, const Line2f& line2) {
		LineIntersection<float> intersection{line1, line2};

		BOOST_CHECK(intersection.isParallel());
		BOOST_CHECK(intersection.isCoincident());
		BOOST_CHECK(!intersection.getIntersectionPoint());
		auto overlapLine = intersection.getOverlapLine();
		BOOST_REQUIRE(overlapLine);
		BOOST_CHECK_CLOSE(overlapLine->start.x, p2.x, 0.001);
		BOOST_CHECK_CLOSE(overlapLine->start.y, p2.y, 0.001);
		BOOST_CHECK_CLOSE(overlapLine->end.x, p2.x, 0.001);
		BOOST_CHECK_CLOSE(overlapLine->end.y, p2.y, 0.001);
	}
};

BOOST_FIXTURE_TEST_CASE(coincident_not_overlapping_lines,
		CoincidentLinesFixture) {
	Line2f line1{p1, p2};
	Line2f line2{p3, p4};

	LineIntersection<float> intersection{line1, line2};

	BOOST_CHECK(intersection.isParallel());
	BOOST_CHECK(intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_CHECK(!intersection.getIntersectionPoint());
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_overlap_at_line1_end_line2_start,
		CoincidentLinesFixture) {
	checkIntersection({p1, p3}, {p2, p4});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_overlap_at_line1_end_line2_end,
		CoincidentLinesFixture) {
	checkIntersection({p1, p3}, {p4, p2});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_overlap_at_line1_start_line2_start,
		CoincidentLinesFixture) {
	checkIntersection({p3, p1}, {p2, p4});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_overlap_at_line1_start_line2_end,
		CoincidentLinesFixture) {
	checkIntersection({p3, p1}, {p4, p2});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_line1_within_line2,
		CoincidentLinesFixture) {
	checkIntersection({p2, p3}, {p1, p4});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_line2_within_line1,
		CoincidentLinesFixture) {
	checkIntersection({p1, p4}, {p2, p3});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_line1_reversed_within_line2,
		CoincidentLinesFixture) {
	checkIntersection({p3, p2}, {p1, p4});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_line2_within_line1_reversed,
		CoincidentLinesFixture) {
	checkIntersection({p4, p1}, {p2, p3});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_line1_within_line2_reversed,
		CoincidentLinesFixture) {
	checkIntersection({p2, p3}, {p4, p1});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_line2_reversed_within_line1,
		CoincidentLinesFixture) {
	checkIntersection({p1, p4}, {p3, p2});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_touch_at_line1_end_line2_start,
		CoincidentLinesFixture) {
	checkTouch({p1, p2}, {p2, p3});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_touch_at_line1_start_line2_start,
		CoincidentLinesFixture) {
	checkTouch({p2, p1}, {p2, p3});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_touch_at_line1_end_line2_end,
		CoincidentLinesFixture) {
	checkTouch({p1, p2}, {p3, p2});
}

BOOST_FIXTURE_TEST_CASE(coincident_lines_touch_at_line1_start_line2_end,
		CoincidentLinesFixture) {
	checkTouch({p2, p1}, {p3, p2});
}

BOOST_AUTO_TEST_SUITE_END()

