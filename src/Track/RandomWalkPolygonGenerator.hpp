#ifndef SRC_TRACK_RANDOMWALKPOLYGONGENERATOR_HPP
#define SRC_TRACK_RANDOMWALKPOLYGONGENERATOR_HPP

#include <SFML/Graphics.hpp>
#include "RandomGenerator.hpp"
#include "RandomTrackException.hpp"
#include "BaseRandomWalk.hpp"

namespace car { namespace track {

class RandomWalkPolygonGenerator {
public:
	struct Params {
		std::size_t horizontalResolution = 10;
		std::size_t verticalResolution = 10;
		float gridSize = 20.f;
		float jitter = 2.f;
		std::shared_ptr<randomWalk::BaseRandomWalk> randomWalkAlgorithm;
	};

	RandomWalkPolygonGenerator(const Params& params): params(params)
	{
	}

	std::vector<sf::Vector2f> operator()(RandomGenerator& rng) const;
private:
	Params params;
};

}} /* namespace car::track */



#endif /* SRC_TRACK_RANDOMWALKPOLYGONGENERATOR_HPP */
