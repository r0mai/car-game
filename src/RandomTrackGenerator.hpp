#ifndef RANDOMTRACKGENERATOR_HPP_
#define RANDOMTRACKGENERATOR_HPP_

#include <functional>
#include <stdexcept>
#include <boost/random/mersenne_twister.hpp>
#include "Line2.hpp"
#include "Track.hpp"

namespace car {

struct RandomTrackException: std::logic_error {
	RandomTrackException(const std::string& what_arg):
		std::logic_error{what_arg}
	{}
};

class RandomTrackGenerator {
public:
	using Generator = std::function<Track(const std::vector<sf::Vector2f>&)>;

	struct Params {
		Generator generator;
		int maxTries = 100;
		int numberOfPoints = 10;
		float inset = 0.2f;

		sf::Vector2f corner1{-60.f, -60.f};
		sf::Vector2f corner2{60.f, 60.f};
	};

	RandomTrackGenerator(const Params& params): params(params)
	{
		if (params.numberOfPoints < 4) {
			throw RandomTrackException{"Too few points (minimum = 4)"};
		}
	}

	Track operator()(uint seed) const;
private:
	Params params;
	Track generateTrack(boost::random::mt19937& rng) const;
};

} /* namespace car */
#endif /* RANDOMTRACKGENERATOR_HPP_ */
