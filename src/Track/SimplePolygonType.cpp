#include "SimplePolygonType.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"

namespace car {

SimplePolygonType::SimplePolygonType():BasicPolygonType{"simple"} {
	optionsDescription.add_options()
			("checkpoint-distance", paramWithDefaultValue(params.checkpointDistance),
					"The distance between each checkpoint.")
			("track-width", paramWithDefaultValue(params.trackWidth),
					"The width of the track at straight sections.")
			;
}

std::function<Track(const std::vector<sf::Vector2f>)> SimplePolygonType::getTrackCreator(
		const boost::program_options::variables_map& /*variablesMap*/) {
	using std::placeholders::_1;
	return std::bind(createPolygonTrack, params.checkpointDistance, params.trackWidth, _1);
}

std::string SimplePolygonType::getHelpString() {
	return "";
}

}



