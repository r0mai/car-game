
#include "Telemetry.hpp"

#include <algorithm>

#include "drawUtil.hpp"

namespace car {

void Telemetry::addDataPoint(const sf::Vector2f& point) {
	dataPoints.push_back(point);
}

void Telemetry::drawAsGraph(sf::RenderWindow& window, const sf::FloatRect& position, const sf::Color& color) {

	if (dataPoints.size() < 2) {
		return;
	}

	float maxUp = position.top; //d
	float minUp = position.top + position.height; //c
	float leftSide = position.left;

	float maxData = maxBound; //b
	float minData = minBound; //a

   	if (automaticBoundsDetection) {
		minData = std::min_element(dataPoints.begin(), dataPoints.end(),
			[](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.y < rhs.y; })->y;
		maxData = std::max_element(dataPoints.begin(), dataPoints.end(),
			[](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs.y < rhs.y; })->y;
	}

	if ( minData - maxData == 0.0 ) {
		return;
	}


	sf::Vector2f lastPoint(leftSide + dataPoints[0].x*10, (-maxData*minUp + minData*maxUp + (minUp - maxUp)*dataPoints[0].y) / (minData - maxData));

	for ( unsigned i = 1; i < dataPoints.size(); ++i ) {
		sf::Vector2f currentPoint(leftSide + dataPoints[i].x*10, (-maxData*minUp + minData*maxUp + (minUp - maxUp)*dataPoints[i].y) / (minData - maxData));
		drawLine(window, lastPoint, currentPoint, color);
		lastPoint = currentPoint;
	}
}

void Telemetry::setAutomaticBoundsDetection(bool value) {
	automaticBoundsDetection = value;
}

void Telemetry::setBounds(float min, float max) {
	minBound = min;
	maxBound = max;
}

}

