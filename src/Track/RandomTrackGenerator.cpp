#include "RandomTrackGenerator.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <algorithm>
#include "Track.hpp"
#include "mathUtil.hpp"
#include "RandomTrackException.hpp"
#include "Line2.hpp"

namespace car { namespace track {

Track RandomTrackGenerator::operator()(uint seed) const {
	boost::random::mt19937 rng{seed};
	for (int i = 0; i < params.maxTries; ++i) {
		Track track = params.generator(params.polygonGenerator(rng));
		try {
			track.check();
		} catch (TrackError&) {
			continue;
		}

		return track;
	}

	throw RandomTrackException{"Track generation failed after maximum number of tries"};
}

}} /* namespace car::track */
