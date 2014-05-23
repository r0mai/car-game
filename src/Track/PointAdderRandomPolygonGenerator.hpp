#ifndef SRC_TRACK_POINTADDERRANDOMPOLYGONGENERATOR_HPP
#define SRC_TRACK_POINTADDERRANDOMPOLYGONGENERATOR_HPP

#include "RandomGenerator.hpp"
#include "RandomTrackException.hpp"
#include "Line2.hpp"

namespace car { namespace track {

class PointAdderRandomPolygonGenerator {
public:
	struct Params {
		int numberOfPoints = 10;
		float inset = 0.2f;

		sf::Vector2f corner1{-60.f, -60.f};
		sf::Vector2f corner2{60.f, 60.f};
	};

	PointAdderRandomPolygonGenerator(const Params& params): params(params) {
		if (params.numberOfPoints < 4) {
			throw RandomTrackException{"Too few points (minimum = 4)"};
		}
	}

	std::vector<sf::Vector2f> operator()(RandomGenerator& rnd) const;
private:
	Params params;
};

}} /* namespace car::track */


#endif /* SRC_TRACK_POINTADDERRANDOMPOLYGONGENERATOR_HPP */
