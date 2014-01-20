
#include "Telemetry.hpp"

#include <algorithm>

namespace car {

void Telemetry::addDataPoint(const irr::core::vector2df& point) {
	dataPoints.push_back(point);
}

void Telemetry::drawAsGraph(irr::video::IVideoDriver *driver, const irr::core::recti& position) {

	using namespace irr;

	driver->draw2DRectangle(video::SColor(50, 255, 50, 255), position);

	if (dataPoints.empty()) {
		return;
	}
	f32 maxUp = position.UpperLeftCorner.Y; //d
	f32 minUp = position.LowerRightCorner.Y; //c
	f32 leftSide = position.UpperLeftCorner.X;

	//b
	f32 maxData = std::max_element(dataPoints.begin(), dataPoints.end(),
			[](const irr::core::vector2df& lhs, const irr::core::vector2df& rhs) { return lhs.Y < rhs.Y; })->Y;
	//a
	f32 minData = std::min_element(dataPoints.begin(), dataPoints.end(),
			[](const irr::core::vector2df& lhs, const irr::core::vector2df& rhs) { return lhs.Y < rhs.Y; })->Y;

	if ( minData - maxData == 0.0 ) {
		return;
	}

	for ( const auto& point : dataPoints ) {
		s32 x = leftSide + point.X*10;
		s32 y = (-maxData*minUp + minData*maxUp + (minUp - maxUp)*point.Y) / (minData - maxData);
		if ( x > 0 && y > 0 ) {
			driver->drawPixel(x, y, video::SColor(0, 255, 0, 255));
		}
	}
}

}

