#ifndef POLYGONTRACKBUILDER_HPP_
#define POLYGONTRACKBUILDER_HPP_

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Track.hpp"

namespace car { namespace track {

Track createPolygonTrack(float checkpointDistance, float trackWidth, const std::vector<sf::Vector2f>& points);

}} /* namespace car::track */

#endif /* POLYGONTRACKBUILDER_HPP_ */
