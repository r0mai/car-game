#include "PolygonTrackBuilder.hpp"
#include "mathUtil.hpp"
#include "Line2.hpp"

namespace car {

namespace {

void closeEdges(std::vector<Line2f>& edge) {
	for (std::size_t i = 0; i < edge.size(); ++i) {
		auto& line1 = edge[i];
		auto& line2 = edge[(i + 1) % edge.size()];

		if (!isParallel(line1, line2)) {
			sf::Vector2f intersectionPoint;
			intersectsInfinite(line1, line2, &intersectionPoint);

			line1.end = intersectionPoint;
			line2.start = intersectionPoint;
		}
	}
}

}

Track PolygonTrackBuilder::operator()(const std::vector<sf::Vector2f>& points) {
	const float distance = trackWidth / 2.f;

	std::vector<Line2f> rightEdge;
	std::vector<Line2f> leftEdge;
	rightEdge.reserve(points.size());
	leftEdge.reserve(points.size());

	for (std::size_t i = 0; i < points.size(); ++i) {
		const auto& point1 = points[i];
		const auto& point2 = points[(i + 1) % points.size()];

		// rotate direction 90 degrees and normalize it
		auto direction = normalize(sf::Vector2f{point2.y - point1.y, point1.x - point2.x});
		auto shift = direction * distance;

		rightEdge.push_back({point1 + shift, point2 + shift});
		leftEdge.push_back({point1 - shift, point2 - shift});
	}

	closeEdges(rightEdge);
	closeEdges(leftEdge);

	Track track;

	for (const auto& line: rightEdge) {
		track.addLine(line);
	}

	for (const auto& line: leftEdge) {
		track.addLine(line);
	}

	return track;
}

} /* namespace car */
