
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

	auto it = dataPoints.begin();
	float horizontalScaling = 10;

	if (scrolling) {
		auto firstPositionToDraw = dataPoints.back().x - position.width / horizontalScaling;
		it = std::find_if(dataPoints.begin(), dataPoints.end(),
				[firstPositionToDraw](const sf::Vector2f& value)
				{ return value.x >= firstPositionToDraw; });
		if (it == dataPoints.end()) {
			// this should never happen
			return;
		}
	}

	auto startingPoint = it->x;

	sf::Vector2f lastPoint(leftSide,
			(-maxData*minUp + minData*maxUp + (minUp - maxUp)*it->y) / (minData - maxData));

	for ( ++it; it != dataPoints.end(); ++it ) {
		sf::Vector2f currentPoint(
				leftSide + (it->x - startingPoint) * horizontalScaling,
				(-maxData*minUp + minData*maxUp + (minUp - maxUp)*it->y) / (minData - maxData));
		drawLine(window, lastPoint, currentPoint, color);
		lastPoint = currentPoint;
	}
}

void Telemetry::setAutomaticBoundsDetection(bool value) {
	automaticBoundsDetection = value;
}

void Telemetry::setScrolling(bool value) {
	scrolling = value;
}

void Telemetry::setBounds(float min, float max) {
	minBound = min;
	maxBound = max;
}

}

