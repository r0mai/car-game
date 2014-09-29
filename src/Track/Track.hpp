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

	void drawBoundary(sf::RenderWindow& window) const;
	void drawCheckpoints(sf::RenderWindow& window, int highlightCheckpoint = -1) const;

	void setOrigin(const sf::Vector2f& point, float direction);
	Car createCar(CarParameters parameters, const PerturbationParameters& perturbation) const;

	bool isInsideTrack(const sf::Vector2f& p) const;
private:
	typedef std::vector<Line2f> Lines;
	Lines lines;

	struct Checkpoint {
		Line2f line;
		float angle;
	};
	std::vector<Checkpoint> checkpoints;

	sf::Vector2f startingPoint;
	float startingDirection = 0.f;

	void perturbateCarParameters(CarParameters& parameters, const PerturbationParameters& perturbation) const;
};

}} /* namespace car::track */

#endif /* !TRACK_HPP */
