#ifndef TRACK_HPP
#define TRACK_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Line2.hpp"

namespace car {

class Track {
public:

	void addLine(const Line2f& line);
	bool collidesWith(const Line2f& line);

	void draw(sf::RenderWindow& window) const;
private:
	typedef std::vector<Line2f> Lines;
	Lines lines;
};

}

#endif /* !TRACK_HPP */
