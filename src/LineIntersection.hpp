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
		return intersectionData ? intersectionData->uA : 0.f;
	}

	T getIntersectionPointRatioLine2() const {
		return intersectionData ? intersectionData->uB : 0.f;
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

	void calculateCoincidence(const Line2<T>& line1, const Line2<T>& line2) {
		Line2<T> result;

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
			// find the points which are not contributing to the
			// common part
			if ((line2.start.x>=line1.start.x && line2.start.x>=line1.end.x && line2.start.x>=line2.end.x) ||
					(line2.start.y>=line1.start.y && line2.start.y>=line1.end.y && line2.start.y>=line2.end.y))
				result.end=line2.start;
			else if ((line2.end.x>=line1.start.x && line2.end.x>=line1.end.x && line2.end.x>=line2.start.x) ||
					(line2.end.y>=line1.start.y && line2.end.y>=line1.end.y && line2.end.y>=line2.start.y))
				result.end=line2.end;
			else if ((line1.start.x>=line2.start.x && line1.start.x>=line1.end.x && line1.start.x>=line2.end.x) ||
					(line1.start.y>=line2.start.y && line1.start.y>=line1.end.y && line1.start.y>=line2.end.y))
				result.end=line1.start;
			else
				result.end=line1.end;

			if (result.end != line2.start && ((line2.start.x<=line1.start.x && line2.start.x<=line1.end.x && line2.start.x<=line2.end.x) ||
					(line2.start.y<=line1.start.y && line2.start.y<=line1.end.y && line2.start.y<=line2.end.y)))
				result.start=line2.start;
			else if (result.end != line2.end && ((line2.end.x<=line1.start.x && line2.end.x<=line1.end.x && line2.end.x<=line2.start.x) ||
					(line2.end.y<=line1.start.y && line2.end.y<=line1.end.y && line2.end.y<=line2.start.y)))
				result.start=line2.end;
			else if (result.end != line1.start && ((line1.start.x<=line2.start.x && line1.start.x<=line1.end.x && line1.start.x<=line2.end.x)
					|| (line1.start.y<=line2.start.y && line1.start.y<=line1.end.y && line1.start.y<=line2.end.y)))
				result.start=line1.start;
			else
				result.start=line1.end;

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
