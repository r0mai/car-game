#ifndef TRACKTYPE_HPP_
#define TRACKTYPE_HPP_

#include <istream>
#include <ostream>

namespace car {

enum class TrackType {
	circle,
	zigzag,
	curvy
};

std::istream& operator>>(std::istream& is, TrackType& trackType);
std::ostream& operator<<(std::ostream& os, TrackType trackType);

}




#endif /* TRACKTYPE_HPP_ */
