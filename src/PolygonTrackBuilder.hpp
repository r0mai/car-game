#ifndef POLYGONTRACKBUILDER_HPP_
#define POLYGONTRACKBUILDER_HPP_

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Track.hpp"

namespace car {

class PolygonTrackBuilder {
public:
	PolygonTrackBuilder(float trackWidth):
		trackWidth(trackWidth)
	{}

	Track operator()(const std::vector<sf::Vector2f>& points);
private:
	float trackWidth;
};

} /* namespace car */
#endif /* POLYGONTRACKBUILDER_HPP_ */
