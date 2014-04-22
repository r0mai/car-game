#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <cmath>
#include <cassert>

#include <SFML/Graphics.hpp>

namespace car {

constexpr float ROUNDING_ERROR_float = 0.00001f;

template<class T, class U>
sf::Vector2<T> vectorCast(const sf::Vector2<U>& v) {
	return sf::Vector2<T>(static_cast<T>(v.x), static_cast<T>(v.y));
}

template<class T>
T getLengthSQ(const sf::Vector2<T>& v) {
	return v.x*v.x + v.y*v.y;
}

template<class T>
T getLength(const sf::Vector2<T>& v) {
	return std::sqrt(getLengthSQ(v));
}

template<class T>
T getDistanceSQ(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
	return getLengthSQ(b-a);
}

template<class T>
T getDistance(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
	return getLength(b-a);
}

template<class T>
sf::Vector2<T> normalize(const sf::Vector2<T>& v) {
	T magnitude = getLength(v);
	if (magnitude == T(0)) {
		return sf::Vector2<T>(T(0), T(0));
	}
	return v / magnitude;
}

template<class T>
T sigmoidApproximation(T x) {
	return x / (1 + std::abs(x));
}

template<class T>
T clamp(T x, T min, T max) {
	assert(max >= min);
	if (x > max) {
		return max;
	}
	if (x < min) {
		return min;
	}
	return x;
}

template<class T>
sf::Vector2<T> rotateClockwise(const sf::Vector2<T>& v) {
	return sf::Vector2<T>(-v.y, v.x);
}

template<class T>
sf::Vector2<T> rotateCounterclockwise(const sf::Vector2<T>& v) {
	return sf::Vector2<T>(v.y, -v.x);
}

inline bool equals(const float a, const float b, const float tolerance = ROUNDING_ERROR_float) {
	return (a + tolerance >= b) && (a - tolerance <= b);
}

}

#endif /* !MATHUTIL_HPP */
