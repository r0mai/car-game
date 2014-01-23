
#include "Telemetry.hpp"

#include <algorithm>

namespace car {

void Telemetry::addDataPoint(const irr::core::vector2df& point) {
	dataPoints.push_back(point);
}

void Telemetry::drawAsGraph(irr::video::IVideoDriver *driver, const irr::core::recti& position) {

	using namespace irr;

	driver->draw2DRectangle(video::SColor(50, 255, 50, 255), position);

	if (dataPoints.size() < 2) {
		return;
	}
	f32 maxUp = position.UpperLeftCorner.Y; //d
	f32 minUp = position.LowerRightCorner.Y; //c
	f32 leftSide = position.UpperLeftCorner.X;

	f32 maxData = maxBound; //b
	f32 minData = minBound; //a

   	if (automaticBoundsDetection) {
		minData = std::min_element(dataPoints.begin(), dataPoints.end(),
			[](const irr::core::vector2df& lhs, const irr::core::vector2df& rhs) { return lhs.Y < rhs.Y; })->Y;
		maxData = std::max_element(dataPoints.begin(), dataPoints.end(),
			[](const irr::core::vector2df& lhs, const irr::core::vector2df& rhs) { return lhs.Y < rhs.Y; })->Y;
	}

	if ( minData - maxData == 0.0 ) {
		return;
	}

	core::vector2di lastPoint(leftSide + dataPoints[0].X*10, (-maxData*minUp + minData*maxUp + (minUp - maxUp)*dataPoints[0].Y) / (minData - maxData));

	for ( u32 i = 1; i < dataPoints.size(); ++i ) {
		core::vector2di currentPoint(leftSide + dataPoints[i].X*10, (-maxData*minUp + minData*maxUp + (minUp - maxUp)*dataPoints[i].Y) / (minData - maxData));
		driver->draw2DLine(lastPoint, currentPoint);
		lastPoint = currentPoint;
	}
}

void Telemetry::setAutomaticBoundsDetection(bool value) {
	automaticBoundsDetection = value;
}

void Telemetry::setBounds(irr::f32 min, irr::f32 max) {
	minBound = min;
	maxBound = max;
}

}

