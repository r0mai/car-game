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

po::options_description createOptionsDescription(Params* params) {
	po::options_description optionsDescription;
	optionsDescription.add_options()
			("checkpoint-distance", optionalParamValue(&Params::checkpointDistance, params),
					"The distance between each checkpoint.")
			("track-width", optionalParamValue(&Params::trackWidth, params),
					"The width of the track at straight sections.")
			("p", optionalParamValue(&Params::points, params),
					"The points of the polygon. Each point is one argument with the format x,y")
			;
	return optionsDescription;
}

const std::string argumentName = "polygon";

}

std::function<Track()> PolygonTrackType::getTrackCreator(const std::vector<std::string>& args) {
	Params params;
	parseConfigFile(args[0], createOptionsDescription(&params));
	auto pointParams = params.points | boost::adaptors::transformed(parsePoint);
	std::vector<sf::Vector2f> points(pointParams.begin(), pointParams.end());
	return [params, points]() {
			return createPolygonTrack(params.checkpointDistance, params.trackWidth,	points);
		};
}

std::string PolygonTrackType::getHelpString() {
	std::ostringstream ss;
	ss << "Create a track based on a polygon. This polygon defines the middle line of the track.\n"
			"The track edges are track-width/2 distance from this polygon in each direction.\n"
			"Format: " << argumentName << ":<file name>\n" <<
			createOptionsDescription(nullptr);
	return ss.str();
}

std::string PolygonTrackType::getArgumentName() {
	return argumentName;
}

std::size_t PolygonTrackType::getMinimumNumberOfArgs() {
	return 1;
}


} /* namespace car */

