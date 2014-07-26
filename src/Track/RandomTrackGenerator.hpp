#ifndef RANDOMTRACKGENERATOR_HPP_
#define RANDOMTRACKGENERATOR_HPP_

#include <functional>
#include <vector>
#include <SFML/Graphics.hpp>
#include "RandomGenerator.hpp"

namespace car { namespace track {

class Track;

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
