#ifndef SRC_TRACK_RANDOMTRACKEXCEPTION_HPP
#define SRC_TRACK_RANDOMTRACKEXCEPTION_HPP

#include <stdexcept>

namespace car { namespace track {

struct RandomTrackException: std::logic_error {
	using std::logic_error::logic_error;
};

}} /* namespace car::track */

#endif /* SRC_TRACK_RANDOMTRACKEXCEPTION_HPP */
