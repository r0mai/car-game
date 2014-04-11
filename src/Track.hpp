#ifndef TRACK_HPP
#define TRACK_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Line2.hpp"

namespace car {

class Track {
public:

	static Track createCircleTrack();

	void addLine(const Line2f& line);
	void addCheckpoint(const Line2f& line);
	bool collidesWith(const Line2f& line);
	int checkpointCollidesWith(const Line2f& line);
	std::size_t getNumberOfCheckpoints() const;

	void draw(sf::RenderWindow& window, int highlihgtCheckpoint = -1) const;
private:
	typedef std::vector<Line2f> Lines;
	Lines lines;
	Lines checkpoints;
};

}

#endif /* !TRACK_HPP */
