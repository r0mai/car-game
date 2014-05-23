#ifndef SRC_TRACK_IRANDOMPOLYGONGENERATORTYPE_HPP
#define SRC_TRACK_IRANDOMPOLYGONGENERATORTYPE_HPP

#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <SFML/Graphics.hpp>
#include "RandomGenerator.hpp"

namespace car { namespace track {

class Track;

class IRandomPolygonGeneratorType {
public:
	virtual std::function<std::vector<sf::Vector2f>(RandomGenerator&)>
			getPolygonCreator(const boost::program_options::variables_map& variablesMap) = 0;
	virtual std::string getHelpString() = 0;
	virtual std::string getArgumentName() = 0;
	virtual boost::program_options::options_description getOptions() = 0;
	virtual ~IRandomPolygonGeneratorType() {}
};

}} /* namespace car::track */



#endif /* SRC_TRACK_IRANDOMPOLYGONGENERATORTYPE_HPP */
