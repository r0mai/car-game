
#include <boost/math/constants/constants.hpp>

#include "Track.hpp"
#include "Car.hpp"
#include "drawUtil.hpp"
#include "mathUtil.hpp"
#include "PolygonTrackBuilder.hpp"

namespace car {

Track createCircleTrack() {

	using namespace boost::math::float_constants;

	Track track;
	const int circleResolution = 60;
	const int numberOfCheckpoints = 72;
	const float innerCircleRadius = 50.;
	const float outerCircleRadius = 60.;
	for ( int i = 0; i < circleResolution; ++i ) {
		track.addLine(Line2f(
					innerCircleRadius*std::cos((i-1)*2*pi/circleResolution),
					innerCircleRadius*std::sin((i-1)*2*pi/circleResolution),
					innerCircleRadius*std::cos((i)*2*pi/circleResolution),
					innerCircleRadius*std::sin((i)*2*pi/circleResolution)));
		track.addLine(Line2f(
					outerCircleRadius*std::cos((i-1)*2*pi/circleResolution),
					outerCircleRadius*std::sin((i-1)*2*pi/circleResolution),
					outerCircleRadius*std::cos((i)*2*pi/circleResolution),
					outerCircleRadius*std::sin((i)*2*pi/circleResolution)));
	}
	for ( int i = 0; i < numberOfCheckpoints; ++i ) {
		track.addCheckpoint(Line2f(
				innerCircleRadius*std::cos((i)*2*pi/numberOfCheckpoints),
				-innerCircleRadius*std::sin((i)*2*pi/numberOfCheckpoints),
				outerCircleRadius*std::cos((i)*2*pi/numberOfCheckpoints),
				-outerCircleRadius*std::sin((i)*2*pi/numberOfCheckpoints)
			));
	}

	track.setOrigin({0.f, 55.f}, 0.f);

	return track;
}

Track createZigZagTrack() {
	std::vector<sf::Vector2f> points;

	points.push_back({-55.f, 55.f});
	points.push_back({ 55.f, 55.f});
	points.push_back({-10.f,   0.f});
	points.push_back({ 55.f, -55.f});
	points.push_back({-55.f, -55.f});

	PolygonTrackBuilder builder{5.f};
	return builder(10.f, points);
}

Track createCurvyTrack() {
	std::vector<sf::Vector2f> points;

	points.push_back({-55.f, 45.f});
	points.push_back({-45.f, 55.f});
	points.push_back({-30.f, 55.f});
	points.push_back({-15.f, 35.f});
	points.push_back({15.f, 35.f});
	points.push_back({45.f, 55.f});
	points.push_back({55.f, 45.f});
	points.push_back({55.f, -10.f});
	points.push_back({45.f, -45.f});
	points.push_back({-45.f, -45.f});

	PolygonTrackBuilder builder{5.f};
	return builder(5.f, points);
}

Car Track::createCar() const {
	return Car{startingPoint, startingDirection};
}

void Track::setOrigin(const sf::Vector2f& point, float direction) {
	startingPoint = point;
	startingDirection = direction;
}


void Track::addLine(const Line2f& line) {
	lines.push_back(line);
}

void Track::addCheckpoint(const Line2f& line) {
	checkpoints.push_back(line);
}


bool Track::collidesWith(const Line2f& line) const {
	for ( const Line2f& trackLine : lines ) {
		if ( intersects(line, trackLine) ) {
			return true;
		}
	}
	return false;
}

boost::optional<sf::Vector2f> Track::collideWithRay(const sf::Vector2f& origin, const sf::Vector2f& direction) const {
	float minimumDistanceSQ = -1.f; //negative distance means, we haven't found an intersecting line
	sf::Vector2f closest;
	for ( const Line2f& trackLine : lines ) {
		sf::Vector2f out;
		if ( intersectsRay(trackLine, origin, direction, &out) ) {
			float distanceSQ = getDistanceSQ(origin, out);
			if ( minimumDistanceSQ < 0.f || distanceSQ < minimumDistanceSQ ) {
				minimumDistanceSQ = distanceSQ;
				closest = out;
			}
		}
	}
	if ( minimumDistanceSQ < 0.f ) {
		return boost::none;
	}
	return closest;
}

int Track::checkpointCollidesWith(const Line2f& line) const {
	for ( std::size_t i = 0; i < checkpoints.size(); ++i ) {
		if ( intersects(line, checkpoints[i]) ) {
			return i;
		}
	}
	return -1;
}

std::size_t Track::getNumberOfCheckpoints() const {
	return checkpoints.size();
}

const Line2f& Track::getCheckpoint(std::size_t n) const {
	return checkpoints[n];
}

void Track::draw(sf::RenderWindow& window, int highlightCheckpoint) const {
	for ( const Line2f& trackLine : lines ) {
		drawLine(window, trackLine);
	}

	sf::Color checkpointColor{255, 255, 64};
	sf::Color highlightedCheckpointColor{128, 128, 255};

	for ( std::size_t i = 0; i < checkpoints.size(); ++i ) {

		drawLine(window, checkpoints[i],
				((static_cast<int>(i) == highlightCheckpoint) ?
						checkpointColor : highlightedCheckpointColor));
	}
}

sf::FloatRect Track::getDimensions() const {
	if (lines.empty()) {
		return sf::FloatRect{};
	}

	sf::FloatRect result{lines[0].start, sf::Vector2f{0, 0}};
	for (const Line2f& line : lines) {
		addToBoundingBox(result, line.start);
		addToBoundingBox(result, line.end);
	}
	return result;
}

namespace {

struct CheckedLine {
	bool start = false;
	bool end = false;
};

bool checkLineEndpoint(const sf::Vector2f& endpoint,
		const sf::Vector2f& intersection, float toleranceSquare,
		bool& alreadyChecked) {
	if (alreadyChecked ||
			getLengthSQ(sf::Vector2f{endpoint.x - intersection.x,
					endpoint.y - intersection.y}) > toleranceSquare) {
		return false;
	}

	alreadyChecked = true;
	return true;
}

}

void Track::check() const
{
	const float toleranceSquare = 0.0001;
	std::vector<CheckedLine> checkedLines(lines.size());

	for ( std::size_t i = 0; i < lines.size(); ++i ) {

		if (getLengthSQ(lines[i].start - lines[i].end) < toleranceSquare * 4) {
			throw TrackError{"Line segment too short"};
		}

		for ( std::size_t j = i + 1; j < lines.size(); ++j ) {
			sf::Vector2f p;
			if (intersects(lines[i], lines[j], &p)) {
				if (!(
						(checkLineEndpoint(lines[i].start, p, toleranceSquare,
								checkedLines[i].start) ||
						checkLineEndpoint(lines[i].end, p, toleranceSquare,
								checkedLines[i].end)) &&
						(checkLineEndpoint(lines[j].start, p, toleranceSquare,
								checkedLines[j].start) ||
						checkLineEndpoint(lines[j].end, p, toleranceSquare,
								checkedLines[j].end))
					)) {
					throw TrackError{"The track intersects with itself"};
				}
			}
		}
	}
}

}

