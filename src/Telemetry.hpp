#ifndef TELEMETRY_HPP
#define TELEMETRY_HPP

#include <irrlicht/irrlicht.h>
#include <vector>

namespace car {

class Telemetry {
public:

	void addDataPoint(const irr::core::vector2df& point);
	void drawAsGraph(irr::video::IVideoDriver *driver, const irr::core::recti& position);
private:
	std::vector<irr::core::vector2df> dataPoints;
};

}

#endif /* !TELEMETRY_HPP */
