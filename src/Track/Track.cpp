
#include "Track.hpp"
#include "Car.hpp"
#include "drawUtil.hpp"
#include "mathUtil.hpp"
#include "createPolygonTrack.hpp"

namespace car {

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

sf::Vector2f Track::collideWithRay(const sf::Vector2f& origin, const sf::Vector2f& direction,
		float maxViewDistance) const {
	Line2f lineToCheck{origin, origin + normalize(direction) * maxViewDistance};
	for ( const Line2f& trackLine : lines ) {
		sf::Vector2f out;
		if ( intersects(trackLine, lineToCheck, &out) ) {
			lineToCheck.end = out;
		}
	}

	return lineToCheck.end;
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

void Track::drawBoundary(sf::RenderWindow& window) const {
	for (const Line2f& trackLine : lines) {
		drawLine(window, trackLine);
	}
}

void Track::drawCheckpoints(sf::RenderWindow& window, int highlightCheckpoint) const {
	sf::Color checkpointColor{255, 255, 64};
	sf::Color highlightedCheckpointColor{128, 128, 255};

	for (std::size_t i = 0; i < checkpoints.size(); ++i) {

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

