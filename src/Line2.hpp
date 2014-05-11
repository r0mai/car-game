#ifndef LINE_HPP
#define LINE_HPP

#include <SFML/System/Vector2.hpp>

#include "mathUtil.hpp"

namespace car {

template<class T>
struct Line2 {

	Line2() = default;
	Line2(const sf::Vector2<T>& start, const sf::Vector2<T>& end) : start(start), end(end) {}

	Line2(T xa, T ya, T xb, T yb) : start(xa, ya), end(xb, yb) {}

	sf::Vector2<T> start;
	sf::Vector2<T> end;
};

typedef Line2<float> Line2f;

namespace detail {
bool intersects(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr);
bool intersectsRay(const Line2f& line, const sf::Vector2f& origin, const sf::Vector2f& direction, sf::Vector2f *outPtr);

inline bool isWrongDirection(float direction, float origin, float start, float end) {
	if (direction > 0) {
		return start < origin && end < origin;
	} else {
		return start > origin && end > origin;
	}
}

inline bool isOutsideRange(float origin1, float origin2, float start, float end) {
	auto originMin = std::min(origin1, origin2);
	auto originMax = std::max(origin1, origin2);
	return (start < originMin && end < originMin) || (start > originMax && end > originMax);
}


}

bool intersectsInfinite(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr = 0);
bool isParallel(const Line2f& line1, const Line2f& line2);
sf::Vector2f nearestPoint(const sf::Vector2f& point, const Line2f& line);

inline
bool intersects(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr = 0) {
	if (detail::isOutsideRange(line1.start.x, line1.end.x, line2.start.x, line2.end.x) ||
			detail::isOutsideRange(line1.start.y, line1.end.y, line2.start.y, line2.end.y)) {
		return false;
	}
	return detail::intersects(line1, line2, outPtr);
}

inline
bool intersectsRay(const Line2f& line, const sf::Vector2f& origin,
		const sf::Vector2f& direction, sf::Vector2f *outPtr = 0) {
	if (detail::isWrongDirection(direction.x, origin.x, line.start.x, line.end.x) ||
			detail::isWrongDirection(direction.y, origin.y, line.start.y, line.end.y)) {
		return false;
	}

	return detail::intersectsRay(line, origin, direction, outPtr);
}


}

#endif /* !LINE_HPP */
