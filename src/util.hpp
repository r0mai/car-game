#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

namespace car {

template<class T, class U>
sf::Vector2<T> vectorCast(const sf::Vector2<U>& v) {
	return sf::Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
}

inline
void drawLine(sf::RenderWindow& window, const sf::Vector2f& a, const sf::Vector2f& b) {
	sf::Vertex v[] = { sf::Vertex(a), sf::Vertex(b) };
	window.draw(v, 2, sf::Lines);
}

template<class T>
T getLength(const sf::Vector2<T>& v) {
	return std::sqrt(v.x*v.x + v.y*v.y);
}

template<class T>
T getLengthSQ(const sf::Vector2<T>& v) {
	return v.x*v.x + v.y*v.y;
}

template<class T>
sf::Vector2<T> normalize(const sf::Vector2<T>& v) {
	T magnitude = getLength(v);
	if (magnitude == T(0)) {
		return sf::Vector2<T>(T(0), T(0));
	}
	return v / magnitude;
}

}

#endif /* !UTIL_HPP */
