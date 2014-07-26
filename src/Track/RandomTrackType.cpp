#include "Track.hpp"
#include "RandomTrackType.hpp"
#include <sstream>
#include "RandomTrackGenerator.hpp"
#include "createPolygonTrack.hpp"
#include "optionsUtil.hpp"
#include "TrackArgumentParser.hpp"
#include "Line2.hpp"

namespace po = boost::program_options;

namespace car { namespace track {

RandomTrackType::RandomTrackType():PolygonBasedTrackType{"random"} {
	optionsDescription.add_options()
			("max-tries", paramWithDefaultValue(generatorParams.maxTries),
					"The number of unsuccessful generations after which the generator fails.")
			("algorithm", po::value(&algorithm)->required(),
					"The algorithm used to generate the polygon.")
			;
}

TrackCreator RandomTrackType::getTrackCreator(
		const boost::program_options::variables_map& variablesMap,
		const std::vector<std::string>& args) {
	using std::placeholders::_1;
	assert(getPolygonType());

	generatorParams.generator = getPolygonType()->getTrackCreator(variablesMap);
	generatorParams.polygonGenerator = polygonGeneratorType->getPolygonCreator(variablesMap);
	RandomTrackGenerator generator{generatorParams};
	return std::bind(generator, boost::lexical_cast<uint>(args[0]));
}

bool RandomTrackType::needsReparse(
		const boost::program_options::variables_map& variablesMap,
		const std::vector<std::string>& args) {
	if (PolygonBasedTrackType::needsReparse(variablesMap, args)) {
		return true;
	} else if (!polygonGeneratorType) {
		polygonGeneratorType = trackArgumentParser::getRandomPolygonGeneratorType(algorithm);
		return true;
	} else {
		return false;
	}
}

boost::program_options::options_description RandomTrackType::getOptions() {
	if (polygonGeneratorType) {
		return polygonGeneratorType->getOptions();
	} else {
		return PolygonBasedTrackType::getOptions();
	}
}

std::string RandomTrackType::getHelpString() {
	std::ostringstream ss;
	ss << "Create a pseudo-random polygon based track. The polygon is created\n"
			"using some random polygon generation algorithm.\n"
			"From this polygon, the polygon track generator is used to create a track.\n"
			"Format: <file name>:<seed>\n" << optionsDescription;
	return ss.str();
}

std::size_t RandomTrackType::getMinimumNumberOfArgs() {
	return 1;
}


}} /* namespace car::track */




