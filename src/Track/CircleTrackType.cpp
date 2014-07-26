#include "CircleTrackType.hpp"

#include <sstream>
#include <boost/program_options.hpp>
#include "optionsUtil.hpp"
#include "Line2.hpp"

namespace po = boost::program_options;

namespace car { namespace track {

CircleTrackType::CircleTrackType():
		BasicTrackType{"circle"} {
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

TrackCreator CircleTrackType::getTrackCreator(const boost::program_options::variables_map& /*variablesMap*/,
		const std::vector<std::string>& /*args*/) {
	return std::bind(createCircleTrack, params);
}

bool CircleTrackType::needsReparse(
		const boost::program_options::variables_map& /*variablesMap*/,
		const std::vector<std::string>& /*args*/) {
	return false;
}

std::string CircleTrackType::getHelpString() {
	std::ostringstream ss;
	ss << "Create a circular track from a config file.\n" <<
			getOptions();
	return ss.str();
}

std::size_t CircleTrackType::getMinimumNumberOfArgs() {
	return 0;
}

}} /* namespace car::track */

