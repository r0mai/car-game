#include "PolygonTrackType.hpp"
#include <sstream>
#include <boost/range/adaptor/transformed.hpp>
#include "Track.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"

namespace po = boost::program_options;

namespace car {

PolygonTrackType::PolygonTrackType():BasicTrackType{"polygon"} {
	optionsDescription.add_options()
			("checkpoint-distance", paramWithDefaultValue(params.checkpointDistance),
					"The distance between each checkpoint.")
			("track-width", paramWithDefaultValue(params.trackWidth),
					"The width of the track at straight sections.")
			("p", po::value(&params.points),
					"The points of the polygon. Each point is one argument with the format x,y")
			;
}

std::function<Track()> PolygonTrackType::getTrackCreator(
		const boost::program_options::variables_map& /*variablesMap*/,
		const std::vector<std::string>& /*args*/) {
	auto pointParams = params.points | boost::adaptors::transformed(parsePoint);
	std::vector<sf::Vector2f> points(pointParams.begin(), pointParams.end());
	return std::bind(createPolygonTrack, params.checkpointDistance, params.trackWidth,	points);
}

std::string PolygonTrackType::getHelpString() {
	std::ostringstream ss;
	ss << "Create a track based on a polygon. This polygon defines the middle line of the track.\n"
			"The track edges are track-width/2 distance from this polygon in each direction.\n"
			<< optionsDescription;
	return ss.str();
}

std::size_t PolygonTrackType::getMinimumNumberOfArgs() {
	return 0;
}


} /* namespace car */

