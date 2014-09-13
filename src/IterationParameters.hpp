#ifndef SRC_ITERATIONPARAMETERS_HPP
#define SRC_ITERATIONPARAMETERS_HPP

#include <boost/optional.hpp>

namespace car {

struct IterationParameters {
	boost::optional<unsigned> generationLimit;
	unsigned printoutFrequency = 1;
	std::string fitnessScript = "fitnessDefault.lua";
};

}

#endif /* SRC_ITERATIONPARAMETERS_HPP */
