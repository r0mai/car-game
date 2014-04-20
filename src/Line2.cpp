#include "Line2.hpp"
#include "mathUtil.hpp"

namespace car {

namespace {

template <typename T>
bool intersects(const Line2<T>& line1, const Line2<T>& line2, sf::Vector2<T> *outPtr) {
	sf::Vector2<T> out;
	const float commonDenominator = (float)(line1.end.y - line1.start.y)*(line2.end.x - line2.start.x) -
									(line1.end.x - line1.start.x)*(line2.end.y - line2.start.y);

	const float numeratorA = (float)(line1.end.x - line1.start.x)*(line2.start.y - line1.start.y) -
									(line1.end.y - line1.start.y)*(line2.start.x -line1.start.x);

	const float numeratorB = (float)(line2.end.x - line2.start.x)*(line2.start.y - line1.start.y) -
									(line2.end.y - line2.start.y)*(line2.start.x -line1.start.x);

	if(equals(commonDenominator, 0.f))
	{
		// The lines are either coincident or parallel
		// if both numerators are 0, the lines are coincident
		if(equals(numeratorA, 0.f) && equals(numeratorB, 0.f))
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

	// I don't care about this right now
	bool checkOnlySegments = true;
	// Get the point of intersection on this line, checking that
	// it is within the line segment.
	const float uA = numeratorA / commonDenominator;
	if(checkOnlySegments && (uA < 0.f || uA > 1.f) )
		return false; // Outside the line segment

	const float uB = numeratorB / commonDenominator;
	if(checkOnlySegments && (uB < 0.f || uB > 1.f))
		return false; // Outside the line segment

	// Calculate the intersection point.
	out.x = static_cast<T>(line2.start.x + uA * (line2.end.x - line2.start.x));
	out.y = static_cast<T>(line2.start.y + uA * (line2.end.y - line2.start.y));
	if ( outPtr ) {
		*outPtr = out;
	}
	return true;
}

} //  unnamed namespace

bool intersects(const Line2f& line1, const Line2f& line2, sf::Vector2f *outPtr) {
	return intersects<float>(line1, line2, outPtr);
}

} // namespace car


