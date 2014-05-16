#include "RandomTrackType.hpp"
#include <sstream>
#include "RandomTrackGenerator.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"

namespace po = boost::program_options;

namespace car {

RandomTrackType::RandomTrackType():PolygonBasedTrackType{"random"} {
	optionsDescription.add_options()
			("max-tries", paramWithDefaultValue(generatorParams.maxTries),
					"The number of unsuccessful generations after which the generator fails.")
			("number-of-points", paramWithDefaultValue(generatorParams.numberOfPoints),
					"The number of edges of the generated track. Minimum is 4 as the starting "
					"shape is a rectangle.")
			("corner1", paramWithDefaultValue(corner1),
					"The first corner of the rectangle where to generate points.")
			("corner2", paramWithDefaultValue(corner2),
					"The second corner of the rectangle where to generate points.")
			("inset", paramWithDefaultValue(generatorParams.inset),
					"The relative position within the generating rectangle where the "
					"initial points are placed (in ratio of the total size).")
			;
}

std::function<Track()> RandomTrackType::getTrackCreator(
		const boost::program_options::variables_map& variablesMap,
		const std::vector<std::string>& args) {
	using std::placeholders::_1;
	assert(getPolygonType());

	generatorParams.corner1 = parsePoint(corner1);
	generatorParams.corner2 = parsePoint(corner2);
	generatorParams.generator = getPolygonType()->getTrackCreator(variablesMap);
	RandomTrackGenerator generator{generatorParams};
	return std::bind(generator, boost::lexical_cast<uint>(args[0]));
}

std::string RandomTrackType::getHelpString() {
	std::ostringstream ss;
	ss << "Create a pseudo-random polygon based track. The track is based off a rectangle and other\n"
			"points are generated randomly. From this polygon, the polygon track generator is used\n"
			"to create a track.\n"
			"Format: <file name>:<seed>\n" << optionsDescription;
	return ss.str();
}

std::size_t RandomTrackType::getMinimumNumberOfArgs() {
	return 1;
}


} /* namespace car */




