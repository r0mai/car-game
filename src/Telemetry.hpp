#ifndef TELEMETRY_HPP
#define TELEMETRY_HPP

#include <irrlicht/irrlicht.h>
#include <vector>

namespace car {

class Telemetry {
public:

	void addDataPoint(const irr::core::vector2df& point);
	void drawAsGraph(irr::video::IVideoDriver *driver, const irr::core::recti& position);

	void setAutomaticBoundsDetection(bool value);
	void setBounds(irr::f32 min, irr::f32 max);
private:
	bool automaticBoundsDetection = true;
	irr::f32 minBound = 0.0;
	irr::f32 maxBound = 1.0;
	std::vector<irr::core::vector2df> dataPoints;
};

}

#endif /* !TELEMETRY_HPP */
