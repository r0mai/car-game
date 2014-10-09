#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <stdexcept>
#include <functional>

#include <boost/optional.hpp>

#include <SFML/Graphics.hpp>

#include "Line2.hpp"


namespace car {

class Car;
struct CarParameters;
struct PerturbationParameters;

namespace track {

struct TrackError: std::logic_error {
	TrackError(const std::string& what_arg):
		std::logic_error{what_arg}
	{}
};

class Track {
public:
	struct Checkpoint {
		Line2f line;
		float angle;
	};

	typedef std::vector<Line2f> Lines;
	typedef std::vector<Checkpoint> Checkpoints;

	void addLine(const Line2f& line);
	void addCheckpoint(const Line2f& line);

	sf::Vector2f collideWithRay(const sf::Vector2f& origin, const sf::Vector2f& direction,
			float maxViewDistance) const;

	bool collidesWith(const Line2f& line) const;
	bool collidesWithCheckpoint(const Line2f& line, std::size_t checkpointId) const;

	std::size_t getNumberOfCheckpoints() const;
	const Line2f& getCheckpointLine(std::size_t n) const;
	float getCheckpointAngle(std::size_t n) const;
	void check() const;

	sf::FloatRect getDimensions() const;

	void setOrigin(const sf::Vector2f& point, float direction);
	Car createCar(CarParameters parameters, const PerturbationParameters& perturbation) const;

	bool isInsideTrack(const sf::Vector2f& p) const;
	const Checkpoints& getCheckpoints() const { return checkpoints; }
	const Lines& getLines() const { return lines; }
	const Lines& getCenterLines() const;
private:
	Lines lines;
	Checkpoints checkpoints;
	mutable boost::optional<Lines> centerLines;

	sf::Vector2f startingPoint;
	float startingDirection = 0.f;

	void perturbateCarParameters(CarParameters& parameters, const PerturbationParameters& perturbation) const;
	void calculateCenterLines() const;
};

}} /* namespace car::track */

#endif /* !TRACK_HPP */
