#include "PolygonTrackType.hpp"
#include <sstream>
#include <boost/range/adaptor/transformed.hpp>
#include "Track.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"

namespace po = boost::program_options;

namespace car {

namespace {

struct Params {
	float checkpointDistance = 5.f;
	float trackWidth = 5.f;
	std::vector<std::string> points;
};

po::options_description createOptionsDescription(Params& params) {
	po::options_description optionsDescription;
	optionsDescription.add_options()
			("checkpoint-distance", paramWithDefaultValue(params.checkpointDistance),
					"The distance between each checkpoint.")
			("track-width", paramWithDefaultValue(params.trackWidth),
					"The width of the track at straight sections.")
			("p", po::value<std::vector<std::string>>(&params.points),
					"The points of the polygon. Each point is one argument with the format x,y")
			;
	return optionsDescription;
}

const std::string argumentName = "polygon";

}

std::function<Track()> PolygonTrackType::getTrackCreator(const std::vector<std::string>& args) {
	Params params;
	parseConfigFile(args[0], createOptionsDescription(params));
	auto pointParams = params.points | boost::adaptors::transformed(parsePoint);
	std::vector<sf::Vector2f> points(pointParams.begin(), pointParams.end());
	return [params, points]() {
			return createPolygonTrack(params.checkpointDistance, params.trackWidth,	points);
		};
}

std::string PolygonTrackType::getHelpString() {
	std::ostringstream ss;
	Params params;
	ss << "Create a track based on a polygon. This polygon defines the middle line of the track.\n"
			"The track edges are track-width/2 distance from this polygon in each direction.\n"
			"Format: " << argumentName << ":<file name>\n" <<
			createOptionsDescription(params);
	return ss.str();
}

std::string PolygonTrackType::getArgumentName() {
	return argumentName;
}

std::size_t PolygonTrackType::getMinimumNumberOfArgs() {
	return 1;
}


} /* namespace car */

