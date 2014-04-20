#ifndef POLYGONTRACKBUILDER_HPP_
#define POLYGONTRACKBUILDER_HPP_

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Track.hpp"

namespace car {

class PolygonTrackBuilder {
public:
	PolygonTrackBuilder(float trackWidth, float checkpointDistance):
		trackWidth(trackWidth), checkpointDistance(checkpointDistance)
	{}

	Track operator()(const std::vector<sf::Vector2f>& points);
private:
	float trackWidth;
	float checkpointDistance;
};

} /* namespace car */
#endif /* POLYGONTRACKBUILDER_HPP_ */
