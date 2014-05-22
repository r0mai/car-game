#ifndef SRC_TRACK_POINTADDERRANDOMPOLYGONGENERATORTYPE_HPP
#define SRC_TRACK_POINTADDERRANDOMPOLYGONGENERATORTYPE_HPP

#include "BasicRandomPolygonGeneratorType.hpp"
#include "PointAdderRandomPolygonGenerator.hpp"

namespace car { namespace track {

class PointAdderRandomPolygonGeneratorType: public BasicRandomPolygonGeneratorType {
public:
	PointAdderRandomPolygonGeneratorType();
	virtual std::function<std::vector<sf::Vector2f>(RandomGenerator&)>
			getPolygonCreator(
					const boost::program_options::variables_map& variablesMap) override;
	virtual std::string getHelpString() override;
private:
	std::string corner1 = "-60.0,-60.0";
	std::string corner2 = "60.0,60.0";
	PointAdderRandomPolygonGenerator::Params generatorParams;
};

}} /* namespace car::track */




#endif /* SRC_TRACK_POINTADDERRANDOMPOLYGONGENERATORTYPE_HPP */
