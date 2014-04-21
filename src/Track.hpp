#ifndef TRACK_HPP
#define TRACK_HPP

#include <SFML/Graphics.hpp>

#include <vector>

#include <boost/optional.hpp>

#include "Line2.hpp"

namespace car {

class Track {
public:

	static Track createCircleTrack();

	void addLine(const Line2f& line);
	void addCheckpoint(const Line2f& line);

	boost::optional<sf::Vector2f> collideWithRay(const sf::Vector2f& origin, const sf::Vector2f& direction) const;

	bool collidesWith(const Line2f& line) const;
	int checkpointCollidesWith(const Line2f& line) const;

	std::size_t getNumberOfCheckpoints() const;

	void draw(sf::RenderWindow& window, int highlightCheckpoint = -1) const;
private:
	typedef std::vector<Line2f> Lines;
	Lines lines;
	Lines checkpoints;
};

}

#endif /* !TRACK_HPP */
