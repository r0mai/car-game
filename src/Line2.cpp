#include "Line2.hpp"
#include "mathUtil.hpp"

namespace car {

namespace {

template <typename T>
struct LineData {
	T commonDenominator;
	T numeratorA;
	T numeratorB;
};

template <typename T>
LineData<T> getLineData(const Line2<T>& line1, const Line2<T>& line2) {
	LineData<T> result;
	result.commonDenominator = (float)(line1.end.y - line1.start.y)*(line2.end.x - line2.start.x) -
									(line1.end.x - line1.start.x)*(line2.end.y - line2.start.y);

	result.numeratorA = (float)(line1.end.x - line1.start.x)*(line2.start.y - line1.start.y) -
									(line1.end.y - line1.start.y)*(line2.start.x -line1.start.x);

	result.numeratorB = (float)(line2.end.x - line2.start.x)*(line2.start.y - line1.start.y) -
									(line2.end.y - line2.start.y)*(line2.start.x -line1.start.x);

	return result;
}

template <typename T>
struct IntersectionData {
	sf::Vector2<T> intersectionPoint;
	T uA;
	T uB;
};

template <typename T>
IntersectionData<T> calculateIntersectionPoint(const Line2<T>& /*line1*/, const Line2<T>& line2,
		const LineData<T>& data) {
	IntersectionData<T> result;

	result.uA = data.numeratorA / data.commonDenominator;
	result.uB = data.numeratorB / data.commonDenominator;

	result.intersectionPoint.x = static_cast<T>(line2.start.x + result.uA * (line2.end.x - line2.start.x));
	result.intersectionPoint.y = static_cast<T>(line2.start.y + result.uA * (line2.end.y - line2.start.y));

	return result;
}

template <typename T>
bool intersectsInfinite(const Line2<T>& line1, const Line2<T>& line2, sf::Vector2<T> *outPtr) {
	const auto data = getLineData(line1, line2);

	if(equals(data.commonDenominator, 0.f))
	{
		// The lines are either coincident or parallel
		// if both numerators are 0, the lines are coincident
		if(equals(data.numeratorA, 0.f) && equals(data.numeratorB, 0.f))
		{
			if (outPtr) {
				*outPtr = line1.start;
			}
			return true; // coincident
		}
		return false; // parallel
	}

	if (outPtr) {
		*outPtr = calculateIntersectionPoint(line1, line2, data).intersectionPoint;
	}

	return true;
}

template <typename T>
bool intersects(const Line2<T>& line1, const Line2<T>& line2, sf::Vector2<T> *outPtr) {
	sf::Vector2<T> out;
	const auto data = getLineData(line1, line2);

	if(equals(data.commonDenominator, 0.f))
	{
		// The lines are either coincident or parallel
		// if both numerators are 0, the lines are coincident
		if(equals(data.numeratorA, 0.f) && equals(data.numeratorB, 0.f))
		{
			// Try and find a common line2.endpoint
			if(line1.start == line2.start || line1.end == line2.start)
				out = line2.start;
			else if(line1.end == line2.end || line1.start == line2.end)
				out = line2.end;
			// now check if the two segments are disjunct
			else if (line1.start.x>line2.start.x && line1.end.x>line2.start.x && line1.start.x>line2.end.x && line1.end.x>line2.end.x)
				return false;
			else if (line1.start.y>line2.start.y && line1.end.y>line2.start.y && line1.start.y>line2.end.y && line1.end.y>line2.end.y)
				return false;
			else if (line1.start.x<line2.start.x && line1.end.x<line2.start.x && line1.start.x<line2.end.x && line1.end.x<line2.end.x)
				return false;
			else if (line1.start.y<line2.start.y && line1.end.y<line2.start.y && line1.start.y<line2.end.y && line1.end.y<line2.end.y)
				return false;
			// else the lines are overlapping to some extent
			else
			{
				// find the points which are not contributing to the
				// common part
				sf::Vector2<T> maxp;
				sf::Vector2<T> minp;
				if ((line2.start.x>line1.start.x && line2.start.x>line1.end.x && line2.start.x>line2.end.x) ||
						(line2.start.y>line1.start.y && line2.start.y>line1.end.y && line2.start.y>line2.end.y))
					maxp=line2.start;
				else if ((line2.end.x>line1.start.x && line2.end.x>line1.end.x && line2.end.x>line2.start.x) ||
						(line2.end.y>line1.start.y && line2.end.y>line1.end.y && line2.end.y>line2.start.y))
					maxp=line2.end;
				else if ((line1.start.x>line2.start.x && line1.start.x>line1.end.x && line1.start.x>line2.end.x) ||
						(line1.start.y>line2.start.y && line1.start.y>line1.end.y && line1.start.y>line2.end.y))
					maxp=line1.start;
				else
					maxp=line1.end;
				if (maxp != line2.start && ((line2.start.x<line1.start.x && line2.start.x<line1.end.x && line2.start.x<line2.end.x) ||
						(line2.start.y<line1.start.y && line2.start.y<line1.end.y && line2.start.y<line2.end.y)))
					minp=line2.start;
				else if (maxp != line2.end && ((line2.end.x<line1.start.x && line2.end.x<line1.end.x && line2.end.x<line2.start.x) ||
						(line2.end.y<line1.start.y && line2.end.y<line1.end.y && line2.end.y<line2.start.y)))
					minp=line2.end;
				else if (maxp != line1.start && ((line1.start.x<line2.start.x && line1.start.x<line1.end.x && line1.start.x<line2.end.x)
						|| (line1.start.y<line2.start.y && line1.start.y<line1.end.y && line1.start.y<line2.end.y)))
					minp=line1.start;
				else
					minp=line1.end;

				// one line is contained in the other. Pick the center
				// of the remaining points, which overlap for sure
				out = sf::Vector2<T>();
				if (line2.start != maxp && line2.start != minp)
					out += line2.start;
				if (line2.end != maxp && line2.end != minp)
					out += line2.end;
				if (line1.start != maxp && line1.start != minp)
					out += line1.start;
				if (line1.end != maxp && line1.end != minp)
					out += line1.end;
				out.x = static_cast<T>(out.x/2);
				out.y = static_cast<T>(out.y/2);
			}

			if ( outPtr ) {
				*outPtr = out;
			}
			return true; // coincident
		}

		return false; // parallel
	}

	const auto intersectionData = calculateIntersectionPoint(line1, line2, data);

	if(intersectionData.uA < 0.f || intersectionData.uA > 1.f ||
			intersectionData.uB < 0.f || intersectionData.uB > 1.f) {
		return false; // Outside the line segment
	}

	if ( outPtr ) {
		*outPtr = intersectionData.intersectionPoint;
	}

	return true;
}

template <typename T>
bool isParallel(const Line2<T>& line1, const Line2<T>& line2) {
	return equals(getLineData(line1, line2).commonDenominator, 0.f);
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


