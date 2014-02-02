
#include "Track.hpp"
#include "drawUtil.hpp"

namespace car {

void Track::addLine(const Line2f& line) {
	lines.push_back(line);
}


bool Track::collidesWith(const Line2f& line) {
	for ( const Line2f& trackLine : lines ) {
		if ( line.intersectWith(trackLine) ) {
			return true;
		}
	}
	return false;
}

void Track::draw(sf::RenderWindow& window) const {
	for ( const Line2f& trackLine : lines ) {
		drawLine(window, trackLine);
	}
}

}

