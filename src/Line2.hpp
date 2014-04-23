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

//code based on irrlicht line2d::intersectWith
bool intersects(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr = 0);
bool intersectsInfinite(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr = 0);
bool intersectsRay(const Line2f& line, const sf::Vector2f& origin, const sf::Vector2f& direction, sf::Vector2f *outPtr = 0);
bool isParallel(const Line2f& line1, const Line2f& line2);
sf::Vector2f nearestPoint(const sf::Vector2f& point, const Line2f& line);


}

#endif /* !LINE_HPP */
