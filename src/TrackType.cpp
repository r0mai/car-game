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
	} else {
		throw std::logic_error{"Invalid track type"};
	}

	return is;
}

std::ostream& operator<<(std::ostream& os, TrackType trackType) {
	switch (trackType) {
	case TrackType::circle: return os << "circle";
	case TrackType::zigzag: return os << "zigzag";
	default: return os;
	}
}

}



