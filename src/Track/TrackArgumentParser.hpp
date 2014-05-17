#ifndef TRACKCREATOR_HPP_
#define TRACKCREATOR_HPP_

#include <vector>
#include <string>
#include <functional>
#include "Parameters.hpp"
#include "OptionParseError.hpp"
#include "IPolygonType.hpp"

namespace car {

class Track;

struct TrackCreatorError: OptionParseError {
	using OptionParseError::OptionParseError;
};

namespace trackArgumentParser {

std::vector<std::function<Track()>> parseArguments(const std::vector<std::string>& args);
std::shared_ptr<IPolygonType> getPolygonType(const std::string& name);
std::string getHelpString();

}

}



#endif /* TRACKCREATOR_HPP_ */
