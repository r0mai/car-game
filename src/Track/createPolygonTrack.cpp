#include "createPolygonTrack.hpp"
#include "mathUtil.hpp"
#include "Line2.hpp"
#include <iostream>
#include <boost/range/algorithm.hpp>

namespace car { namespace track {

namespace {

std::vector<Line2f> closeEdges(std::vector<Line2f>& edge) {
	std::vector<Line2f> output;
	output.reserve(edge.size());
	for (std::size_t i = 0; i < edge.size(); ++i) {
		auto& line1 = edge[i];
		auto& line2 = edge[(i + 1) % edge.size()];

		sf::Vector2f intersectionPoint;
		if (intersects(line1, line2, &intersectionPoint)) {
			line1.end = intersectionPoint;
			line2.start = intersectionPoint;
		} else {
			output.push_back({line1.end, line2.start});
		}
	}
	return output;
}

void createEdges(std::vector<Line2f>& rightEdge, std::vector<Line2f>& leftEdge,
		float trackWidth, const std::vector<sf::Vector2f>& points) {
	const float distance = trackWidth / 2.f;

	for (std::size_t i = 0; i < points.size(); ++i) {
		const auto& point1 = points[i];
		const auto& point2 = points[(i + 1) % points.size()];

		// rotate direction 90 degrees and normalize it
		auto roadVector = point2 - point1;
		auto shiftDirection = normalize(rotateClockwise(roadVector));
		auto shift = shiftDirection * distance;

		rightEdge.push_back({point1 + shift, point2 + shift});
		leftEdge.push_back({point1 - shift, point2 - shift});
	}
}

Track createTrack(std::vector<Line2f>& rightEdge, std::vector<Line2f>& leftEdge,
		float checkpointDistance) {
	const float tooClose = 1.0f;
	Track track;

	for (std::size_t i = 0; i < rightEdge.size(); ++i) {
		const auto& line1 = rightEdge[i];
		const auto& line2 = leftEdge[i];

		track.addLine(line1);
		track.addLine(line2);

		auto roadVector = line1.end - line1.start;
		auto roadDirection = normalize(roadVector);
		auto checkpointDirection = rotateCounterclockwise(roadDirection);
		auto length = getLength(roadVector);

		track.addCheckpoint({line1.start, line2.start});

		for (float position = 0.f; position < length; position += checkpointDistance) {
			auto base = line1.start + roadDirection * position;
			sf::Vector2f intersectionPoint;

			if (intersectsRay(line2, base, checkpointDirection, &intersectionPoint) &&
					!(equals(intersectionPoint.x, line2.start.x, tooClose) &&
					equals(intersectionPoint.y, line2.start.y, tooClose)) &&
					!(equals(intersectionPoint.x, line2.end.x, tooClose) &&
					equals(intersectionPoint.y, line2.end.y, tooClose))) {
				track.addCheckpoint({base, intersectionPoint});
			}
		}

		track.addCheckpoint({line1.end, line2.end});
	}

	return track;
}

}

Track createPolygonTrack(float checkpointDistance, float trackWidth, const std::vector<sf::Vector2f>& points) {
	std::vector<Line2f> rightEdge;
	std::vector<Line2f> leftEdge;
	rightEdge.reserve(points.size());
	leftEdge.reserve(points.size());

	createEdges(rightEdge, leftEdge, trackWidth, points);
	auto additions = closeEdges(rightEdge);
	boost::copy(closeEdges(leftEdge), std::back_inserter(additions));
	auto track = createTrack(rightEdge, leftEdge, checkpointDistance);

	auto roadVector = points[1] - points[0];
	track.setOrigin(points[0], std::atan2(roadVector.y, roadVector.x));

	for (const auto& line: additions) {
		track.addLine(line);
	}

	return track;
}

}} /* namespace car::track */
