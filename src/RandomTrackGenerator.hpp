#ifndef RANDOMTRACKGENERATOR_HPP_
#define RANDOMTRACKGENERATOR_HPP_

#include <functional>
#include <stdexcept>
#include <boost/random/mersenne_twister.hpp>
#include "Line2.hpp"

namespace car {

class Track;

struct RandomTrackException: std::logic_error {
	RandomTrackException(const std::string& what_arg):
		std::logic_error{what_arg}
	{}
};

class RandomTrackGenerator {
public:
	using Generator = std::function<Track(float, std::vector<sf::Vector2f>)>;

	RandomTrackGenerator(
			Generator generator,
			int maxTries, int numberOfPoints,
			float minPathWidth, float maxPathWidth,
			sf::Vector2f corner1, sf::Vector2f corner2):
				generator(generator), maxTries(maxTries),
				numberOfPoints(numberOfPoints),
				minPathWidth(minPathWidth), maxPathWidth(maxPathWidth),
				corner1(corner1), corner2(corner2)
	{
		if (numberOfPoints < 4) {
			throw RandomTrackException{"Too few points (minimum = 4)"};
		}
	}

	Track operator()(uint seed) const;
private:
	Generator generator;
	int maxTries;
	int numberOfPoints;
	float minPathWidth;
	float maxPathWidth;

	sf::Vector2f corner1;
	sf::Vector2f corner2;

	Track generateTrack(boost::random::mt19937& rng) const;
};

} /* namespace car */
#endif /* RANDOMTRACKGENERATOR_HPP_ */
