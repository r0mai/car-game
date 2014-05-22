#include "PointAdderRandomPolygonGeneratorType.hpp"
#include "optionsUtil.hpp"

namespace car { namespace track {

PointAdderRandomPolygonGeneratorType::PointAdderRandomPolygonGeneratorType():
		BasicRandomPolygonGeneratorType{"point-adder"} {
	optionsDescription.add_options()
			("number-of-points", paramWithDefaultValue(generatorParams.numberOfPoints),
					"The number of edges of the generated track. Minimum is 4 as the starting "
					"shape is a rectangle.")
			("corner1", paramWithDefaultValue(corner1),
					"The first corner of the rectangle where to generate points.")
			("corner2", paramWithDefaultValue(corner2),
					"The second corner of the rectangle where to generate points.")
			("inset", paramWithDefaultValue(generatorParams.inset),
					"The relative position within the generating rectangle where the "
					"initial points are placed (in ratio of the total size).")
			;
}

std::function<std::vector<sf::Vector2f>(RandomGenerator&)>
PointAdderRandomPolygonGeneratorType::getPolygonCreator(
		const boost::program_options::variables_map& /*variablesMap*/) {

	generatorParams.corner1 = parsePoint(corner1);
	generatorParams.corner2 = parsePoint(corner2);

	return PointAdderRandomPolygonGenerator{generatorParams};
}

std::string PointAdderRandomPolygonGeneratorType::getHelpString() {
	std::ostringstream ss;
	ss << "The track starts out as a rectangle. Points are generated within the\n"
			"selected area, and each point splits the edge of the polygon nearest to it.\n" << 
			optionsDescription;
	return ss.str();
}

}} /* namespace car::track */


