#include "Line2.hpp"
#include "LineIntersection.hpp"

namespace car {

namespace detail {

namespace {

template <typename T>
bool intersectsInfinite(const Line2<T>& line1, const Line2<T>& line2, sf::Vector2<T> *outPtr) {
	LineIntersection<T> data{line1, line2};

	if (data.isCoincident()) {
		if (outPtr) {
			*outPtr = line1.start;
		}
		return true;
	}

	auto intersectionPoint = data.getIntersectionPoint();
	if (!intersectionPoint) {
		return false;
	}

	if (outPtr) {
		*outPtr = *intersectionPoint;
	}

	return true;
}

template <typename T>
bool intersects(const Line2<T>& line1, const Line2<T>& line2, sf::Vector2<T> *outPtr) {
	LineIntersection<T> data{line1, line2};

	auto overlapLine = data.getOverlapLine();
	if (overlapLine) {
		if ( outPtr ) {
			*outPtr = (overlapLine->start + overlapLine->end) / static_cast<T>(2);
		}

		return true;
	}

	auto intersectionPoint = data.getIntersectionPoint();
	if (!intersectionPoint) {
		return false;
	}

	auto uA = data.getIntersectionPointRatioLine1();
	auto uB = data.getIntersectionPointRatioLine2();

	if(uA < 0.f || uA > 1.f || uB < 0.f || uB > 1.f) {
		return false; // Outside the line segment
	}

	if ( outPtr ) {
		*outPtr = *intersectionPoint;
	}

	return true;
}

template <typename T>
bool intersectsRay(const Line2<T>& line, const sf::Vector2<T>& origin,
		const sf::Vector2<T>& direction, sf::Vector2<T> *outPtr) {
	LineIntersection<T> data{line, Line2<T>{origin, origin+direction}};

	auto overlapLine = data.getOverlapLine();
	if (overlapLine) {
		//TODO this is not good like this, half point is not the best
		if ( outPtr ) {
			*outPtr = (overlapLine->start + overlapLine->end) / static_cast<T>(2);
		}

		return true;
	}

	auto intersectionPoint = data.getIntersectionPoint();
	if (!intersectionPoint) {
		return false;
	}

	auto uA = data.getIntersectionPointRatioLine1();
	auto uB = data.getIntersectionPointRatioLine2();

	if(uA < 0.f || uA > 1.f || uB < 0.f) {
		return false; // Outside the line segment
	}

	if ( outPtr ) {
		*outPtr = *intersectionPoint;
	}

	return true;

}

template <typename T>
bool isParallel(const Line2<T>& line1, const Line2<T>& line2) {
	return LineIntersection<T>{line1, line2}.isParallel();
}

template <typename T>
sf::Vector2<T> nearestPoint(const sf::Vector2<T>& point, const Line2<T>& line) {
	if (equals(line.start.x, line.end.x) && equals(line.start.y, line.end.y)) {
		return line.start;
	}

	auto orthogonalDirection = rotateClockwise(line.end - line.start);
	LineIntersection<T> intersection{line, {point, point + orthogonalDirection}};
	auto intersectionPoint = intersection.getIntersectionPoint();
	assert(intersectionPoint);
	auto ratio = intersection.getIntersectionPointRatioLine1();

	if (ratio < 0.f) {
		return line.start;
	}

	if (ratio > 1.f) {
		return line.end;
	}

	return *intersectionPoint;
}

} //  unnamed namespace

bool intersects(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr) {
	return intersects<float>(line1, line2, outPtr);
}

bool intersectsRay(const Line2f& line, const sf::Vector2f& origin, const sf::Vector2f& direction, sf::Vector2f *outPtr) {
	return intersectsRay<float>(line, origin, direction, outPtr);
}

} // namespace detail


bool intersectsInfinite(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr) {
	return detail::intersectsInfinite<float>(line1, line2, outPtr);
}

bool isParallel(const Line2f& line1, const Line2f& line2) {
	return detail::isParallel<float>(line1, line2);
}

sf::Vector2f nearestPoint(const sf::Vector2f& point, const Line2f& line) {
	return detail::nearestPoint<float>(point, line);
}

} // namespace car


