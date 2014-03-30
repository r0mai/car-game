#ifndef TELEMETRY_HPP
#define TELEMETRY_HPP

#include <vector>

#include <SFML/Graphics.hpp>

namespace car {

class Telemetry {
public:

	void addDataPoint(const sf::Vector2f& point);
	void drawAsGraph(sf::RenderWindow& driver, const sf::FloatRect& position, const sf::Color& color = sf::Color::White);

	void setAutomaticBoundsDetection(bool value);
	void setBounds(float min, float max);
private:
	bool automaticBoundsDetection = true;
	float minBound = 0.0;
	float maxBound = 1.0;
	std::vector<sf::Vector2f> dataPoints;
};

}

#endif /* !TELEMETRY_HPP */
