#include "PolygonTrackType.hpp"
#include <sstream>
#include <boost/range/adaptor/transformed.hpp>
#include "optionsUtil.hpp"
#include "Track.hpp"

namespace po = boost::program_options;

namespace car { namespace track {

PolygonTrackType::PolygonTrackType():PolygonBasedTrackType{"polygon"} {
	optionsDescription.add_options()
			("p", po::value(&points),
					"The points of the polygon. Each point is one argument with the format x,y")
			;
}

std::function<Track()> PolygonTrackType::getTrackCreator(
		const boost::program_options::variables_map& variablesMap,
		const std::vector<std::string>& /*args*/) {
	assert(getPolygonType());
	auto pointParams = points | boost::adaptors::transformed(parsePoint);
	std::vector<sf::Vector2f> points(pointParams.begin(), pointParams.end());
	return std::bind(getPolygonType()->getTrackCreator(variablesMap), points);
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


}} /* namespace car::track */

