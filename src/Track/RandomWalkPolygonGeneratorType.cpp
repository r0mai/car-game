#include "RandomWalkPolygonGeneratorType.hpp"
#include "optionsUtil.hpp"
#include "randomWalks.hpp"

namespace po = boost::program_options;

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
			("random-walk-algorithm", po::value(&randomWalkAlgorithm)->required(),
					("The algorithm used to traverse the graph. " +
					randomWalk::getHelpString()).c_str())

			;
}

std::function<std::vector<sf::Vector2f>(RandomGenerator&)>
RandomWalkPolygonGeneratorType::getPolygonCreator(
		const boost::program_options::variables_map& /*variablesMap*/) {
	generatorParams.randomWalkAlgorithm = randomWalk::getRandomWalk(randomWalkAlgorithm);
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


