#ifndef TRACKCREATOR_HPP_
#define TRACKCREATOR_HPP_

#include <vector>
#include <string>
#include <functional>
#include "TrackCreatorError.hpp"
#include "IPolygonType.hpp"
#include "IRandomPolygonGeneratorType.hpp"
#include "TrackCreator.hpp"

namespace car { namespace track {

class Track;

namespace trackArgumentParser {

TrackCreators parseArguments(const std::vector<std::string>& args);
std::shared_ptr<IPolygonType> getPolygonType(const std::string& name);
std::shared_ptr<IRandomPolygonGeneratorType>
getRandomPolygonGeneratorType(const std::string& name);
std::string getHelpString();

}

}} /* namespace car::track */



#endif /* TRACKCREATOR_HPP_ */
