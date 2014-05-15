#include "RandomTrackType.hpp"
#include <sstream>
#include "RandomTrackGenerator.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"

namespace po = boost::program_options;

namespace car {

RandomTrackType::RandomTrackType():optionsDescription{"Random track type options"} {
	optionsDescription.add_options()
			("checkpoint-distance", paramWithDefaultValue(params.checkpointDistance),
					"The distance between each checkpoint.")
			("track-width", paramWithDefaultValue(params.trackWidth),
					"The width of the track at straight sections.")
			("max-tries", paramWithDefaultValue(params.generatorParams.maxTries),
					"The number of unsuccessful generations after which the generator fails.")
			("number-of-points", paramWithDefaultValue(params.generatorParams.numberOfPoints),
					"The number of edges of the generated track. Minimum is 4 as the starting "
					"shape is a rectangle.")
			("corner1", paramWithDefaultValue(params.corner1),
					"The first corner of the rectangle where to generate points.")
			("corner2", paramWithDefaultValue(params.corner2),
					"The second corner of the rectangle where to generate points.")
			("inset", paramWithDefaultValue(params.generatorParams.inset),
					"The relative position within the generating rectangle where the "
					"initial points are placed (in ratio of the total size).")
			;
}

namespace {

const std::string argumentName = "random";

}

std::function<Track()> RandomTrackType::getTrackCreator(
		const boost::program_options::variables_map& /*variablesMap*/,
		const std::vector<std::string>& args) {
	using std::placeholders::_1;

	params.generatorParams.corner1 = parsePoint(params.corner1);
	params.generatorParams.corner2 = parsePoint(params.corner2);
	params.generatorParams.generator = std::bind(&createPolygonTrack,
			params.checkpointDistance, params.trackWidth, _1);
	RandomTrackGenerator generator{params.generatorParams};
	return std::bind(generator, boost::lexical_cast<uint>(args[0]));
}

std::string RandomTrackType::getHelpString() {
	std::ostringstream ss;
	Params params;
	ss << "Create a pseudo-random polygon based track. The track is based off a rectangle and other\n"
			"points are generated randomly. From this polygon, the polygon track generator is used\n"
			"to create a track.\n"
			"Format: <file name>:<seed>\n" << optionsDescription;
	return ss.str();
}

boost::program_options::options_description RandomTrackType::getOptions() {
	return optionsDescription;
}

std::string RandomTrackType::getArgumentName() {
	return argumentName;
}

std::size_t RandomTrackType::getMinimumNumberOfArgs() {
	return 1;
}


} /* namespace car */




