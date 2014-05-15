#include "TrackArgumentParser.hpp"

#include <map>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include "Track.hpp"
#include "optionsUtil.hpp"
#include "RandomTrackGenerator.hpp"
#include "PolygonTrackType.hpp"
#include "CircleTrackType.hpp"
#include "RandomTrackType.hpp"

namespace algo = boost::algorithm;
namespace po = boost::program_options;

namespace car {

namespace trackArgumentParser {

namespace {

const std::map<std::string, std::shared_ptr<ITrackType>> trackTypes{
	createTrackTypeElement<CircleTrackType>(),
	createTrackTypeElement<PolygonTrackType>(),
	createTrackTypeElement<RandomTrackType>(),
};

std::function<Track()> parseArgument(const std::string& arg) {
	std::vector<std::string> tokens;
	algo::split(tokens, arg, [](char ch) { return ch == ':'; });

	if (tokens.empty()) {
		throw TrackCreatorError{"Invalid argument: '" + arg + "'"};
	}

	auto filename = tokens[0];

	std::string trackTypeName;
	po::options_description typeDescription;
	typeDescription.add_options()
			("type", po::value(&trackTypeName)->required(), "The type of the track.")
			;

	auto parsedTypeOptions = po::parse_config_file<char>(filename.c_str(), typeDescription, true);
	po::variables_map typeVariablesMap;
	po::store(parsedTypeOptions, typeVariablesMap);
	po::notify(typeVariablesMap);

	auto it = trackTypes.find(trackTypeName);
	if (it == trackTypes.end()) {
		throw TrackCreatorError{"Invalid track type: " + trackTypeName};
	}

	std::vector<std::string> args(++tokens.begin(), tokens.end());
	auto& trackType = *it->second;

	if (args.size() < trackType.getMinimumNumberOfArgs()) {
		throw TrackCreatorError{"Too few tokens for track type " + it->first};
	}

	auto optionsDescription = trackType.getOptions();

	auto parsedOptions = po::parse_config_file<char>(filename.c_str(), optionsDescription, true);
	po::variables_map variablesMap;
	po::store(parsedOptions, variablesMap);
	po::notify(variablesMap);

	return trackType.getTrackCreator(variablesMap, args);
}

}

std::vector<std::function<Track()>>
parseArguments(const std::vector<std::string>& args) {
	if (args.empty()) {
		throw TrackCreatorError{"No tracks specified."};
	}

	std::vector<std::function<Track()>> result;
	result.reserve(args.size());
	boost::transform(args, std::back_inserter(result), parseArgument);
	return result;
}

std::string getHelpString() {
	std::ostringstream ss;
	ss << "Allowed track types: " <<
			algo::join(trackTypes | boost::adaptors::map_keys, ", ") << ".\n";

	for (const auto& trackType: trackTypes) {
		ss << "\nFormat of track type " << trackType.first << ":\n" <<
				trackType.second->getHelpString();
	}

	return ss.str();
}

}
}
