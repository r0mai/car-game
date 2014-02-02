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


}

#endif /* !DRAWUTIL_HPP */
