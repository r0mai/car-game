#ifndef TRACKCREATOR_HPP_
#define TRACKCREATOR_HPP_

#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include "Parameters.hpp"

namespace car {

class Track;

struct TrackCreatorError: std::logic_error {
	TrackCreatorError(const std::string& what_arg):
		std::logic_error{what_arg}
	{}
};

class TrackCreator {
public:
	TrackCreator(const Parameters& parameters): parameters(parameters) {}

	std::vector<std::function<Track()>>
	operator()(const std::vector<std::string>& args);
private:
	const Parameters& parameters;

	std::function<Track()> parseArgument(const std::string& arg);
};

}



#endif /* TRACKCREATOR_HPP_ */
