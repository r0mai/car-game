#ifndef LINEINTERSECTION_HPP_
#define LINEINTERSECTION_HPP_

#include <boost/optional.hpp>
#include "Line2.hpp"
#include "mathUtil.hpp"

namespace car {

template <typename T>
class LineIntersection {
public:
	LineIntersection(const Line2<T>& line1, const Line2<T>& line2):
			parallel{false}, coincident{false} {

		calculateLineData(line1, line2);

		if(equals(commonDenominator, 0.f))
		{
			parallel = true;
			// The lines are either coincident or parallel
			// if both numerators are 0, the lines are coincident
			if(equals(numeratorA, 0.f) && equals(numeratorB, 0.f))
			{
				coincident = true;
				calculateCoincidence(line1, line2);
			}

			return;
		}

		calculateIntersectionPoint(line1, line2);
	}

	boost::optional<sf::Vector2<T>> getIntersectionPoint() const {
		if (intersectionData) {
			return intersectionData->intersectionPoint;
		} else {
			return {};
		}
	}

	T getIntersectionPointRatioLine1() const {
		return intersectionData ? intersectionData->uB : 0.f;
	}

	T getIntersectionPointRatioLine2() const {
		return intersectionData ? intersectionData->uA : 0.f;
	}

	const boost::optional<Line2<T>>& getOverlapLine() const {
		return overlapLine;
	}

	bool isParallel() const {
		return parallel;
	}

	bool isCoincident() const {
		return coincident;
	}

private:
	T commonDenominator;
	T numeratorA;
	T numeratorB;

	struct IntersectionData {
		sf::Vector2<T> intersectionPoint;
		T uA;
		T uB;
	};

	boost::optional<IntersectionData> intersectionData;
	boost::optional<Line2<T>> overlapLine;
	bool parallel;
	bool coincident;

	void calculateLineData(const Line2<T>& line1, const Line2<T>& line2) {
		commonDenominator = (line1.end.y - line1.start.y)*(line2.end.x - line2.start.x) -
									(line1.end.x - line1.start.x)*(line2.end.y - line2.start.y);

		numeratorA = (line1.end.x - line1.start.x)*(line2.start.y - line1.start.y) -
							(line1.end.y - line1.start.y)*(line2.start.x -line1.start.x);

		numeratorB = (line2.end.x - line2.start.x)*(line2.start.y - line1.start.y) -
							(line2.end.y - line2.start.y)*(line2.start.x -line1.start.x);
	}

	static T getCoincidenceByCoordinate(const Line2<T>& line1, const Line2<T>& line2,
			T sf::Vector2<T>::*c, T t2) {
		T t1v1 = (line2.start.*c * (1 - t2) + line2.end.*c * t2 - line1.start.*c);
		T v1 = (line1.end.*c - line1.start.*c);
		if (v1 > 0) {
			if (t1v1 < 0) t1v1 = 0;
			if (t1v1 > v1) t1v1 = v1;
		} else {
			if (t1v1 > 0) t1v1 = 0;
			if (t1v1 < v1) t1v1 = v1;
		}
		return line1.start.*c + t1v1;
	}

	void calculateCoincidence(const Line2<T>& line1, const Line2<T>& line2) {

		// now check if the two segments are disjunct
		if (line1.start.x>line2.start.x && line1.end.x>line2.start.x && line1.start.x>line2.end.x && line1.end.x>line2.end.x)
			return;
		else if (line1.start.y>line2.start.y && line1.end.y>line2.start.y && line1.start.y>line2.end.y && line1.end.y>line2.end.y)
			return;
		else if (line1.start.x<line2.start.x && line1.end.x<line2.start.x && line1.start.x<line2.end.x && line1.end.x<line2.end.x)
			return;
		else if (line1.start.y<line2.start.y && line1.end.y<line2.start.y && line1.start.y<line2.end.y && line1.end.y<line2.end.y)
			return;
		// else the lines are overlapping to some extent
		else {
			Line2<T> result;
			result.start.x = getCoincidenceByCoordinate(line1, line2, &sf::Vector2<T>::x, 0);
			result.start.y = getCoincidenceByCoordinate(line1, line2, &sf::Vector2<T>::y, 0);
			result.end.x = getCoincidenceByCoordinate(line1, line2, &sf::Vector2<T>::x, 1);
			result.end.y = getCoincidenceByCoordinate(line1, line2, &sf::Vector2<T>::y, 1);

			overlapLine = result;
		}
	}

	void calculateIntersectionPoint(const Line2<T>& /*line1*/, const Line2<T>& line2) {
		IntersectionData result;

		result.uA = numeratorA / commonDenominator;
		result.uB = numeratorB / commonDenominator;

		result.intersectionPoint.x = static_cast<T>(line2.start.x + result.uA * (line2.end.x - line2.start.x));
		result.intersectionPoint.y = static_cast<T>(line2.start.y + result.uA * (line2.end.y - line2.start.y));

		intersectionData = result;
	}
};

} /* namespace car */
#endif /* LINEINTERSECTION_HPP_ */
