#include "RandomWalkPolygonGeneratorType.hpp"
#include "optionsUtil.hpp"
#include "WilsonRandomWalk.hpp"

namespace car { namespace track {

RandomWalkPolygonGeneratorType::RandomWalkPolygonGeneratorType():
		BasicRandomPolygonGeneratorType{"random-walk"} {
	optionsDescription.add_options()
			("horizontal-resolution",
					paramWithDefaultValue(generatorParams.horizontalResolution),
					"The horizontal size of the grid.")
			("vertical-resolution",
					paramWithDefaultValue(generatorParams.verticalResolution),
					"The vertical size of the grid.")
			("grid-size", paramWithDefaultValue(generatorParams.gridSize),
					"The width and height of each cell in the grid.")
			("jitter", paramWithDefaultValue(generatorParams.jitter),
					"The maximum amount to shift each point in the grid in each direction.")
			;
}

std::function<std::vector<sf::Vector2f>(RandomGenerator&)>
RandomWalkPolygonGeneratorType::getPolygonCreator(
		const boost::program_options::variables_map& /*variablesMap*/) {
	generatorParams.randomWalkAlgorithm = std::make_shared<randomWalk::WilsonRandomWalk>();
	return RandomWalkPolygonGenerator{generatorParams};
}

std::string RandomWalkPolygonGeneratorType::getHelpString() {
	std::ostringstream ss;
	ss << "Take a random walk from one corner of the grid to the other corner in one part, then\n"
			"take another random walk back in the other part.\n" <<
			optionsDescription;
	return ss.str();
}

}} /* namespace car::track */


