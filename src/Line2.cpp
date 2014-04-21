#include "Line2.hpp"
#include "LineIntersection.hpp"

namespace car {

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
bool isParallel(const Line2<T>& line1, const Line2<T>& line2) {
	return LineIntersection<T>{line1, line2}.isParallel();
}

} //  unnamed namespace

bool intersects(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr) {
	return intersects<float>(line1, line2, outPtr);
}

bool intersectsInfinite(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr) {
	return intersectsInfinite<float>(line1, line2, outPtr);
}

bool isParallel(const Line2f& line1, const Line2f& line2) {
	return isParallel<float>(line1, line2);
}

} // namespace car


