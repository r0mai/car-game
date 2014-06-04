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

BOOST_AUTO_TEST_CASE(coincident_not_overlapping_lines) {
	Line2f line1{{1.f, 0.f}, {2.f, 2.f}};
	Line2f line2{{3.f, 4.f}, {4.f, 6.f}};

	LineIntersection<float> intersection{line1, line2};

	BOOST_CHECK(intersection.isParallel());
	BOOST_CHECK(intersection.isCoincident());
	BOOST_CHECK(!intersection.getOverlapLine());
	BOOST_CHECK(!intersection.getIntersectionPoint());
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_end_line2_start) {
	Line2f line1{{1.f, 0.f}, {4.f, 6.f}};
	Line2f line2{{3.f, 4.f}, {5.f, 8.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_end_line2_end) {
	Line2f line1{{1.f, 0.f}, {4.f, 6.f}};
	Line2f line2{{5.f, 8.f}, {3.f, 4.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_start_line2_start) {
	Line2f line1{{4.f, 6.f}, {1.f, 0.f}};
	Line2f line2{{3.f, 4.f}, {5.f, 8.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_start_line2_end) {
	Line2f line1{{4.f, 6.f}, {1.f, 0.f}};
	Line2f line2{{5.f, 8.f}, {3.f, 4.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_line1_within_line2) {
	Line2f line1{{3.f, 4.f}, {4.f, 6.f}};
	Line2f line2{{1.f, 0.f}, {5.f, 8.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_line2_within_line1) {
	Line2f line1{{1.f, 0.f}, {5.f, 8.f}};
	Line2f line2{{3.f, 4.f}, {4.f, 6.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_end_line2_start_reverse) {
	Line2f line1{{1.f, 0.f}, {4.f, 6.f}};
	Line2f line2{{5.f, 8.f}, {3.f, 4.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_end_line2_end_reverse) {
	Line2f line1{{1.f, 0.f}, {4.f, 6.f}};
	Line2f line2{{3.f, 4.f}, {5.f, 8.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_start_line2_start_reverse) {
	Line2f line1{{4.f, 6.f}, {1.f, 0.f}};
	Line2f line2{{5.f, 8.f}, {3.f, 4.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_overlap_at_line1_start_line2_end_reverse) {
	Line2f line1{{4.f, 6.f}, {1.f, 0.f}};
	Line2f line2{{3.f, 4.f}, {5.f, 8.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_line1_within_line2_reverse) {
	Line2f line1{{3.f, 4.f}, {4.f, 6.f}};
	Line2f line2{{5.f, 8.f}, {1.f, 0.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}

BOOST_AUTO_TEST_CASE(coincident_lines_line2_within_line1_reverse) {
	Line2f line1{{1.f, 0.f}, {5.f, 8.f}};
	Line2f line2{{4.f, 6.f}, {3.f, 4.f}};

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
	BOOST_CHECK_CLOSE(overlapLine->start.x, 3, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->start.y, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.x, 4, 0.001);
	BOOST_CHECK_CLOSE(overlapLine->end.y, 6, 0.001);
}


BOOST_AUTO_TEST_SUITE_END()

