#include "RandomTrackGenerator.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <algorithm>
#include "Track.hpp"
#include "mathUtil.hpp"

namespace car {

namespace {

std::size_t findNearestLine(const std::vector<sf::Vector2f>& points, const sf::Vector2f& referencePoint) {
	std::size_t n = points.size();

	std::vector<float> distanceSquares(n);

	for (std::size_t i = 0; i < n; ++i) {
		std::size_t j = (i + 1) % n;

		auto nearest = nearestPoint(referencePoint, {points[i], points[j]});
		distanceSquares[i] = getDistanceSQ(referencePoint, nearest);
	}

	return std::min_element(distanceSquares.begin(), distanceSquares.end()) - distanceSquares.begin();
}

}

Track RandomTrackGenerator::operator()(uint seed) const {
	boost::random::mt19937 rng{seed};
	for (int i = 0; i < maxTries; ++i) {
		Track track = generateTrack(rng);
		try {
			track.check();
		} catch (TrackError&) {
			continue;
		}

		return track;
	}

	throw RandomTrackException{"Track generation failed after maximum number of tries"};
}

Track RandomTrackGenerator::generateTrack(boost::random::mt19937& rng) const {


	sf::Vector2f startEdge1 = corner1 * 0.2f + corner2 * 0.8f;
	sf::Vector2f startEdge2 = corner2 * 0.2f + corner1 * 0.8f;

	std::vector<sf::Vector2f> points{
		startEdge1, {startEdge2.x, startEdge1.y},
		startEdge2, {startEdge1.x, startEdge2.y}
	};

	using Distribution = boost::random::uniform_real_distribution<float>;
	Distribution distX{std::min(corner1.x, corner2.x), std::max(corner1.x, corner2.x)};
	Distribution distY{std::min(corner1.y, corner2.y), std::max(corner1.y, corner2.y)};

	for (int i = 4; i < numberOfPoints; ++i) {
		sf::Vector2f newPoint{distX(rng), distY(rng)};

		int index = findNearestLine(points, newPoint) + 1;

		points.insert(points.begin() + index, newPoint);
	}

	Distribution distWidth{minPathWidth, maxPathWidth};

	return generator(distWidth(rng), points);
}

} /* namespace car */
