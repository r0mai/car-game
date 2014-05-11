#include "TrackArgumentParser.hpp"

#include <map>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include "Track.hpp"
#include "RandomTrackGenerator.hpp"
#include "PolygonTrackType.hpp"
#include "CircleTrackType.hpp"

namespace algo = boost::algorithm;

namespace car {

namespace trackArgumentParser {

namespace {

const std::map<std::string, std::shared_ptr<ITrackType>> trackTypes{
	createTrackTypeElement<CircleTrackType>(),
	createTrackTypeElement<PolygonTrackType>(),
};

std::function<Track()> parseArgument(const std::string& arg) {
	std::vector<std::string> tokens;
	algo::split(tokens, arg, [](char ch) { return ch == ':'; });

	if (tokens.empty()) {
		throw TrackCreatorError{"Invalid argument: '" + arg + "'"};
	}

	auto it = trackTypes.find(tokens[0]);
	if (it == trackTypes.end()) {
		throw TrackCreatorError{"Invalid track type: " + tokens[0]};
	}

	std::vector<std::string> args(++tokens.begin(), tokens.end());
	auto& trackType = *it->second;

	if (args.size() < trackType.getMinimumNumberOfArgs()) {
		throw TrackCreatorError{"Too few tokens for track type " + it->first};
	}

	return trackType.getTrackCreator(args);
//	switch (it->second) {
//	case TrackType::circle:
//		return createCircleTrack;
//	case TrackType::zigzag:
//		return createZigZagTrack;
//	case TrackType::curvy:
//		return createCurvyTrack;
//	case TrackType::evil:
//		return createEvilTrack;
//	case TrackType::random:
//		RandomTrackGenerator generator{createPolygonTrack{5.f}, 100,
//			parameters.randomTrackPoints, parameters.minRandomTrackWidth, parameters.maxRandomTrackWidth,
//			{-60.f, -60.f}, {60.f, 60.f}};
//
//		if (tokens.size() == 1) {
//			return std::bind(generator, std::rand());
//		} else {
//			return std::bind(generator, boost::lexical_cast<uint>(tokens[1]));
//		}
//	}

	// should have returned by now
	assert(false && "Track type not handled");
	return {};
}

}

std::vector<std::function<Track()>>
parseArguments(const std::vector<std::string>& args) {
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
