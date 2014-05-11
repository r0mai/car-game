#include "CircleTrackType.hpp"
#include <sstream>
#include "createCircleTrack.hpp"
#include "optionsUtil.hpp"

namespace po = boost::program_options;

namespace car {

namespace {

po::options_description createOptionsDescription(CircleTrackParams& params) {
	po::options_description optionsDescription;
	optionsDescription.add_options()
			("inner-radius", paramWithDefaultValue(params.innerRadius),
					"The radius of the inner edge of the track.")
			("outer-radius", paramWithDefaultValue(params.outerRadius),
					"The radius of the outer edge of the track.")
			("resolution", paramWithDefaultValue(params.resolution),
					"The number of lines for approximating the circle.")
			("number-of-checkpoints", paramWithDefaultValue(params.numberOfCheckpoints),
					"The number of checkpoints for the track.")
			;
	return optionsDescription;
}

const std::string argumentName = "circle";

}

std::function<Track()> CircleTrackType::getTrackCreator(const std::vector<std::string>& args) {
	CircleTrackParams params;
	if (args.size() > 0) {
		parseConfigFile(args[0], createOptionsDescription(params));
	}
	return [params]() {
			return createCircleTrack(params);
		};
}

std::string CircleTrackType::getHelpString() {
	std::ostringstream ss;
	CircleTrackParams params;
	ss << "Create a circular track from a config file.\n"
			"If the file is omitted, then the default parameters are used.\n"
			"Format: " << argumentName << "[:<file name>]\n" <<
			createOptionsDescription(params);
	return ss.str();
}

std::string CircleTrackType::getArgumentName() {
	return argumentName;
}

std::size_t CircleTrackType::getMinimumNumberOfArgs() {
	return 0;
}


} /* namespace car */

