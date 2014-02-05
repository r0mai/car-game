#ifndef LINE_HPP
#define LINE_HPP

#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "mathUtil.hpp"

namespace car {

template<class T>
struct Line2 {

	Line2() = default;
	Line2(const sf::Vector2<T>& start, const sf::Vector2<T>& end) : start(start), end(end) {}

	Line2(T xa, T ya, T xb, T yb) : start(xa, ya), end(xb, yb) {}

	//code based on irrlicht line2d::intersectWith
	bool intersectWith(const Line2<T>& line, sf::Vector2<T> *outPtr = 0) const {
		sf::Vector2<T> out;
		const float commonDenominator = (float)(line.end.y - line.start.y)*(end.x - start.x) -
										(line.end.x - line.start.x)*(end.y - start.y);

		const float numeratorA = (float)(line.end.x - line.start.x)*(start.y - line.start.y) -
										(line.end.y - line.start.y)*(start.x -line.start.x);

		const float numeratorB = (float)(end.x - start.x)*(start.y - line.start.y) -
										(end.y - start.y)*(start.x -line.start.x);

		if(equals(commonDenominator, 0.f))
		{
			// The lines are either coincident or parallel
			// if both numerators are 0, the lines are coincident
			if(equals(numeratorA, 0.f) && equals(numeratorB, 0.f))
			{
				// Try and find a common endpoint
				if(line.start == start || line.end == start)
					out = start;
				else if(line.end == end || line.start == end)
					out = end;
				// now check if the two segments are disjunct
				else if (line.start.x>start.x && line.end.x>start.x && line.start.x>end.x && line.end.x>end.x)
					return false;
				else if (line.start.y>start.y && line.end.y>start.y && line.start.y>end.y && line.end.y>end.y)
					return false;
				else if (line.start.x<start.x && line.end.x<start.x && line.start.x<end.x && line.end.x<end.x)
					return false;
				else if (line.start.y<start.y && line.end.y<start.y && line.start.y<end.y && line.end.y<end.y)
					return false;
				// else the lines are overlapping to some extent
				else
				{
					// find the points which are not contributing to the
					// common part
					sf::Vector2<T> maxp;
					sf::Vector2<T> minp;
					if ((start.x>line.start.x && start.x>line.end.x && start.x>end.x) ||
							(start.y>line.start.y && start.y>line.end.y && start.y>end.y))
						maxp=start;
					else if ((end.x>line.start.x && end.x>line.end.x && end.x>start.x) ||
							(end.y>line.start.y && end.y>line.end.y && end.y>start.y))
						maxp=end;
					else if ((line.start.x>start.x && line.start.x>line.end.x && line.start.x>end.x) ||
							(line.start.y>start.y && line.start.y>line.end.y && line.start.y>end.y))
						maxp=line.start;
					else
						maxp=line.end;
					if (maxp != start && ((start.x<line.start.x && start.x<line.end.x && start.x<end.x) ||
							(start.y<line.start.y && start.y<line.end.y && start.y<end.y)))
						minp=start;
					else if (maxp != end && ((end.x<line.start.x && end.x<line.end.x && end.x<start.x) ||
							(end.y<line.start.y && end.y<line.end.y && end.y<start.y)))
						minp=end;
					else if (maxp != line.start && ((line.start.x<start.x && line.start.x<line.end.x && line.start.x<end.x)
							|| (line.start.y<start.y && line.start.y<line.end.y && line.start.y<end.y)))
						minp=line.start;
					else
						minp=line.end;

					// one line is contained in the other. Pick the center
					// of the remaining points, which overlap for sure
					out = sf::Vector2<T>();
					if (start != maxp && start != minp)
						out += start;
					if (end != maxp && end != minp)
						out += end;
					if (line.start != maxp && line.start != minp)
						out += line.start;
					if (line.end != maxp && line.end != minp)
						out += line.end;
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
		out.x = static_cast<T>(start.x + uA * (end.x - start.x));
		out.y = static_cast<T>(start.y + uA * (end.y - start.y));
		if ( outPtr ) {
			*outPtr = out;
		}
		return true;
	}

	sf::Vector2<T> start;
	sf::Vector2<T> end;
};

typedef Line2<float> Line2f;

}

#endif /* !LINE_HPP */
