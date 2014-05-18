#include "createCircleTrack.hpp"
#include <boost/math/constants/constants.hpp>

namespace car { namespace track {

Track createCircleTrack(const CircleTrackParams& params) {
	using namespace boost::math::float_constants;

	Track track;
	float increment = 2*pi/params.resolution;
	for ( int i = 0; i < params.resolution; ++i ) {
		track.addLine(Line2f(
					params.innerRadius*std::cos((i-1)*increment),
					params.innerRadius*std::sin((i-1)*increment),
					params.innerRadius*std::cos((i)*increment),
					params.innerRadius*std::sin((i)*increment)));
		track.addLine(Line2f(
					params.outerRadius*std::cos((i-1)*increment),
					params.outerRadius*std::sin((i-1)*increment),
					params.outerRadius*std::cos((i)*increment),
					params.outerRadius*std::sin((i)*increment)));
	}

	increment = 2*pi/params.numberOfCheckpoints;
	for ( int i = 0; i < params.numberOfCheckpoints; ++i ) {
		track.addCheckpoint(Line2f(
				params.innerRadius*std::cos((i)*increment),
				-params.innerRadius*std::sin((i)*increment),
				params.outerRadius*std::cos((i)*increment),
				-params.outerRadius*std::sin((i)*increment)
			));
	}

	track.setOrigin({0.f, (params.innerRadius + params.outerRadius)/2.f}, 0.f);

	return track;
}


}} /* namespace car::track */


