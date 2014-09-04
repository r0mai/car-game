#ifndef DRAWUTIL_HPP
#define DRAWUTIL_HPP

#include <SFML/Graphics.hpp>

#include "Line2.hpp"

namespace car {

inline
void drawLine(sf::RenderWindow& window, const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& color = sf::Color::White) {
	sf::Vertex v[] = { sf::Vertex(a, color), sf::Vertex(b, color) };
	window.draw(v, 2, sf::Lines);
}

inline
void drawLine(sf::RenderWindow& window, const Line2f& line, const sf::Color& color = sf::Color::White) {
	drawLine(window, line.start, line.end, color);
}

template <typename Integer>
inline Integer averageColorComponent(Integer first, Integer second, float ratio) {
	ratio = std::max(0.f, std::min(1.f, ratio));
	float result = first * (1.f - ratio) + second * ratio;
	return std::max(std::numeric_limits<Integer>::min(),
			std::min(std::numeric_limits<Integer>::max(),
				static_cast<Integer>(result)
			));
}

inline
sf::Color averageColor(const sf::Color& color1, const sf::Color& color2, float ratio) {
	return sf::Color{
		averageColorComponent(color1.r, color2.r, ratio),
		averageColorComponent(color1.g, color2.g, ratio),
		averageColorComponent(color1.b, color2.b, ratio),
		averageColorComponent(color1.a, color2.a, ratio)};
}

}

#endif /* !DRAWUTIL_HPP */
