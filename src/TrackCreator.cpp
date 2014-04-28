#include "TrackCreator.hpp"

#include <map>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include "Track.hpp"
#include "RandomTrackGenerator.hpp"
#include "PolygonTrackBuilder.hpp"
#include "TrackType.hpp"

namespace car {

namespace {

const std::map<std::string, TrackType> trackTypes{
	{"circle", TrackType::circle},
	{"zigzag", TrackType::zigzag},
	{"curvy", TrackType::curvy},
	{"random", TrackType::random},
};


}

std::function<Track()> TrackCreator::parseArgument(const std::string& arg) {
	namespace algo = boost::algorithm;

	std::vector<std::string> tokens;
	algo::split(tokens, arg, [](char ch) { return ch == ':'; });

	if (tokens.empty()) {
		throw TrackCreatorError{"Invalid argument: '" + arg + "'"};
	}

	auto it = trackTypes.find(tokens[0]);
	if (it == trackTypes.end()) {
		throw TrackCreatorError{"Invalid track type: " + tokens[0]};
	}

	switch (it->second) {
	case TrackType::circle:
		return createCircleTrack;
	case TrackType::zigzag:
		return createZigZagTrack;
	case TrackType::curvy:
		return createCurvyTrack;
	case TrackType::random:
		RandomTrackGenerator generator{PolygonTrackBuilder{5.f}, 100,
			parameters.randomTrackPoints, parameters.minRandomTrackWidth, parameters.maxRandomTrackWidth,
			{-60.f, -60.f}, {60.f, 60.f}};

		if (tokens.size() == 1) {
			return std::bind(generator, std::rand());
		} else {
			return std::bind(generator, boost::lexical_cast<uint>(tokens[1]));
		}
	}

	// should have returned by now
	assert(false && "Track type not handled");
	return {};
}


std::vector<std::function<Track()>>
TrackCreator::operator()(const std::vector<std::string>& args) {
	std::vector<std::function<Track()>> result;
	result.reserve(args.size());
	boost::transform(args, std::back_inserter(result),
			[this](const std::string& arg) { return parseArgument(arg); });
	return result;
}

}

