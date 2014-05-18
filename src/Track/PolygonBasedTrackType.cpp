#include "PolygonBasedTrackType.hpp"
#include "TrackArgumentParser.hpp"

namespace po = boost::program_options;

namespace car { namespace track {

PolygonBasedTrackType::PolygonBasedTrackType(const std::string& argumentName):
		BasicTrackType{argumentName}
{
	optionsDescription.add_options()
		("polygon-type", po::value(&polygonTypeName)->required(),
				"The type of the polygon to use. Further possible options depend on this value.");
}

bool PolygonBasedTrackType::needsReparse(
		const boost::program_options::variables_map& /*variablesMap*/,
		const std::vector<std::string>& /*args*/) {
	if (polygonType) {
		return false;
	} else {
		polygonType = trackArgumentParser::getPolygonType(polygonTypeName);
		return true;
	}
}

boost::program_options::options_description PolygonBasedTrackType::getOptions() {
	if (polygonType) {
		return polygonType->getOptions();
	} else {
		return BasicTrackType::getOptions();
	}
}


}} /* namespace car::track */

