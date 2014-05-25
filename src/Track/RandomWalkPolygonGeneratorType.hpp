#ifndef SRC_TRACK_RANDOMWALKPOLYGONGENERATORTYPE_HPP
#define SRC_TRACK_RANDOMWALKPOLYGONGENERATORTYPE_HPP

#include "BasicRandomPolygonGeneratorType.hpp"
#include "RandomWalkPolygonGenerator.hpp"

namespace car { namespace track {

class RandomWalkPolygonGeneratorType: public BasicRandomPolygonGeneratorType {
public:
	RandomWalkPolygonGeneratorType();
	virtual std::function<std::vector<sf::Vector2f>(RandomGenerator&)>
			getPolygonCreator(
					const boost::program_options::variables_map& variablesMap) override;
	virtual std::string getHelpString() override;
private:
	RandomWalkPolygonGenerator::Params generatorParams;
};

}} /* namespace car::track */



#endif /* SRC_TRACK_RANDOMWALKPOLYGONGENERATORTYPE_HPP */
