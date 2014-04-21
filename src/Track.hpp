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

class Track {
public:
	void addLine(const Line2f& line);
	void addCheckpoint(const Line2f& line);

	boost::optional<sf::Vector2f> collideWithRay(const sf::Vector2f& origin, const sf::Vector2f& direction) const;

	bool collidesWith(const Line2f& line) const;
	int checkpointCollidesWith(const Line2f& line) const;

	std::size_t getNumberOfCheckpoints() const;
	void check() const;

	void draw(sf::RenderWindow& window, int highlightCheckpoint = -1) const;
private:
	typedef std::vector<Line2f> Lines;
	Lines lines;
	Lines checkpoints;
};

Track createCircleTrack();
Track createZigZagTrack();

}

#endif /* !TRACK_HPP */
