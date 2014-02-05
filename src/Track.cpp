
#include "Track.hpp"
#include "drawUtil.hpp"

namespace car {

Track Track::createCircleTrack() {
	Track track;
	const int circleResolution = 20;
	const float innerCircleRadius = 50.;
	const float outerCircleRadius = 60.;
	for ( int i = 0; i < circleResolution; ++i ) {
		track.addLine(Line2f(
					innerCircleRadius*std::cos((i-1)*2*M_PI/circleResolution),
					innerCircleRadius*std::sin((i-1)*2*M_PI/circleResolution),
					innerCircleRadius*std::cos((i)*2*M_PI/circleResolution),
					innerCircleRadius*std::sin((i)*2*M_PI/circleResolution)));
		track.addLine(Line2f(
					outerCircleRadius*std::cos((i-1)*2*M_PI/circleResolution),
					outerCircleRadius*std::sin((i-1)*2*M_PI/circleResolution),
					outerCircleRadius*std::cos((i)*2*M_PI/circleResolution),
					outerCircleRadius*std::sin((i)*2*M_PI/circleResolution)));
	}
	return track;
}

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

