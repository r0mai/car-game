#ifndef CREATECIRCLETRACK_HPP_
#define CREATECIRCLETRACK_HPP_

namespace car { namespace track {

class Track;

struct CircleTrackParams {
	float innerRadius = 50.f;
	float outerRadius = 60.f;
	int resolution = 60;
	int numberOfCheckpoints = 72;
};

Track createCircleTrack(const CircleTrackParams& params);

}} /* namespace car::track */



#endif /* CREATECIRCLETRACK_HPP_ */
