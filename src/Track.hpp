#ifndef TRACK_HPP
#define TRACK_HPP

#include <vector>
#include <stdexcept>
#include <functional>

#include <boost/optional.hpp>

#include <SFML/Graphics.hpp>

#include "Line2.hpp"

namespace car {

struct TrackError: std::logic_error {
	TrackError(const std::string& what_arg):
		std::logic_error{what_arg}
	{}
};

class Car;

class Track {
public:
	void addLine(const Line2f& line);
	void addCheckpoint(const Line2f& line);

	boost::optional<sf::Vector2f> collideWithRay(const sf::Vector2f& origin, const sf::Vector2f& direction) const;

	bool collidesWith(const Line2f& line) const;
	int checkpointCollidesWith(const Line2f& line) const;

	std::size_t getNumberOfCheckpoints() const;
	const Line2f& getCheckpoint(std::size_t n) const;
	void check() const;

	sf::FloatRect getDimensions() const;

	void drawBoundary(sf::RenderWindow& window) const;
	void drawCheckpoints(sf::RenderWindow& window, int highlightCheckpoint = -1) const;

	void setOrigin(const sf::Vector2f& point, float direction);
	Car createCar() const;
private:
	typedef std::vector<Line2f> Lines;
	Lines lines;
	Lines checkpoints;
	sf::Vector2f startingPoint;
	float startingDirection = 0.f;
};

Track createCircleTrack();
Track createZigZagTrack();
Track createCurvyTrack();

}

#endif /* !TRACK_HPP */
