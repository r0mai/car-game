#include "CircleTrackType.hpp"

#include <sstream>
#include <boost/program_options.hpp>
#include "optionsUtil.hpp"

namespace po = boost::program_options;

namespace car {

namespace {

const std::string argumentName = "circle";

}

CircleTrackType::CircleTrackType():optionsDescription{"Circle track type options"} {
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
}

std::function<Track()> CircleTrackType::getTrackCreator(const boost::program_options::variables_map& /*variablesMap*/,
		const std::vector<std::string>& /*args*/) {
	auto params = this->params;
	return [params]() {
			return createCircleTrack(params);
		};
}

std::string CircleTrackType::getHelpString() {
	std::ostringstream ss;
	ss << "Create a circular track from a config file.\n"
			"If the file is omitted, then the default parameters are used.\n" <<
			optionsDescription;
	return ss.str();
}

boost::program_options::options_description CircleTrackType::getOptions() {
	return optionsDescription;
}

std::string CircleTrackType::getArgumentName() {
	return argumentName;
}

std::size_t CircleTrackType::getMinimumNumberOfArgs() {
	return 0;
}

} /* namespace car */

