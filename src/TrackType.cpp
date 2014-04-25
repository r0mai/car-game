#include "TrackType.hpp"
#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

namespace car {

std::istream& operator>>(std::istream& is, TrackType& trackType) {
	std::string s;
	is >> s;
	if (boost::algorithm::iequals(s, std::string{"circle"})) {
		trackType = TrackType::circle;
	} else if (boost::algorithm::iequals(s, std::string{"zigzag"})) {
		trackType = TrackType::zigzag;
	} else if (boost::algorithm::iequals(s, std::string{"curvy"})) {
		trackType = TrackType::curvy;
	} else if (boost::algorithm::iequals(s, std::string{"random"})) {
		trackType = TrackType::random;
	} else if (boost::algorithm::iequals(s, std::string{"random_noseed"})) {
		trackType = TrackType::random_noseed;
	} else {
		throw std::logic_error{"Invalid track type"};
	}

	return is;
}

std::ostream& operator<<(std::ostream& os, TrackType trackType) {
	switch (trackType) {
	case TrackType::circle: return os << "circle";
	case TrackType::zigzag: return os << "zigzag";
	case TrackType::curvy: return os << "curvy";
	case TrackType::random: return os << "random";
	case TrackType::random_noseed: return os << "random_noseed";
	default: return os;
	}
}

}



