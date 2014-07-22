#ifndef SRC_SCREENDIMENSION_HPP
#define SRC_SCREENDIMENSION_HPP

#include <boost/variant.hpp>
#include <SFML/Graphics.hpp>
#include "ExplicitType.hpp"

namespace car {

using Meters = ExplicitType<struct tag_Meters, float>;
using Percent = ExplicitType<struct tag_Percent, float>;
using Pixels = ExplicitType<struct tag_Pixels, unsigned>;

using ScreenDimension = boost::variant<Meters, Percent, Pixels>;

class ScreenDimensionConverter {
public:
	using result_type = float;

	ScreenDimensionConverter(sf::Vector2f viewSize, float pixelsPerMeter):
		viewSize(viewSize), pixelsPerMeter(pixelsPerMeter)
	{}

	result_type operator()(const Meters& meters) const {
		return meters.value();
	}

	result_type operator()(const Percent& percent) const {
		// size / 2 * percent / 100
		return std::min(viewSize.x, viewSize.y) * percent.value() / 200.f;
	}

	result_type operator()(const Pixels& pixels) const {
		return pixels.value() / pixelsPerMeter;
	}
private:
	sf::Vector2f viewSize;
	float pixelsPerMeter;
};

ScreenDimension parseScreenDimenstion(const std::string& s);


}




#endif /* SRC_SCREENDIMENSION_HPP */
