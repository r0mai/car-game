#ifndef SRC_TRACK_TRACKCREATORERROR_HPP
#define SRC_TRACK_TRACKCREATORERROR_HPP

#include "OptionParseError.hpp"

namespace car { namespace track {

struct TrackCreatorError: OptionParseError {
	using OptionParseError::OptionParseError;
};


}} /* namespace car::track */

#endif /* SRC_TRACK_TRACKCREATORERROR_HPP */
