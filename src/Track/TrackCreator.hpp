#ifndef SRC_TRACK_TRACKCREATOR_HPP
#define SRC_TRACK_TRACKCREATOR_HPP

#include <functional>
#include <vector>

namespace car { namespace track {

class Track;

using TrackCreator = std::function<Track()>;
using TrackCreators = std::vector<TrackCreator>;

}} // namespace car::track


#endif /* SRC_TRACK_TRACKCREATOR_HPP */
