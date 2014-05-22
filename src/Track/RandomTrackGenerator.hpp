#ifndef RANDOMTRACKGENERATOR_HPP_
#define RANDOMTRACKGENERATOR_HPP_

#include <functional>
#include "RandomGenerator.hpp"
#include "Line2.hpp"
#include "Track.hpp"

namespace car { namespace track {

class RandomTrackGenerator {
public:
	using Generator = std::function<Track(const std::vector<sf::Vector2f>&)>;
	using PolygonGenerator = std::function<std::vector<sf::Vector2f>(RandomGenerator&)>;

	struct Params {
		Generator generator;
		PolygonGenerator polygonGenerator;
		int maxTries = 100;
	};

	RandomTrackGenerator(const Params& params): params(params)
	{ }

	Track operator()(uint seed) const;
private:
	Params params;
};

}} /* namespace car::track */

#endif /* RANDOMTRACKGENERATOR_HPP_ */
