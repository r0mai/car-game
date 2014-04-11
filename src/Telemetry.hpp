#ifndef TELEMETRY_HPP
#define TELEMETRY_HPP

#include <deque>

#include <SFML/Graphics.hpp>

namespace car {

class Telemetry {
public:

	void addDataPoint(const sf::Vector2f& point);
	void drawAsGraph(sf::RenderWindow& driver, const sf::FloatRect& position, const sf::Color& color = sf::Color::White);

	void setAutomaticBoundsDetection(bool value);
	void setScrolling(bool value);
	void setBounds(float min, float max);
private:
	bool automaticBoundsDetection = true;
	bool scrolling = true;
	float minBound = 0.0;
	float maxBound = 1.0;
	std::deque<sf::Vector2f> dataPoints;

	static float horizontalScaling;
	static float horizontalResolution;
	static std::size_t maxNumberOfPoints;
};

}

#endif /* !TELEMETRY_HPP */
