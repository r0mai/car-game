#include "SimplePolygonType.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"

namespace car { namespace track {

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
	std::ostringstream ss;
	ss << "The track is created by shifting each line of the polygon track-width/2\n"
			"in each direction, then normalizing at the edges. Checkpoints are\n"
			"generated at each end of the lines and between the lines at regular\n"
			"intervals.\n" << optionsDescription;
	return ss.str();
}

}} /* namespace car::track */



