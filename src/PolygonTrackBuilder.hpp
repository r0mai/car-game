#ifndef POLYGONTRACKBUILDER_HPP_
#define POLYGONTRACKBUILDER_HPP_

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Track.hpp"

namespace car {

class PolygonTrackBuilder {
public:
	PolygonTrackBuilder(float checkpointDistance):
		checkpointDistance(checkpointDistance)
	{}

	Track operator()(float trackWidth, const std::vector<sf::Vector2f>& points);
private:
	float checkpointDistance;
};

} /* namespace car */
#endif /* POLYGONTRACKBUILDER_HPP_ */
