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
#include "SimplePolygonType.hpp"
#include "PointAdderRandomPolygonGeneratorType.hpp"
#include "RandomWalkPolygonGeneratorType.hpp"
#include "LazyArgumentMap.hpp"

namespace algo = boost::algorithm;
namespace po = boost::program_options;

namespace car { namespace track {

namespace trackArgumentParser {

namespace {

template <typename Type>
std::pair<std::string, std::shared_ptr<Type>>
createTypeElement() {
	auto element = std::make_shared<Type>();
	auto argumentName = element->getArgumentName();
	return {std::move(argumentName), std::move(element)};
}

LAZY_ARGUMENT_MAP(ITrackType, trackTypes) {
	return {
		createTypeElement<CircleTrackType>(),
		createTypeElement<PolygonTrackType>(),
		createTypeElement<RandomTrackType>(),
	};
}

LAZY_ARGUMENT_MAP(IPolygonType, polygonTypes) {
	return {
		createTypeElement<SimplePolygonType>(),
	};
}

const std::map<std::string, std::shared_ptr<IRandomPolygonGeneratorType>>
		randomPolygonGeneratorTypes{
	createTypeElement<PointAdderRandomPolygonGeneratorType>(),
	createTypeElement<RandomWalkPolygonGeneratorType>(),
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

	auto it = trackTypes().find(trackTypeName);
	if (it == trackTypes().end()) {
		throw TrackCreatorError{"Invalid track type: " + trackTypeName};
	}

	std::vector<std::string> args(++tokens.begin(), tokens.end());
	auto& trackType = *it->second;

	if (args.size() < trackType.getMinimumNumberOfArgs()) {
		throw TrackCreatorError{"Too few tokens for track type " + it->first};
	}

	po::variables_map variablesMap;
	do {
		auto optionsDescription = trackType.getOptions();
		auto parsedOptions = po::parse_config_file<char>(filename.c_str(), optionsDescription, true);
		po::store(parsedOptions, variablesMap);
		po::notify(variablesMap);
	} while (trackType.needsReparse(variablesMap, args));

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

std::shared_ptr<IPolygonType> getPolygonType(const std::string& name) {
	auto it = polygonTypes().find(name);
	if (it == polygonTypes().end()) {
		throw TrackCreatorError{"Invalid polygon type: " + name};
	}

	return it->second;
}

std::shared_ptr<IRandomPolygonGeneratorType>
getRandomPolygonGeneratorType(const std::string& name) {
	auto it = randomPolygonGeneratorTypes.find(name);
	if (it == randomPolygonGeneratorTypes.end()) {
		throw TrackCreatorError{"Invalid random polygon generator type: " + name};
	}

	return it->second;
}

std::string getHelpString() {
	std::ostringstream ss;
	ss << "Allowed track types: " <<
			algo::join(trackTypes() | boost::adaptors::map_keys, ", ") << ".\n";

	for (const auto& trackType: trackTypes()) {
		ss << "\nFormat of track type " << trackType.first << ":\n" <<
				trackType.second->getHelpString();
	}

	ss << "\nAllowed polygon types: " <<
			algo::join(polygonTypes() | boost::adaptors::map_keys, ", ") << ".\n";

	for (const auto& polygonType: polygonTypes()) {
		ss << "\nFormat of polygon type " << polygonType.first << ":\n" <<
				polygonType.second->getHelpString();
	}

	ss << "\nAllowed random polygon generator types: " <<
			algo::join(randomPolygonGeneratorTypes | boost::adaptors::map_keys, ", ") << ".\n";

	for (const auto& randomPolygonGeneratorType: randomPolygonGeneratorTypes) {
		ss << "\nFormat of random polygon generator type " <<
				randomPolygonGeneratorType.first << ":\n" <<
				randomPolygonGeneratorType.second->getHelpString();
	}


	return ss.str();
}

} /* namespace trackArgumentParser */
}} /* namespace car::track */
