
#include "Track.hpp"
#include "Car.hpp"
#include "drawUtil.hpp"
#include "mathUtil.hpp"
#include "createPolygonTrack.hpp"
#include "Line2.hpp"
#include "RandomGenerator.hpp"
#include <boost/random/normal_distribution.hpp>
#include <boost/functional/hash.hpp>

namespace car { namespace track {

namespace {

class CarParameterPerturbator {
public:
	CarParameterPerturbator(RandomGenerator& randomGenerator, CarParameters& parameters, const CarParameters& perturbation):
		randomGenerator(randomGenerator),
		parameters(parameters),
		perturbation(perturbation)
	{}

	void operator()(float CarParameters::* parameter) {
		boost::random::normal_distribution<float> dist{parameters.*parameter, perturbation.*parameter};
		parameters.*parameter = dist(randomGenerator);
	}

private:
	RandomGenerator& randomGenerator;
	CarParameters& parameters;
	const CarParameters& perturbation;
};

}

void Track::perturbateCarParameters(CarParameters& parameters, const CarParameters& perturbation) const {
	std::size_t seed = 0;
	for (const auto& line: lines) {
		boost::hash_combine(seed, line.start.x);
		boost::hash_combine(seed, line.start.y);
		boost::hash_combine(seed, line.end.x);
		boost::hash_combine(seed, line.end.y);
	}
	RandomGenerator randomGenerator{seed};

	CarParameterPerturbator perturbator{randomGenerator, parameters, perturbation};
	perturbator(&CarParameters::cDrag);
	perturbator(&CarParameters::cRollingResistance);
	perturbator(&CarParameters::pEngine);
	perturbator(&CarParameters::fEngineMax);
	perturbator(&CarParameters::fBrake);
	perturbator(&CarParameters::mass);
	perturbator(&CarParameters::maxTurnAngle);
	perturbator(&CarParameters::turnRate);
	perturbator(&CarParameters::rearCMDistance);
	perturbator(&CarParameters::frontCMDistance);
	perturbator(&CarParameters::carWidth);
	perturbator(&CarParameters::throttleIncreaseSpeed);
	perturbator(&CarParameters::throttleDecreaseSpeed);
	perturbator(&CarParameters::brakeIncreaseSpeed);
	perturbator(&CarParameters::brakeDecreaseSpeed);
	perturbator(&CarParameters::turnSpeed);
}

Car Track::createCar(CarParameters parameters, const CarParameters& perturbation) const {
	perturbateCarParameters(parameters, perturbation);
	return Car{parameters, startingPoint, startingDirection};
}

bool Track::isInsideTrack(const sf::Vector2f& p) const {
	Line2f line{startingPoint, p};
	return std::count_if(lines.begin(), lines.end(),
			[&](const Line2f& trackLine) { return intersects(trackLine, line); }
		) % 2 == 0;
}

void Track::setOrigin(const sf::Vector2f& point, float direction) {
	startingPoint = point;
	startingDirection = direction;
}


void Track::addLine(const Line2f& line) {
	lines.push_back(line);
}

void Track::addCheckpoint(const Line2f& line) {
	auto direction = rotateClockwise(line.end - line.start);
	checkpoints.push_back({line, std::atan2(direction.y, direction.x)});
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

bool Track::collidesWithCheckpoint(const Line2f& line, std::size_t checkpointId) const {
	return intersects(line, checkpoints[checkpointId].line);
}

std::size_t Track::getNumberOfCheckpoints() const {
	return checkpoints.size();
}

const Line2f& Track::getCheckpointLine(std::size_t n) const {
	return checkpoints[n].line;
}

float Track::getCheckpointAngle(std::size_t n) const {
	return checkpoints[n].angle;
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

		auto color = (static_cast<int>(i) == highlightCheckpoint) ?
						checkpointColor : highlightedCheckpointColor;
		drawLine(window, checkpoints[i].line, color);
		auto centerPoint = (checkpoints[i].line.start + checkpoints[i].line.end) / 2.f;
		drawLine(window, centerPoint,
				centerPoint + sf::Vector2f(cos(checkpoints[i].angle), sin(checkpoints[i].angle)),
				color);
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

}} /* namespace car::track */

