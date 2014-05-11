#ifndef TRACKCREATOR_HPP_
#define TRACKCREATOR_HPP_

#include <vector>
#include <string>
#include <functional>
#include "Parameters.hpp"
#include "OptionParseError.hpp"

namespace car {

class Track;

struct TrackCreatorError: OptionParseError {
	using OptionParseError::OptionParseError;
};

class TrackArgumentParser {
public:
	TrackArgumentParser(const Parameters& parameters): parameters(parameters) {}

	std::vector<std::function<Track()>>
	operator()(const std::vector<std::string>& args);
private:
	const Parameters& parameters;

	std::function<Track()> parseArgument(const std::string& arg);
};

}



#endif /* TRACKCREATOR_HPP_ */
