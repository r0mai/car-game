#include "BasicTrackType.hpp"

namespace car {

BasicTrackType::BasicTrackType(std::string argumentName):
		argumentName(std::move(argumentName))
{
}

std::string BasicTrackType::getArgumentName() {
	return argumentName;
}
boost::program_options::options_description BasicTrackType::getOptions() {
	return optionsDescription;
}

}




